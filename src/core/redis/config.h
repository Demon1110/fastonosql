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

#include <string>

#include "core/config/config.h"

namespace fastonosql {
namespace core {
namespace redis {

struct Config : public RemoteConfig {
  Config();
  Config(const Config& other);
  Config& operator=(const Config& other);

  std::string hostsocket;
  uint64_t interval;
  int dbnum;
  int interactive;
  int latency_mode;
  int latency_history;
  int cluster_mode;
  int cluster_reissue_command;
  int slave_mode;
  int getrdb_mode;
  int stat_mode;
  int scan_mode;
  std::string pattern;
  std::string rdb_filename;
  int bigkeys;
  std::string auth;

 protected:
  void copy(const Config& other);
  void init();
};

}  // namespace redis
}  // namespace core
}  // namespace fastonosql

namespace common {
std::string ConvertToString(const fastonosql::core::redis::Config& conf);
}  // namespace common
