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

#include "core/db/unqlite/command.h"

#include <algorithm>  // for transform

namespace fastonosql {
namespace core {
namespace unqlite {

Command::Command(FastoObject* parent, common::CommandValue* cmd, const std::string& delimiter)
    : FastoObjectCommand(parent, cmd, delimiter, UNQLITE) {}

bool Command::IsReadOnly() const {
  std::string key = InputCmd();
  if (key.empty()) {
    return true;
  }

  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  return key != "get";
}

}  // namespace unqlite
}  // namespace core
}  // namespace fastonosql
