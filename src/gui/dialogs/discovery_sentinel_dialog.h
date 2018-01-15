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

#include <QDialog>

#include "core/server/iserver_info.h"
#include "proxy/connection_settings/iconnection_settings.h"  // for IConnectionSettingsBaseSPtr

class QLabel;  // lines 29-29
class QShowEvent;
class QTreeWidget;  // lines 30-30
class QWidget;

namespace common {
namespace qt {
namespace gui {
class GlassWidget;
}
}  // namespace qt
}  // namespace common

namespace fastonosql {
namespace gui {
class ConnectionListWidgetItemDiscovered;

class DiscoverySentinelDiagnosticDialog : public QDialog {
  Q_OBJECT
 public:
  enum { fix_height = 320, fix_width = 480 };

  DiscoverySentinelDiagnosticDialog(QWidget* parent, proxy::IConnectionSettingsBaseSPtr connection);
  std::vector<ConnectionListWidgetItemDiscovered*> selectedConnections() const;

 private Q_SLOTS:
  void connectionResultReady(bool suc,
                             qint64 mstimeExecute,
                             const QString& resultText,
                             std::vector<core::ServerDiscoverySentinelInfoSPtr> infos);

 protected:
  virtual void showEvent(QShowEvent* e) override;

 private:
  void testConnection(proxy::IConnectionSettingsBaseSPtr connection);

  common::qt::gui::GlassWidget* glass_widget_;
  QLabel* execute_time_label_;
  QLabel* status_label_;
  QTreeWidget* list_widget_;
  QLabel* icon_label_;
};

}  // namespace gui
}  // namespace fastonosql
