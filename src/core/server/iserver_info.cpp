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

#include "core/server/iserver_info.h"

#include <string>  // for string

namespace fastonosql {
namespace core {

ServerCommonInfo::ServerCommonInfo() : name(), type(MASTER), state(SUP), cstate(SCONNECTED), host() {}

ServerCommonInfo::ServerCommonInfo(const std::string& name,
                                   serverTypes type,
                                   serverState state,
                                   serverConnectState cstate)
    : name(name), type(type), state(state), cstate(cstate) {}

ServerDiscoveryInfoBase::ServerDiscoveryInfoBase(connectionTypes ctype, const ServerCommonInfo& info)
    : ctype_(ctype), info_(info) {}

connectionTypes ServerDiscoveryInfoBase::connectionType() const {
  return ctype_;
}

ServerCommonInfo ServerDiscoveryInfoBase::info() const {
  return info_;
}

std::string ServerDiscoveryInfoBase::name() const {
  return info_.name;
}

void ServerDiscoveryInfoBase::setName(const std::string& name) {
  info_.name = name;
}

common::net::HostAndPortAndSlot ServerDiscoveryInfoBase::host() const {
  return info_.host;
}

void ServerDiscoveryInfoBase::setHost(const common::net::HostAndPortAndSlot& host) {
  info_.host = host;
}

ServerDiscoveryInfoBase::~ServerDiscoveryInfoBase() {}

ServerDiscoveryClusterInfo::ServerDiscoveryClusterInfo(connectionTypes ctype, const ServerCommonInfo& info, bool self)
    : ServerDiscoveryInfoBase(ctype, info), self_(self) {}

ServerDiscoveryClusterInfo::~ServerDiscoveryClusterInfo() {}

bool ServerDiscoveryClusterInfo::self() const {
  return self_;
}

IStateField::~IStateField() {}

IServerInfo::~IServerInfo() {}

connectionTypes IServerInfo::Type() const {
  return type_;
}

IServerInfo::IServerInfo(connectionTypes type) : type_(type) {}

ServerDiscoverySentinelInfo::ServerDiscoverySentinelInfo(connectionTypes ctype, const ServerCommonInfo& info)
    : ServerDiscoveryInfoBase(ctype, info) {}

ServerDiscoverySentinelInfo::~ServerDiscoverySentinelInfo() {}

ServerInfoSnapShoot::ServerInfoSnapShoot() : msec(0), info() {}

ServerInfoSnapShoot::ServerInfoSnapShoot(common::time64_t msec, IServerInfoSPtr info) : msec(msec), info(info) {}

bool ServerInfoSnapShoot::isValid() const {
  return msec > 0 && info;
}

}  // namespace core
}  // namespace fastonosql
