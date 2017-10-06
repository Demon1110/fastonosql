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

#include "proxy/events/events_info.h"

namespace fastonosql {
namespace proxy {
namespace events {

typedef common::qt::Event<events_info::ConnectInfoRequest, QEvent::User + 1> ConnectRequestEvent;
typedef common::qt::Event<events_info::ConnectInfoResponce, QEvent::User + 2> ConnectResponceEvent;

typedef common::qt::Event<events_info::ShutDownInfoRequest, QEvent::User + 3> ShutDownRequestEvent;
typedef common::qt::Event<events_info::ShutDownInfoResponce, QEvent::User + 4> ShutDownResponceEvent;

typedef common::qt::Event<events_info::CommandRootCreatedInfo, QEvent::User + 5> CommandRootCreatedEvent;
typedef common::qt::Event<events_info::CommandRootCompleatedInfo, QEvent::User + 6> CommandRootCompleatedEvent;

typedef common::qt::Event<events_info::EnterModeInfo, QEvent::User + 7> EnterModeEvent;
typedef common::qt::Event<events_info::LeaveModeInfo, QEvent::User + 8> LeaveModeEvent;

typedef common::qt::Event<events_info::DisConnectInfoRequest, QEvent::User + 9> DisconnectRequestEvent;
typedef common::qt::Event<events_info::DisConnectInfoResponce, QEvent::User + 10> DisconnectResponceEvent;

typedef common::qt::Event<events_info::ExecuteInfoRequest, QEvent::User + 11> ExecuteRequestEvent;
typedef common::qt::Event<events_info::ExecuteInfoResponce, QEvent::User + 12> ExecuteResponceEvent;

typedef common::qt::Event<events_info::LoadDatabasesInfoRequest, QEvent::User + 13> LoadDatabasesInfoRequestEvent;
typedef common::qt::Event<events_info::LoadDatabasesInfoResponce, QEvent::User + 14> LoadDatabasesInfoResponceEvent;

typedef common::qt::Event<events_info::ServerInfoRequest, QEvent::User + 15> ServerInfoRequestEvent;
typedef common::qt::Event<events_info::ServerInfoResponce, QEvent::User + 16> ServerInfoResponceEvent;

typedef common::qt::Event<events_info::ServerInfoHistoryRequest, QEvent::User + 17> ServerInfoHistoryRequestEvent;
typedef common::qt::Event<events_info::ServerInfoHistoryResponce, QEvent::User + 18> ServerInfoHistoryResponceEvent;

typedef common::qt::Event<events_info::ClearServerHistoryRequest, QEvent::User + 19> ClearServerHistoryRequestEvent;
typedef common::qt::Event<events_info::ClearServerHistoryResponce, QEvent::User + 20> ClearServerHistoryResponceEvent;

typedef common::qt::Event<events_info::ServerPropertyInfoRequest, QEvent::User + 21> ServerPropertyInfoRequestEvent;
typedef common::qt::Event<events_info::ServerPropertyInfoResponce, QEvent::User + 22> ServerPropertyInfoResponceEvent;

typedef common::qt::Event<events_info::ChangeServerPropertyInfoRequest, QEvent::User + 23>
    ChangeServerPropertyInfoRequestEvent;
typedef common::qt::Event<events_info::ChangeServerPropertyInfoResponce, QEvent::User + 24>
    ChangeServerPropertyInfoResponceEvent;

typedef common::qt::Event<events_info::LoadServerChannelsRequest, QEvent::User + 25> LoadServerChannelsRequestEvent;
typedef common::qt::Event<events_info::LoadServerChannelsResponce, QEvent::User + 26> LoadServerChannelsResponceEvent;

typedef common::qt::Event<events_info::BackupInfoRequest, QEvent::User + 27> ImportRequestEvent;
typedef common::qt::Event<events_info::BackupInfoResponce, QEvent::User + 28> ImportResponceEvent;

typedef common::qt::Event<events_info::ExportInfoRequest, QEvent::User + 29> ExportRequestEvent;
typedef common::qt::Event<events_info::ExportInfoResponce, QEvent::User + 30> ExportResponceEvent;

typedef common::qt::Event<events_info::LoadDatabaseContentRequest, QEvent::User + 31> LoadDatabaseContentRequestEvent;
typedef common::qt::Event<events_info::LoadDatabaseContentResponce, QEvent::User + 32> LoadDatabaseContentResponceEvent;

typedef common::qt::Event<events_info::DiscoveryInfoRequest, QEvent::User + 33> DiscoveryInfoRequestEvent;
typedef common::qt::Event<events_info::DiscoveryInfoResponce, QEvent::User + 34> DiscoveryInfoResponceEvent;

typedef common::qt::Event<events_info::ChangePasswordRequest, QEvent::User + 35> ChangePasswordRequestEvent;
typedef common::qt::Event<events_info::ChangePasswordResponce, QEvent::User + 36> ChangePasswordResponceEvent;

typedef common::qt::Event<events_info::ChangeMaxConnectionRequest, QEvent::User + 37> ChangeMaxConnectionRequestEvent;
typedef common::qt::Event<events_info::ChangeMaxConnectionResponce, QEvent::User + 38> ChangeMaxConnectionResponceEvent;

typedef common::qt::Event<events_info::ProgressInfoResponce, QEvent::User + 100> ProgressResponceEvent;

}  // namespace events
}  // namespace proxy
}  // namespace fastonosql
