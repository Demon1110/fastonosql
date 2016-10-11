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

#include "core/driver/root_locker.h"

namespace fastonosql {
namespace core {

class FirstChildUpdateRootLocker : public RootLocker {
 public:
  FirstChildUpdateRootLocker(IDriver* parent,
                             QObject* receiver,
                             const std::string& text,
                             bool silence);

 private:
  // notification of execute events
  virtual void addedChildren(FastoObjectIPtr child) override;

  FastoObjectIPtr watched_child_;
};

}  // namespace core
}  // namespace fastonosql