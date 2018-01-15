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

#include "gui/dialogs/connection_dialog.h"

#include <QDialogButtonBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>

#include <common/qt/convert2string.h>  // for ConvertToString

#include "proxy/connection_settings_factory.h"

#include "gui/connection_widgets_factory.h"
#include "gui/dialogs/connection_diagnostic_dialog.h"
#include "gui/gui_factory.h"  // for GuiFactory

#include "translations/global.h"  // for trShow, trPrivateKey, etc

namespace {
const QString trTitle = QObject::tr("Connection Settings");
const QString trPrivateKeyInvalidInput = QObject::tr("Invalid private key value!");
const std::string defaultNameConnectionFolder = "/";

}  // namespace

namespace fastonosql {
namespace gui {

ConnectionDialog::ConnectionDialog(core::connectionTypes type, const QString& connectionName, QWidget* parent)
    : QDialog(parent), connection_() {
  proxy::connection_path_t path(common::file_system::stable_dir_path(defaultNameConnectionFolder) +
                                common::ConvertToString(connectionName));
  proxy::IConnectionSettingsBase* connection =
      proxy::ConnectionSettingsFactory().GetInstance().CreateFromType(type, path);
  init(connection);
}

ConnectionDialog::ConnectionDialog(proxy::IConnectionSettingsBase* connection, QWidget* parent)
    : QDialog(parent), connection_() {
  CHECK(connection);
  init(connection);
}

proxy::IConnectionSettingsBaseSPtr ConnectionDialog::connection() const {
  return connection_;
}

void ConnectionDialog::setFolderEnabled(bool val) {
  connection_widget_->setUIFolderEnabled(val);
}

void ConnectionDialog::accept() {
  if (validateAndApply()) {
    QDialog::accept();
  }
}

void ConnectionDialog::testConnection() {
  if (validateAndApply()) {
    ConnectionDiagnosticDialog diag(this, connection_);
    diag.exec();
  }
}

void ConnectionDialog::changeEvent(QEvent* e) {
  if (e->type() == QEvent::LanguageChange) {
    retranslateUi();
  }
  QDialog::changeEvent(e);
}

void ConnectionDialog::init(proxy::IConnectionSettingsBase* connection) {
  setWindowIcon(GuiFactory::GetInstance().GetIcon(connection->GetType()));
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);  // Remove help
                                                                     // button (?)

  connection_.reset(connection);
  connection_widget_ = ConnectionWidgetsFactory::GetInstance().createWidget(connection);
  QLayout* connection_widget_layout = connection_widget_->layout();
  QMargins mar = connection_widget_layout->contentsMargins();
  connection_widget_layout->setContentsMargins(0, 0, 0, mar.bottom());

  QHBoxLayout* bottomLayout = new QHBoxLayout;
  test_button_ = new QPushButton("&Test");
  test_button_->setIcon(GuiFactory::GetInstance().GetMessageBoxInformationIcon());
  VERIFY(connect(test_button_, &QPushButton::clicked, this, &ConnectionDialog::testConnection));

  bottomLayout->addWidget(test_button_, 1, Qt::AlignLeft);
  button_box_ = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Save);
  button_box_->setOrientation(Qt::Horizontal);
  VERIFY(connect(button_box_, &QDialogButtonBox::accepted, this, &ConnectionDialog::accept));
  VERIFY(connect(button_box_, &QDialogButtonBox::rejected, this, &ConnectionDialog::reject));
  bottomLayout->addWidget(button_box_);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(connection_widget_);
  mainLayout->addLayout(bottomLayout);
  mainLayout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(mainLayout);

  retranslateUi();
}

void ConnectionDialog::retranslateUi() {
  setWindowTitle(trTitle);
}

bool ConnectionDialog::validateAndApply() {
  if (!connection_widget_->validated()) {
    return false;
  }

  connection_.reset(connection_widget_->createConnection());
  return true;
}

}  // namespace gui
}  // namespace fastonosql
