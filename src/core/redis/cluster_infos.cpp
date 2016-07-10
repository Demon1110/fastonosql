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

#include "core/redis/cluster_infos.h"

#define MARKER "\n"

namespace fastonosql {
namespace core {
namespace redis {

DiscoveryClusterInfo::DiscoveryClusterInfo(const ServerCommonInfo& info, bool self)
  : ServerDiscoveryClusterInfo(REDIS, info, self) {
}


namespace {

common::Error makeServerCommonInfoFromLine(const std::string& line, ServerCommonInfo* info, bool* self) {
  if (line.empty() || !info || !self) {
    return common::make_error_value("Invalid input argument(s)", common::ErrorValue::E_ERROR);
  }

  ServerCommonInfo linfo;
  bool lself = false;
  int fieldpos = 0;
  std::string word;
  for (size_t i = 0; i < line.size(); ++i) {
    char ch = line[i];
    if (ch == ' ') {
      switch (fieldpos) {
      case 0: {
        linfo.name = word;
        break;
      }
      case 1: {
        linfo.host = common::ConvertFromString<common::net::HostAndPortAndSlot>(word);
        break;
      }
      case 2: {
        if (word.find("slave") != std::string::npos) {
          linfo.type = SLAVE;
        }
        lself = word.find("myself") != std::string::npos;
        break;
      }
      default:
        break;
      }
      word.clear();
      ++fieldpos;
    } else {
      word += ch;
    }
  }

  *info = linfo;
  *self = lself;
  return common::Error();
}

}

common::Error makeDiscoveryClusterInfo(const common::net::HostAndPort& parentHost,
                                   const std::string& text,
                                   std::vector<ServerDiscoveryClusterInfoSPtr>* infos) {
  if (text.empty() || !infos) {
    return common::make_error_value("Invalid input argument(s)", common::ErrorValue::E_ERROR);;
  }

  size_t pos = 0;
  size_t start = 0;

  while ((pos = text.find(MARKER, start)) != std::string::npos) {
    std::string line = text.substr(start, pos - start);

    ServerCommonInfo inf;
    bool self = false;
    common::Error lerr = makeServerCommonInfoFromLine(line, &inf, &self);
    if (lerr && lerr->isError()) {
      continue;
    }
    if (common::net::isLocalHost(inf.host.host)) {
      inf.host.host = parentHost.host;
    }

    ServerDiscoveryClusterInfoSPtr ser(new DiscoveryClusterInfo(inf, self));
    infos->push_back(ser);
    start = pos + 1;
  }

  return common::Error();
}

}  // namespace redis
}  // namespace core
}  // namespace fastonosql
