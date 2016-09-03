/*  Copyright (C) 2014-2016 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it
   and/or modify
    it under the terms of the GNU General Public License as
   published by
    the Free Software Foundation, either version 3 of the
   License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be
   useful,
    but WITHOUT ANY WARRANTY; without even the implied
   warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General
   Public License
    along with FastoNoSQL.  If not, see
   <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QDialog>

#include "core/core_fwd.h"              // for IServerSPtr
#include "core/server_property_info.h"  // for property_t

class QEvent;
class QShowEvent;
class QTableView;  // lines 26-26
class QWidget;

namespace fasto {
namespace qt {
namespace gui {
class GlassWidget;
}
}
}  // lines 31-31
namespace fastonosql {
namespace core {
namespace events_info {
struct ChangeServerPropertyInfoRequest;
}
}
}
namespace fastonosql {
namespace core {
namespace events_info {
struct ChangeServerPropertyInfoResponce;
}
}
}
namespace fastonosql {
namespace core {
namespace events_info {
struct ServerPropertyInfoRequest;
}
}
}
namespace fastonosql {
namespace core {
namespace events_info {
struct ServerPropertyInfoResponce;
}
}
}

namespace fastonosql {
namespace gui {

class PropertyServerDialog : public QDialog {
  Q_OBJECT
 public:
  explicit PropertyServerDialog(core::IServerSPtr server, QWidget* parent = 0);
  enum { min_width = 240, min_height = 200 };

 private Q_SLOTS:
  void startServerProperty(const core::events_info::ServerPropertyInfoRequest& req);
  void finishServerProperty(const core::events_info::ServerPropertyInfoResponce& res);

  void startServerChangeProperty(const core::events_info::ChangeServerPropertyInfoRequest& req);
  void finishServerChangeProperty(const core::events_info::ChangeServerPropertyInfoResponce& res);

  void changedProperty(const core::property_t& prop);

 protected:
  virtual void changeEvent(QEvent* e);
  virtual void showEvent(QShowEvent* e);

 private:
  void retranslateUi();

  fasto::qt::gui::GlassWidget* glassWidget_;
  QTableView* propertyes_table_;
  const core::IServerSPtr server_;
};

}  // namespace gui
}  // namespace fastonosql
