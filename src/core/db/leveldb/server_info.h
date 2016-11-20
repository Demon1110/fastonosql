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

#include <stdint.h>  // for uint32_t

#include <iosfwd>  // for ostream
#include <string>  // for string

#include "core/types.h"
#include "core/server/iserver_info.h"

#define LEVELDB_STATS_LABEL "# Stats"

#define LEVELDB_CAMPACTIONS_LEVEL_LABEL "compactions_level"
#define LEVELDB_FILE_SIZE_MB_LABEL "file_size_mb"
#define LEVELDB_TIME_SEC_LABEL "time_sec"
#define LEVELDB_READ_MB_LABEL "read_mb"
#define LEVELDB_WRITE_MB_LABEL "write_mb"

namespace common {
class Value;
}

namespace fastonosql {
namespace core {
namespace leveldb {

class ServerInfo : public IServerInfo {
 public:
  // Compactions\nLevel  Files Size(MB) Time(sec) Read(MB)
  // Write(MB)\n
  struct Stats : IStateField {
    Stats();
    explicit Stats(const std::string& common_text);
    virtual common::Value* ValueByIndex(unsigned char index) const override;

    uint32_t compactions_level;
    uint32_t file_size_mb;
    uint32_t time_sec;
    uint32_t read_mb;
    uint32_t write_mb;
  } stats_;

  ServerInfo();
  explicit ServerInfo(const Stats& stats);

  virtual common::Value* ValueByIndexes(unsigned char property, unsigned char field) const override;
  virtual std::string ToString() const override;
  virtual uint32_t Version() const override;
};

std::ostream& operator<<(std::ostream& out, const ServerInfo& value);

ServerInfo* MakeLeveldbServerInfo(const std::string& content);

}  // namespace leveldb
}  // namespace core
}  // namespace fastonosql
