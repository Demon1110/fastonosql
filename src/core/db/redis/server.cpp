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

#include "core/db/redis/server.h"

#include <string>  // for operator==, basic_string

#include <common/error.h>   // for Error
#include <common/macros.h>  // for CHECK

#include "core/server/iserver_info.h"

#include "core/events/events_info.h"  // for DiscoveryInfoResponce

#include "core/db/redis/database.h"      // for Database
#include "core/db/redis/driver.h"        // for Driver
#include "core/db/redis/server_info.h"   // for ServerInfo, etc

namespace fastonosql {
namespace core {
namespace redis {

Server::Server(IConnectionSettingsBaseSPtr settings)
    : IServerRemote(new Driver(settings)), role_(MASTER), mode_(STANDALONE) {}

serverTypes Server::Role() const {
  return role_;
}

serverMode Server::Mode() const {
  return mode_;
}

serverState Server::State() const {
  return SUP;
}

common::net::HostAndPort Server::Host() const {
  Driver* const rdrv = static_cast<Driver* const>(drv_);
  return rdrv->Host();
}

IDatabaseSPtr Server::CreateDatabase(IDataBaseInfoSPtr info) {
  return IDatabaseSPtr(new Database(shared_from_this(), info));
}

void Server::HandleDiscoveryInfoResponceEvent(events::DiscoveryInfoResponceEvent* ev) {
  auto v = ev->value();
  common::Error er = v.errorInfo();
  if (!er) {
    struct ServerInfo* rinf = dynamic_cast<struct ServerInfo*>(v.sinfo.get());  // +
    CHECK(rinf);
    if (rinf->replication_.role_ == "master") {
      role_ = MASTER;
    } else if (rinf->replication_.role_ == "slave") {
      role_ = SLAVE;
    }

    if (rinf->server_.redis_mode_ == "standalone") {
      mode_ = STANDALONE;
    } else if (rinf->server_.redis_mode_ == "sentinel") {
      mode_ = SENTINEL;
    } else if (rinf->server_.redis_mode_ == "cluster") {
      mode_ = CLUSTER;
    }
  }
  IServer::HandleDiscoveryInfoResponceEvent(ev);
}

}  // namespace redis
}  // namespace core
}  // namespace fastonosql
