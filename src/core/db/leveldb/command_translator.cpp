/*  Copyright (C) 2014-2017 FastoGT. All right reserved.

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

#include "core/db/leveldb/command_translator.h"

#include <common/macros.h>   // for UNUSED
#include <common/sprintf.h>  // for MemSPrintf

#define LEVELDB_GET_KEY_COMMAND COMMONTYPE_GET_KEY_COMMAND
#define LEVELDB_SET_KEY_COMMAND COMMONTYPE_SET_KEY_COMMAND
#define LEVELDB_RENAME_KEY_COMMAND "RENAME"
#define LEVELDB_DELETE_KEY_COMMAND "DEL"

namespace fastonosql {
namespace core {
namespace leveldb {

CommandTranslator::CommandTranslator(const std::vector<CommandHolder>& commands) : ICommandTranslator(commands) {}

common::Error CommandTranslator::CreateKeyCommandImpl(const NDbKValue& key, command_buffer_t* cmdstring) const {
  const NKey cur = key.GetKey();
  key_t key_str = cur.GetKey();
  std::string value_str = key.ValueString();
  string_byte_writer_t wr;
  wr << LEVELDB_SET_KEY_COMMAND << " " << key_str << " " << value_str;
  *cmdstring = wr.GetBuffer();
  return common::Error();
}

common::Error CommandTranslator::LoadKeyCommandImpl(const NKey& key,
                                                    common::Value::Type type,
                                                    command_buffer_t* cmdstring) const {
  UNUSED(type);

  key_t key_str = key.GetKey();
  string_byte_writer_t wr;
  wr << LEVELDB_GET_KEY_COMMAND << " " << key_str;
  *cmdstring = wr.GetBuffer();
  return common::Error();
}

common::Error CommandTranslator::DeleteKeyCommandImpl(const NKey& key, command_buffer_t* cmdstring) const {
  key_t key_str = key.GetKey();
  string_byte_writer_t wr;
  wr << LEVELDB_DELETE_KEY_COMMAND << " " << key_str;
  *cmdstring = wr.GetBuffer();
  return common::Error();
}

common::Error CommandTranslator::RenameKeyCommandImpl(const NKey& key,
                                                      const std::string& new_name,
                                                      command_buffer_t* cmdstring) const {
  key_t key_str = key.GetKey();
  string_byte_writer_t wr;
  wr << LEVELDB_RENAME_KEY_COMMAND << " " << key_str << " " << new_name;
  *cmdstring = wr.GetBuffer();
  return common::Error();
}

common::Error CommandTranslator::ChangeKeyTTLCommandImpl(const NKey& key,
                                                         ttl_t ttl,
                                                         command_buffer_t* cmdstring) const {
  UNUSED(key);
  UNUSED(ttl);
  UNUSED(cmdstring);

  static const std::string error_msg =
      "Sorry, but now " PROJECT_NAME_TITLE " not supported change ttl command for LevelDB.";
  return common::make_error_value(error_msg, common::ErrorValue::E_ERROR);
}

common::Error CommandTranslator::LoadKeyTTLCommandImpl(const NKey& key, command_buffer_t* cmdstring) const {
  UNUSED(key);
  UNUSED(cmdstring);

  static const std::string error_msg =
      "Sorry, but now " PROJECT_NAME_TITLE " not supported get ttl command for LevelDB.";
  return common::make_error_value(error_msg, common::ErrorValue::E_ERROR);
}

bool CommandTranslator::IsLoadKeyCommandImpl(const CommandInfo& cmd) const {
  return cmd.IsEqualName(LEVELDB_GET_KEY_COMMAND);
}

common::Error CommandTranslator::PublishCommandImpl(const NDbPSChannel& channel,
                                                    const std::string& message,
                                                    command_buffer_t* cmdstring) const {
  UNUSED(channel);
  UNUSED(message);
  UNUSED(cmdstring);

  static const std::string error_msg =
      "Sorry, but now " PROJECT_NAME_TITLE " not supported publish command for LevelDB.";
  return common::make_error_value(error_msg, common::ErrorValue::E_ERROR);
}

common::Error CommandTranslator::SubscribeCommandImpl(const NDbPSChannel& channel, command_buffer_t* cmdstring) const {
  UNUSED(channel);
  UNUSED(cmdstring);

  static const std::string error_msg =
      "Sorry, but now " PROJECT_NAME_TITLE " not supported subscribe command for LevelDB.";
  return common::make_error_value(error_msg, common::ErrorValue::E_ERROR);
}

}  // namespace leveldb
}  // namespace core
}  // namespace fastonosql
