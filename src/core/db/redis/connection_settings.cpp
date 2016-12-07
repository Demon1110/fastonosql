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

#include "core/db/redis/connection_settings.h"

#include <string>  // for string

#include <common/net/types.h>  // for HostAndPort
#include <common/convert2string.h>

#include "core/connection_settings/iconnection_settings_ssh.h"

#include "core/db/redis/config.h"  // for Config

namespace fastonosql {
namespace core {
namespace redis {

ConnectionSettings::ConnectionSettings(const connection_path_t& connectionName)
    : IConnectionSettingsRemoteSSH(connectionName, REDIS), info_() {}

std::string ConnectionSettings::Delimiter() const {
  return info_.delimiter;
}

void ConnectionSettings::SetDelimiter(const std::string& delimiter) {
  info_.delimiter = delimiter;
}

std::string ConnectionSettings::NsSeparator() const {
  return info_.ns_separator;
}

void ConnectionSettings::SetNsSeparator(const std::string& ns) {
  info_.ns_separator = ns;
}

common::net::HostAndPort ConnectionSettings::Host() const {
  return info_.host;
}

void ConnectionSettings::SetHost(const common::net::HostAndPort& host) {
  info_.host = host;
}

void ConnectionSettings::SetCommandLine(const std::string& line) {
  info_ = common::ConvertFromString<Config>(line);
}

std::string ConnectionSettings::CommandLine() const {
  return common::ConvertToString(info_);
}

Config ConnectionSettings::Info() const {
  return info_;
}

void ConnectionSettings::SetInfo(const Config& info) {
  info_ = info;
}

ConnectionSettings* ConnectionSettings::Clone() const {
  ConnectionSettings* red = new ConnectionSettings(*this);
  return red;
}

}  // namespace redis
}  // namespace core
}  // namespace fastonosql
