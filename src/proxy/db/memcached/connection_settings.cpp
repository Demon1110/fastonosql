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

#include "proxy/db/memcached/connection_settings.h"

#include <string>

#include <common/convert2string.h>  // for ConvertFromString

#include "core/connection_types.h"  // for core::connectionTypes::MEMCACHED

namespace fastonosql {
namespace proxy {
namespace memcached {

ConnectionSettings::ConnectionSettings(const connection_path_t& connectionName)
    : IConnectionSettingsRemote(connectionName, core::MEMCACHED), info_() {}

core::memcached::Config ConnectionSettings::Info() const {
  return info_;
}

void ConnectionSettings::SetInfo(const core::memcached::Config& info) {
  info_ = info;
}

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

std::string ConnectionSettings::CommandLine() const {
  return common::ConvertToString(info_);
}

void ConnectionSettings::SetCommandLine(const std::string& line) {
  info_ = common::ConvertFromString<core::memcached::Config>(line);
}

ConnectionSettings* ConnectionSettings::Clone() const {
  ConnectionSettings* red = new ConnectionSettings(*this);
  return red;
}

}  // namespace memcached
}  // namespace proxy
}  // namespace fastonosql
