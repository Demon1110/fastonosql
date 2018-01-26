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

#include "gui/db/pika/connection_widget.h"

#include <QCheckBox>
#include <QEvent>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExpValidator>
#include <QSpinBox>

#include <common/convert2string.h>
#include <common/qt/convert2string.h>

#include "proxy/db/pika/connection_settings.h"

#include "gui/widgets/host_port_widget.h"
#include "gui/widgets/path_widget.h"
#include "gui/widgets/ssh_widget.h"

#include "translations/global.h"

namespace {
const QString trUnixPath = QObject::tr("Unix socket path:");
const QString trSSL = QObject::tr("SSL");
}  // namespace

namespace fastonosql {
namespace gui {
namespace pika {

ConnectionWidget::ConnectionWidget(QWidget* parent) : ConnectionBaseWidget(parent) {
  QVBoxLayout* vbox = new QVBoxLayout;

  isSSLConnection_ = new QCheckBox;
  VERIFY(connect(isSSLConnection_, &QCheckBox::stateChanged, this, &ConnectionWidget::sslStateChange));

  QHBoxLayout* hbox = new QHBoxLayout;
  hbox->addWidget(isSSLConnection_);

  host_widget_ = new HostPortWidget;
  QLayout* host_layout = host_widget_->layout();
  host_layout->setContentsMargins(0, 0, 0, 0);
  vbox->addLayout(hbox);
  vbox->addWidget(host_widget_);
  addLayout(vbox);

  useAuth_ = new QCheckBox;
  VERIFY(connect(useAuth_, &QCheckBox::stateChanged, this, &ConnectionWidget::authStateChange));
  addWidget(useAuth_);

  QHBoxLayout* passwordLayout = new QHBoxLayout;
  password_box_ = new QLineEdit;
  password_box_->setEchoMode(QLineEdit::Password);
  password_echo_mode_button_ = new QPushButton(translations::trShow);
  VERIFY(connect(password_echo_mode_button_, &QPushButton::clicked, this, &ConnectionWidget::togglePasswordEchoMode));
  passwordLayout->addWidget(password_box_);
  passwordLayout->addWidget(password_echo_mode_button_);
  addLayout(passwordLayout);

  QHBoxLayout* def_layout = new QHBoxLayout;
  default_db_label_ = new QLabel;

  default_db_num_ = new QSpinBox;
  default_db_num_->setRange(0, INT32_MAX);
  default_db_num_->setEnabled(false);
  def_layout->addWidget(default_db_label_);
  def_layout->addWidget(default_db_num_);
  addLayout(def_layout);

  // ssh

  sshWidget_ = new SSHWidget;
  QLayout* ssh_layout = sshWidget_->layout();
  ssh_layout->setContentsMargins(0, 0, 0, 0);
  addWidget(sshWidget_);

  useAuth_->setChecked(false);
  password_box_->setEnabled(false);
  password_echo_mode_button_->setEnabled(false);
}

void ConnectionWidget::syncControls(proxy::IConnectionSettingsBase* connection) {
  proxy::pika::ConnectionSettings* pika = static_cast<proxy::pika::ConnectionSettings*>(connection);
  if (pika) {
    core::pika::Config config = pika->GetInfo();
    host_widget_->setHost(config.host);
    isSSLConnection_->setChecked(config.is_ssl);

    std::string auth = config.auth;
    if (!auth.empty()) {
      useAuth_->setChecked(true);
      QString qauth;
      common::ConvertFromString(auth, &qauth);
      password_box_->setText(qauth);
    } else {
      useAuth_->setChecked(false);
      password_box_->clear();
    }
    default_db_num_->setValue(config.db_num);
    core::SSHInfo ssh_info = pika->GetSSHInfo();
    sshWidget_->setInfo(ssh_info);
  }
  ConnectionBaseWidget::syncControls(pika);
}

void ConnectionWidget::retranslateUi() {
  isSSLConnection_->setText(trSSL);
  useAuth_->setText(trUseAuth);
  default_db_label_->setText(trDefaultDb);
  ConnectionBaseWidget::retranslateUi();
}

void ConnectionWidget::togglePasswordEchoMode() {
  bool isPassword = password_box_->echoMode() == QLineEdit::Password;
  password_box_->setEchoMode(isPassword ? QLineEdit::Normal : QLineEdit::Password);
  password_echo_mode_button_->setText(isPassword ? translations::trHide : translations::trShow);
}

void ConnectionWidget::authStateChange(int state) {
  password_box_->setEnabled(state);
  password_echo_mode_button_->setEnabled(state);
}

void ConnectionWidget::sslStateChange(int state) {
  sshWidget_->setEnabled(!state);
}

bool ConnectionWidget::validated() const {
  if (sshWidget_->isEnabled() && sshWidget_->isSSHChecked()) {
    if (!sshWidget_->isValidSSHInfo()) {
      return false;
    }
  }

  if (!isValidCredential()) {
    return false;
  }

  if (!host_widget_->isValidHost()) {
    return false;
  }

  return ConnectionBaseWidget::validated();
}

bool ConnectionWidget::isValidCredential() const {
  if (useAuth_->isChecked()) {
    QString pass = password_box_->text();
    return !pass.isEmpty();
  }

  return true;
}

proxy::IConnectionSettingsBase* ConnectionWidget::createConnectionImpl(const proxy::connection_path_t& path) const {
  proxy::pika::ConnectionSettings* conn = new proxy::pika::ConnectionSettings(path);
  core::pika::Config config = conn->GetInfo();
  config.host = host_widget_->host();
  config.is_ssl = isSSLConnection_->isChecked();

  if (useAuth_->isChecked() && isValidCredential()) {
    config.auth = common::ConvertToString(password_box_->text());
  }
  config.db_num = default_db_num_->value();
  conn->SetInfo(config);

  core::SSHInfo info;
  if (sshWidget_->isEnabled() && sshWidget_->isSSHChecked()) {
    info = sshWidget_->info();
  }
  conn->SetSSHInfo(info);
  return conn;
}

}  // namespace pika
}  // namespace gui
}  // namespace fastonosql
