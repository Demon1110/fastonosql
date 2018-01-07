/*  Copyright (C) 2014-2018 FastoGT. All right reserved.

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

#include <QObject>

#include "core/server/iserver_info.h"
#include "proxy/connection_settings/iconnection_settings.h"  // for IConnectionSettingsBaseSPtr

namespace fastonosql {
namespace gui {

class DiscoverySentinelConnection : public QObject {
  Q_OBJECT
 public:
  explicit DiscoverySentinelConnection(proxy::IConnectionSettingsBaseSPtr conn, QObject* parent = Q_NULLPTR);

 Q_SIGNALS:
  void connectionResult(bool suc,
                        qint64 msTimeExecute,
                        const QString& resultText,
                        std::vector<core::ServerDiscoverySentinelInfoSPtr> infos);

 public Q_SLOTS:
  void routine();

 private:
  proxy::IConnectionSettingsBaseSPtr connection_;
  common::time64_t startTime_;
};

}  // namespace gui
}  // namespace fastonosql
