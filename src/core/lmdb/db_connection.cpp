/*  Copyright (C) 2014-2016 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FastoNoSQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core/lmdb/db_connection.h"

#include <errno.h>   // for EACCES
#include <lmdb.h>    // for mdb_txn_abort, MDB_val
#include <stdlib.h>  // for NULL, free, calloc
#include <time.h>    // for time_t
#include <string>    // for string

#include <common/value.h>  // for StringValue (ptr only)
#include <common/utils.h>  // for c_strornull
#include <common/convert2string.h>

#include "core/lmdb/config.h"               // for Config
#include "core/lmdb/connection_settings.h"  // for ConnectionSettings
#include "core/lmdb/command_translator.h"
#include "core/lmdb/database.h"

#include "global/global.h"  // for FastoObject, etc

namespace fastonosql {
class FastoObjectArray;
}

#define LMDB_OK 0

namespace fastonosql {
namespace core {
namespace lmdb {

struct lmdb {
  MDB_env* env;
  MDB_dbi dbir;
};

namespace {

int lmdb_open(lmdb** context, const char* dbname, bool create_if_missing) {
  if (create_if_missing) {
    common::Error err = common::file_system::create_directory(dbname, true);
    if (err && err->isError()) {
      return EACCES;
    }
    if (common::file_system::is_directory(dbname) != common::SUCCESS) {
      return EACCES;
    }
  }

  lmdb* lcontext = reinterpret_cast<lmdb*>(calloc(1, sizeof(lmdb)));
  int rc = mdb_env_create(&lcontext->env);
  if (rc != LMDB_OK) {
    free(lcontext);
    return rc;
  }
  rc = mdb_env_open(lcontext->env, dbname, 0, 0664);
  if (rc != LMDB_OK) {
    free(lcontext);
    return rc;
  }

  MDB_txn* txn = NULL;
  rc = mdb_txn_begin(lcontext->env, NULL, 0, &txn);
  if (rc != LMDB_OK) {
    free(lcontext);
    return rc;
  }

  rc = mdb_dbi_open(txn, NULL, 0, &lcontext->dbir);
  mdb_txn_abort(txn);
  if (rc != LMDB_OK) {
    free(lcontext);
    return rc;
  }

  *context = lcontext;
  return rc;
}

void lmdb_close(lmdb** context) {
  if (!context) {
    return;
  }

  lmdb* lcontext = *context;
  if (!lcontext) {
    return;
  }

  mdb_dbi_close(lcontext->env, lcontext->dbir);
  mdb_env_close(lcontext->env);
  free(lcontext);
  *context = NULL;
}

}  // namespace

common::Error createConnection(const Config& config, NativeConnection** context) {
  if (!context) {
    return common::make_error_value("Invalid input argument(s)", common::ErrorValue::E_ERROR);
  }

  DCHECK(*context == NULL);
  struct lmdb* lcontext = NULL;
  std::string folder = config.dbname;  // start point must be folder
  common::tribool is_dir = common::file_system::is_directory(folder);
  if (is_dir != common::SUCCESS) {
    folder = common::file_system::get_dir_path(folder);
  }
  const char* dbname = common::utils::c_strornull(folder);
  int st = lmdb_open(&lcontext, dbname, config.create_if_missing);
  if (st != LMDB_OK) {
    std::string buff = common::MemSPrintf("Fail open database: %s", mdb_strerror(st));
    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
  }

  *context = lcontext;
  return common::Error();
}

common::Error createConnection(ConnectionSettings* settings, NativeConnection** context) {
  if (!settings) {
    return common::make_error_value("Invalid input argument(s)", common::ErrorValue::E_ERROR);
  }

  Config config = settings->info();
  return createConnection(config, context);
}

common::Error testConnection(ConnectionSettings* settings) {
  if (!settings) {
    return common::make_error_value("Invalid input argument(s)", common::ErrorValue::E_ERROR);
  }

  struct lmdb* ldb = NULL;
  common::Error er = createConnection(settings, &ldb);
  if (er && er->isError()) {
    return er;
  }

  lmdb_close(&ldb);
  return common::Error();
}

DBConnection::DBConnection(CDBConnectionClient* client)
    : base_class(lmdbCommands, client, new CommandTranslator) {}

const char* DBConnection::versionApi() {
  return STRINGIZE(MDB_VERSION_MAJOR) "." STRINGIZE(MDB_VERSION_MINOR) "." STRINGIZE(
      MDB_VERSION_PATCH);
}

unsigned int DBConnection::curDb() const {
  if (connection_.handle_) {
    return connection_.handle_->dbir;
  }

  return 0;
}

common::Error DBConnection::info(const char* args, ServerInfo::Stats* statsout) {
  UNUSED(args);

  if (!isConnected()) {
    DNOTREACHED();
    return common::make_error_value("Not connected", common::Value::E_ERROR);
  }

  if (!statsout) {
    NOTREACHED();
    return common::make_error_value("Invalid input argument for command: INFO",
                                    common::ErrorValue::E_ERROR);
  }

  ServerInfo::Stats linfo;
  Config conf = config();
  linfo.db_path = conf.dbname;

  *statsout = linfo;
  return common::Error();
}

common::Error DBConnection::dbkcount(size_t* size) {
  if (!isConnected()) {
    DNOTREACHED();
    return common::make_error_value("Not connected", common::Value::E_ERROR);
  }

  if (!size) {
    return common::make_error_value("Invalid input argument(s)", common::ErrorValue::E_ERROR);
  }

  MDB_cursor* cursor = NULL;
  MDB_txn* txn = NULL;
  int rc = mdb_txn_begin(connection_.handle_->env, NULL, MDB_RDONLY, &txn);
  if (rc == LMDB_OK) {
    rc = mdb_cursor_open(txn, connection_.handle_->dbir, &cursor);
  }

  if (rc != LMDB_OK) {
    mdb_txn_abort(txn);
    std::string buff = common::MemSPrintf("DBKCOUNT function error: %s", mdb_strerror(rc));
    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
  }

  MDB_val key;
  MDB_val data;
  size_t sz = 0;
  while (mdb_cursor_get(cursor, &key, &data, MDB_NEXT) == LMDB_OK) {
    sz++;
  }
  mdb_cursor_close(cursor);
  mdb_txn_abort(txn);

  *size = sz;
  return common::Error();
}

common::Error DBConnection::setInner(const std::string& key, const std::string& value) {
  if (!isConnected()) {
    DNOTREACHED();
    return common::make_error_value("Not connected", common::Value::E_ERROR);
  }

  MDB_val mkey;
  mkey.mv_size = key.size();
  mkey.mv_data = const_cast<char*>(key.c_str());
  MDB_val mval;
  mval.mv_size = value.size();
  mval.mv_data = const_cast<char*>(value.c_str());

  MDB_txn* txn = NULL;
  int rc = mdb_txn_begin(connection_.handle_->env, NULL, 0, &txn);
  if (rc == LMDB_OK) {
    rc = mdb_put(txn, connection_.handle_->dbir, &mkey, &mval, 0);
    if (rc == LMDB_OK) {
      rc = mdb_txn_commit(txn);
    } else {
      mdb_txn_abort(txn);
    }
  }

  if (rc != LMDB_OK) {
    std::string buff = common::MemSPrintf("set function error: %s", mdb_strerror(rc));
    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
  }

  return common::Error();
}

common::Error DBConnection::getInner(const std::string& key, std::string* ret_val) {
  if (!isConnected()) {
    DNOTREACHED();
    return common::make_error_value("Not connected", common::Value::E_ERROR);
  }

  MDB_val mkey;
  mkey.mv_size = key.size();
  mkey.mv_data = const_cast<char*>(key.c_str());
  MDB_val mval;

  MDB_txn* txn = NULL;
  int rc = mdb_txn_begin(connection_.handle_->env, NULL, MDB_RDONLY, &txn);
  if (rc == LMDB_OK) {
    rc = mdb_get(txn, connection_.handle_->dbir, &mkey, &mval);
  }
  mdb_txn_abort(txn);

  if (rc != LMDB_OK) {
    std::string buff = common::MemSPrintf("get function error: %s", mdb_strerror(rc));
    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
  }

  ret_val->assign(reinterpret_cast<const char*>(mval.mv_data), mval.mv_size);
  return common::Error();
}

common::Error DBConnection::delInner(const std::string& key) {
  if (!isConnected()) {
    DNOTREACHED();
    return common::make_error_value("Not connected", common::Value::E_ERROR);
  }

  MDB_val mkey;
  mkey.mv_size = key.size();
  mkey.mv_data = const_cast<char*>(key.c_str());

  MDB_txn* txn = NULL;
  int rc = mdb_txn_begin(connection_.handle_->env, NULL, 0, &txn);
  if (rc == LMDB_OK) {
    rc = mdb_del(txn, connection_.handle_->dbir, &mkey, NULL);
    if (rc == LMDB_OK) {
      rc = mdb_txn_commit(txn);
    } else {
      mdb_txn_abort(txn);
    }
  }

  if (rc != LMDB_OK) {
    std::string buff = common::MemSPrintf("delete function error: %s", mdb_strerror(rc));
    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
  }

  return common::Error();
}

common::Error DBConnection::keys(const std::string& key_start,
                                 const std::string& key_end,
                                 uint64_t limit,
                                 std::vector<std::string>* ret) {
  if (!isConnected()) {
    DNOTREACHED();
    return common::make_error_value("Not connected", common::Value::E_ERROR);
  }

  MDB_cursor* cursor = NULL;
  MDB_txn* txn = NULL;
  int rc = mdb_txn_begin(connection_.handle_->env, NULL, MDB_RDONLY, &txn);
  if (rc == LMDB_OK) {
    rc = mdb_cursor_open(txn, connection_.handle_->dbir, &cursor);
  }

  if (rc != LMDB_OK) {
    mdb_txn_abort(txn);
    std::string buff = common::MemSPrintf("Keys function error: %s", mdb_strerror(rc));
    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
  }

  MDB_val key;
  MDB_val data;
  while ((mdb_cursor_get(cursor, &key, &data, MDB_NEXT) == LMDB_OK) && limit > ret->size()) {
    std::string skey(reinterpret_cast<const char*>(key.mv_data), key.mv_size);
    if (key_start < skey && key_end > skey) {
      ret->push_back(skey);
    }
  }

  mdb_cursor_close(cursor);
  mdb_txn_abort(txn);
  return common::Error();
}

common::Error DBConnection::help(int argc, const char** argv) {
  UNUSED(argc);
  UNUSED(argv);

  return notSupported("HELP");
}

common::Error DBConnection::flushdb() {
  if (!isConnected()) {
    DNOTREACHED();
    return common::make_error_value("Not connected", common::Value::E_ERROR);
  }

  MDB_cursor* cursor = NULL;
  MDB_txn* txn = NULL;
  int rc = mdb_txn_begin(connection_.handle_->env, NULL, 0, &txn);
  if (rc == LMDB_OK) {
    rc = mdb_cursor_open(txn, connection_.handle_->dbir, &cursor);
  }

  if (rc != LMDB_OK) {
    mdb_txn_abort(txn);
    std::string buff = common::MemSPrintf("flushdb function error: %s", mdb_strerror(rc));
    return common::make_error_value(buff, common::ErrorValue::E_ERROR);
  }

  MDB_val key;
  MDB_val data;
  size_t sz = 0;
  while (mdb_cursor_get(cursor, &key, &data, MDB_NEXT) == LMDB_OK) {
    sz++;
    rc = mdb_del(txn, connection_.handle_->dbir, &key, NULL);
    if (rc != LMDB_OK) {
      mdb_cursor_close(cursor);
      mdb_txn_abort(txn);
      std::string buff = common::MemSPrintf("del function error: %s", mdb_strerror(rc));
      return common::make_error_value(buff, common::ErrorValue::E_ERROR);
    }
  }

  mdb_cursor_close(cursor);
  if (sz != 0) {
    rc = mdb_txn_commit(txn);
    if (rc != LMDB_OK) {
      std::string buff = common::MemSPrintf("commit function error: %s", mdb_strerror(rc));
      return common::make_error_value(buff, common::ErrorValue::E_ERROR);
    }
    return common::Error();
  }

  mdb_txn_abort(txn);
  return common::Error();
}

common::Error DBConnection::selectImpl(const std::string& name, IDataBaseInfo** info) {
  size_t kcount = 0;
  common::Error err = dbkcount(&kcount);
  MCHECK(!err);
  *info = new DataBaseInfo(name, true, kcount);
  return common::Error();
}

common::Error DBConnection::setImpl(const key_and_value_t& key, key_and_value_t* added_key) {
  std::string key_str = key.keyString();
  std::string value_str = key.valueString();
  common::Error err = setInner(key_str, value_str);
  if (err && err->isError()) {
    return err;
  }

  *added_key = key;
  return common::Error();
}

common::Error DBConnection::getImpl(const key_t& key, key_and_value_t* loaded_key) {
  std::string key_str = key.key();
  std::string value_str;
  common::Error err = getInner(key_str, &value_str);
  if (err && err->isError()) {
    return err;
  }

  common::StringValue* val = common::Value::createStringValue(value_str);
  *loaded_key = key_and_value_t(key, common::make_value(val));
  return common::Error();
}

common::Error DBConnection::delImpl(const keys_t& keys, keys_t* deleted_keys) {
  for (size_t i = 0; i < keys.size(); ++i) {
    NKey key = keys[i];
    std::string key_str = key.key();
    common::Error err = delInner(key_str);
    if (err && err->isError()) {
      continue;
    }

    deleted_keys->push_back(key);
  }

  return common::Error();
}

common::Error DBConnection::setTTLImpl(const key_t& key, ttl_t ttl) {
  UNUSED(key);
  UNUSED(ttl);
  return common::make_error_value("Sorry, but now " PROJECT_NAME_TITLE
                                  " for LMDB not supported TTL commands.",
                                  common::ErrorValue::E_ERROR);
}

common::Error info(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  DBConnection* mdb = static_cast<DBConnection*>(handler);
  ServerInfo::Stats statsout;
  common::Error er = mdb->info(argc == 1 ? argv[0] : nullptr, &statsout);
  if (!er) {
    common::StringValue* val = common::Value::createStringValue(ServerInfo(statsout).toString());
    FastoObject* child = new FastoObject(out, val, mdb->delimiter());
    out->addChildren(child);
  }

  return er;
}

common::Error dbkcount(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  UNUSED(argc);
  UNUSED(argv);

  DBConnection* mdb = static_cast<DBConnection*>(handler);
  size_t dbkcount = 0;
  common::Error er = mdb->dbkcount(&dbkcount);
  if (!er) {
    common::FundamentalValue* val = common::Value::createUIntegerValue(dbkcount);
    FastoObject* child = new FastoObject(out, val, mdb->delimiter());
    out->addChildren(child);
  }

  return er;
}

common::Error select(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  UNUSED(argc);

  DBConnection* mdb = static_cast<DBConnection*>(handler);
  common::Error err = mdb->select(argv[0], NULL);
  if (err && err->isError()) {
    return err;
  }

  common::StringValue* val = common::Value::createStringValue("OK");
  FastoObject* child = new FastoObject(out, val, mdb->delimiter());
  out->addChildren(child);
  return common::Error();
}

common::Error set(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  UNUSED(argc);

  NKey key(argv[0]);
  common::StringValue* string_val = common::Value::createStringValue(argv[1]);
  key_and_value_t kv(key, common::make_value(string_val));

  DBConnection* red = static_cast<DBConnection*>(handler);
  key_and_value_t key_added;
  common::Error err = red->set(kv, &key_added);
  if (err && err->isError()) {
    return err;
  }

  common::StringValue* val = common::Value::createStringValue("OK");
  FastoObject* child = new FastoObject(out, val, red->delimiter());
  out->addChildren(child);
  return common::Error();
}

common::Error get(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  UNUSED(argc);

  NKey key(argv[0]);
  DBConnection* unqlite = static_cast<DBConnection*>(handler);
  key_and_value_t key_loaded;
  common::Error err = unqlite->get(key, &key_loaded);
  if (err && err->isError()) {
    return err;
  }

  value_t val = key_loaded.value();
  common::Value* copy = val->deepCopy();
  FastoObject* child = new FastoObject(out, copy, unqlite->delimiter());
  out->addChildren(child);
  return common::Error();
}

common::Error del(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  keys_t keysdel;
  for (int i = 0; i < argc; ++i) {
    keysdel.push_back(NKey(argv[i]));
  }

  DBConnection* level = static_cast<DBConnection*>(handler);
  keys_t keys_deleted;
  common::Error err = level->del(keysdel, &keys_deleted);
  if (err && err->isError()) {
    return err;
  }

  common::FundamentalValue* val = common::Value::createUIntegerValue(keys_deleted.size());
  FastoObject* child = new FastoObject(out, val, level->delimiter());
  out->addChildren(child);
  return common::Error();
}

common::Error set_ttl(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  UNUSED(out);
  UNUSED(argc);

  DBConnection* mdb = static_cast<DBConnection*>(handler);
  key_t key(argv[0]);
  time_t ttl = common::ConvertFromString<time_t>(argv[1]);
  common::Error err = mdb->setTTL(key, ttl);
  if (err && err->isError()) {
    return err;
  }

  common::StringValue* val = common::Value::createStringValue("OK");
  FastoObject* child = new FastoObject(out, val, mdb->delimiter());
  out->addChildren(child);
  return common::Error();
}

common::Error keys(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  UNUSED(argc);

  DBConnection* mdb = static_cast<DBConnection*>(handler);
  std::vector<std::string> keysout;
  common::Error er =
      mdb->keys(argv[0], argv[1], common::ConvertFromString<uint64_t>(argv[2]), &keysout);
  if (!er) {
    common::ArrayValue* ar = common::Value::createArrayValue();
    for (size_t i = 0; i < keysout.size(); ++i) {
      common::StringValue* val = common::Value::createStringValue(keysout[i]);
      ar->append(val);
    }
    FastoObjectArray* child = new FastoObjectArray(out, ar, mdb->delimiter());
    out->addChildren(child);
  }

  return er;
}

common::Error help(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  UNUSED(out);

  DBConnection* mdb = static_cast<DBConnection*>(handler);
  return mdb->help(argc - 1, argv + 1);
}

common::Error flushdb(CommandHandler* handler, int argc, const char** argv, FastoObject* out) {
  UNUSED(argc);
  UNUSED(argv);
  UNUSED(out);

  DBConnection* mdb = static_cast<DBConnection*>(handler);
  return mdb->flushdb();
}

}  // namespace lmdb
template <>
common::Error ConnectionAllocatorTraits<lmdb::NativeConnection, lmdb::Config>::connect(
    const lmdb::Config& config,
    lmdb::NativeConnection** hout) {
  lmdb::NativeConnection* context = nullptr;
  common::Error er = lmdb::createConnection(config, &context);
  if (er && er->isError()) {
    return er;
  }

  *hout = context;
  return common::Error();
}
template <>
common::Error ConnectionAllocatorTraits<lmdb::NativeConnection, lmdb::Config>::disconnect(
    lmdb::NativeConnection** handle) {
  lmdb::lmdb_close(handle);
  *handle = nullptr;
  return common::Error();
}
template <>
bool ConnectionAllocatorTraits<lmdb::NativeConnection, lmdb::Config>::isConnected(
    lmdb::NativeConnection* handle) {
  if (!handle) {
    return false;
  }

  return true;
}
}  // namespace core
}  // namespace fastonosql
