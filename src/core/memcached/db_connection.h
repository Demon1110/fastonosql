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

#pragma once

#include <stdint.h>  // for uint32_t, uint64_t
#include <time.h>    // for time_t
#include <string>    // for string

#include <common/error.h>   // for Error
#include <common/macros.h>  // for WARN_UNUSED_RESULT

#include "core/command_info.h"             // for UNDEFINED_EXAMPLE_STR, UNDEF...
#include "core/connection_types.h"         // for connectionTypes::MEMCACHED
#include "core/db_key.h"                   // for NDbKValue, NKey, NKeys
#include "core/internal/cdb_connection.h"  // for CDBConnection
#include "core/memcached/connection_settings.h"
#include "core/memcached/server_info.h"

namespace fastonosql {
class FastoObject;
}
namespace fastonosql {
namespace core {
class CDBConnectionClient;
}
}
namespace fastonosql {
namespace core {
class CommandHandler;
}
}
namespace fastonosql {
namespace core {
class IDataBaseInfo;
}
}
namespace fastonosql {
namespace core {
namespace memcached {
class ConnectionSettings;
}
}
}

struct memcached_st;  // lines 37-37

namespace fastonosql {
namespace core {
namespace memcached {

typedef memcached_st NativeConnection;

common::Error CreateConnection(const Config& config, NativeConnection** context);
common::Error CreateConnection(ConnectionSettings* settings, NativeConnection** context);
common::Error TestConnection(ConnectionSettings* settings);

class DBConnection : public core::internal::CDBConnection<NativeConnection, Config, MEMCACHED> {
 public:
  typedef core::internal::CDBConnection<NativeConnection, Config, MEMCACHED> base_class;
  DBConnection(CDBConnectionClient* client);

  common::Error Keys(const std::string& key_start,
                     const std::string& key_end,
                     uint64_t limit,
                     std::vector<std::string>* ret) WARN_UNUSED_RESULT;
  common::Error Info(const char* args, ServerInfo::Stats* statsout) WARN_UNUSED_RESULT;
  common::Error DBkcount(size_t* size) WARN_UNUSED_RESULT;

  common::Error AddIfNotExist(const std::string& key,
                              const std::string& value,
                              time_t expiration,
                              uint32_t flags) WARN_UNUSED_RESULT;
  common::Error Replace(const std::string& key,
                        const std::string& value,
                        time_t expiration,
                        uint32_t flags) WARN_UNUSED_RESULT;
  common::Error Append(const std::string& key,
                       const std::string& value,
                       time_t expiration,
                       uint32_t flags) WARN_UNUSED_RESULT;
  common::Error Prepend(const std::string& key,
                        const std::string& value,
                        time_t expiration,
                        uint32_t flags) WARN_UNUSED_RESULT;
  common::Error Incr(const std::string& key, uint64_t value) WARN_UNUSED_RESULT;
  common::Error Decr(const std::string& key, uint64_t value) WARN_UNUSED_RESULT;
  common::Error Flushdb(time_t expiration) WARN_UNUSED_RESULT;
  common::Error VersionServer() const WARN_UNUSED_RESULT;
  common::Error Help(int argc, const char** argv) WARN_UNUSED_RESULT;

 private:
  common::Error DelInner(const std::string& key, time_t expiration) WARN_UNUSED_RESULT;
  common::Error GetInner(const std::string& key, std::string* ret_val) WARN_UNUSED_RESULT;
  common::Error SetInner(const std::string& key,
                         const std::string& value,
                         time_t expiration,
                         uint32_t flags) WARN_UNUSED_RESULT;
  common::Error ExpireInner(const std::string& key, time_t expiration) WARN_UNUSED_RESULT;

  virtual common::Error SelectImpl(const std::string& name, IDataBaseInfo** info) override;
  virtual common::Error DeleteImpl(const NKeys& keys, NKeys* deleted_keys) override;
  virtual common::Error GetImpl(const NKey& key, NDbKValue* loaded_key) override;
  virtual common::Error SetImpl(const NDbKValue& key, NDbKValue* added_key) override;
  virtual common::Error RenameImpl(const NKey& key, const std::string& new_key) override;
  virtual common::Error SetTTLImpl(const NKey& key, ttl_t ttl) override;
  virtual common::Error QuitImpl() override;
};

}  // namespace memcached
}  // namespace core
}  // namespace fastonosql
