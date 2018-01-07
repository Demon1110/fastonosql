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

#include <common/qt/gui/shortcuts.h>  // for FastoQKeySequence

class QKeyEvent;

namespace fastonosql {
namespace gui {

extern const common::qt::gui::FastoQKeySequence g_open_key;
extern const common::qt::gui::FastoQKeySequence g_new_key;
extern const common::qt::gui::FastoQKeySequence g_save_key;
extern const common::qt::gui::FastoQKeySequence g_save_as_key;
extern const common::qt::gui::FastoQKeySequence g_quit_key;
extern const common::qt::gui::FastoQKeySequence g_close_key;
extern const common::qt::gui::FastoQKeySequence g_new_tab_key;
extern const common::qt::gui::FastoQKeySequence g_next_tab_key;
extern const common::qt::gui::FastoQKeySequence g_prev_tab_key;
extern const common::qt::gui::FastoQKeySequence g_refresh_key;
extern const common::qt::gui::FastoQKeySequence g_full_screen_key;
extern const common::qt::gui::FastoQKeySequence g_execute_key;

bool IsOpenShortcut(QKeyEvent* keyEvent);
bool IsSaveShortcut(QKeyEvent* keyEvent);
bool IsSaveAsShortcut(QKeyEvent* keyEvent);
bool IsQuitShortcut(QKeyEvent* keyEvent);
bool IsCloseShortcut(QKeyEvent* keyEvent);
bool IsNewTabShortcut(QKeyEvent* keyEvent);
bool IsNextTabShortcut(QKeyEvent* keyEvent);
bool IsPreviousTabShortcut(QKeyEvent* keyEvent);
bool IsRefreshShortcut(QKeyEvent* keyEvent);
bool IsFullScreenShortcut(QKeyEvent* keyEvent);
bool IsExecuteScriptShortcut(QKeyEvent* keyEvent);

}  // namespace gui
}  // namespace fastonosql
