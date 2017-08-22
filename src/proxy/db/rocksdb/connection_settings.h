/*  Copyright (C) 2014-2017 FastoGT. All right reserved.

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

#include <string>

#include "proxy/connection_settings/iconnection_settings_local.h"

#include "core/db/rocksdb/config.h"

namespace fastonosql {
namespace proxy {
namespace rocksdb {

class ConnectionSettings : public IConnectionSettingsLocal {
 public:
  explicit ConnectionSettings(const connection_path_t& connectionName);

  core::rocksdb::Config Info() const;
  void SetInfo(const core::rocksdb::Config& info);

  virtual std::string GetDelimiter() const override;
  virtual void SetDelimiter(const std::string& delimiter) override;

  virtual std::string GetNsSeparator() const override;
  virtual void SetNsSeparator(const std::string& ns) override;

  virtual std::string DBPath() const override;
  virtual void SetDBPath(const std::string& db_path) override;

  virtual std::string CommandLine() const override;
  virtual void SetCommandLine(const std::string& line) override;

  virtual ConnectionSettings* Clone() const override;

 private:
  core::rocksdb::Config info_;
};

}  // namespace rocksdb
}  // namespace proxy
}  // namespace fastonosql
