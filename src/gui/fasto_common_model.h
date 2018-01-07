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

#include <common/qt/gui/base/tree_model.h>  // for TreeModel

namespace fastonosql {
namespace core {
class NDbKValue;
}
namespace gui {

class FastoCommonModel : public common::qt::gui::TreeModel {
  Q_OBJECT
 public:
  explicit FastoCommonModel(QObject* parent = Q_NULLPTR);

  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;
  virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  virtual int columnCount(const QModelIndex& parent) const override;

  void changeValue(const core::NDbKValue& value);

 Q_SIGNALS:
  void changedValue(const core::NDbKValue& value);
};

}  // namespace gui
}  // namespace fastonosql
