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

#include <common/qt/gui/base/table_model.h>  // for TableModel

#include "core/server_property_info.h"  // for property_t

namespace fastonosql {
namespace gui {

class PropertyTableModel : public common::qt::gui::TableModel {
  Q_OBJECT
 public:
  explicit PropertyTableModel(QObject* parent = 0);

  virtual QVariant data(const QModelIndex& index, int role) const;
  virtual bool setData(const QModelIndex& index, const QVariant& value, int role);
  virtual Qt::ItemFlags flags(const QModelIndex& index) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  virtual int columnCount(const QModelIndex& parent) const;

  void changeProperty(const core::property_t& pr);

 Q_SIGNALS:
  void propertyChanged(const core::property_t& pr);
};

}  // namespace gui
}  // namespace fastonosql
