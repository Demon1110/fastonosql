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

#include <string>  // for string

#include "proxy/connection_settings/iconnection_settings.h"  // for IConnectionSettingsBaseSPtr
#include "proxy/proxy_fwd.h"                                  // for IDatabaseSPtr
#include "proxy/server/iserver_local.h"                      // for IServerLocal

namespace fastonosql {
namespace proxy {
namespace leveldb {

class Server : public IServerLocal {
  Q_OBJECT
 public:
  explicit Server(IConnectionSettingsBaseSPtr settings);
  virtual std::string Path() const override;

 private:
  virtual IDatabaseSPtr CreateDatabase(core::IDataBaseInfoSPtr info) override;
};

}  // namespace leveldb
}  // namespace proxy
}  // namespace fastonosql
