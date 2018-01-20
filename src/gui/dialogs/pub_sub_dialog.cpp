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

#include "gui/dialogs/pub_sub_dialog.h"

#include <QDialogButtonBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>

#include <common/qt/convert2string.h>
#include <common/qt/logger.h>

#include "proxy/server/iserver.h"

#include "gui/channels_table_model.h"  // for KeysTableModel, etc
#include "gui/fasto_table_view.h"      // for FastoTableView
#include "gui/gui_factory.h"           // for GuiFactory

#include "translations/global.h"  // for trKeyCountOnThePage, etc

namespace {
const QString trPublishToChannel_1S = QObject::tr("Publish to channel %1");
const QString trEnterWhatYoWantToSend = QObject::tr("Enter what you want to send:");
const QString trSubscribeInNewConsole = QObject::tr("Subscribe in new console");
}  // namespace

namespace fastonosql {
namespace gui {

PubSubDialog::PubSubDialog(const QString& title, proxy::IServerSPtr server, QWidget* parent)
    : QDialog(parent),
      search_box_(nullptr),
      search_button_(nullptr),
      channels_table_(nullptr),
      channels_model_(nullptr),
      proxy_model_(nullptr),
      server_(server) {
  CHECK(server_);
  setWindowTitle(title);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);  // Remove help
                                                                     // button (?)

  VERIFY(
      connect(server.get(), &proxy::IServer::LoadServerChannelsStarted, this, &PubSubDialog::startLoadServerChannels));
  VERIFY(connect(server.get(), &proxy::IServer::LoadServerChannelsFinished, this,
                 &PubSubDialog::finishLoadServerChannels));

  // main layout
  QVBoxLayout* mainlayout = new QVBoxLayout;
  QHBoxLayout* searchLayout = new QHBoxLayout;
  search_box_ = new QLineEdit;
  search_box_->setText(ALL_PUBSUB_CHANNELS);
  VERIFY(connect(search_box_, &QLineEdit::textChanged, this, &PubSubDialog::searchLineChanged));
  searchLayout->addWidget(search_box_);

  search_button_ = new QPushButton;
  VERIFY(connect(search_button_, &QPushButton::clicked, this, &PubSubDialog::searchClicked));
  searchLayout->addWidget(search_button_);
  mainlayout->addLayout(searchLayout);

  channels_model_ = new ChannelsTableModel(this);
  proxy_model_ = new QSortFilterProxyModel(this);
  proxy_model_->setSourceModel(channels_model_);
  proxy_model_->setDynamicSortFilter(true);

  VERIFY(
      connect(server_.get(), &proxy::IServer::ExecuteStarted, this, &PubSubDialog::startExecute, Qt::DirectConnection));
  VERIFY(connect(server_.get(), &proxy::IServer::ExecuteFinished, this, &PubSubDialog::finishExecute,
                 Qt::DirectConnection));

  channels_table_ = new FastoTableView;
  channels_table_->setSortingEnabled(true);
  channels_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
  channels_table_->setSelectionMode(QAbstractItemView::SingleSelection);
  channels_table_->setContextMenuPolicy(Qt::CustomContextMenu);
  VERIFY(connect(channels_table_, &FastoTableView::customContextMenuRequested, this, &PubSubDialog::showContextMenu));
  channels_table_->sortByColumn(0, Qt::AscendingOrder);
  channels_table_->setModel(proxy_model_);

  QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
  buttonBox->setOrientation(Qt::Horizontal);
  VERIFY(connect(buttonBox, &QDialogButtonBox::accepted, this, &PubSubDialog::accept));
  VERIFY(connect(buttonBox, &QDialogButtonBox::rejected, this, &PubSubDialog::reject));
  mainlayout->addWidget(channels_table_);
  mainlayout->addWidget(buttonBox);

  setMinimumSize(QSize(min_width, min_height));
  setLayout(mainlayout);
  retranslateUi();
}

void PubSubDialog::startExecute(const proxy::events_info::ExecuteInfoRequest& req) {
  UNUSED(req);
}

void PubSubDialog::finishExecute(const proxy::events_info::ExecuteInfoResponce& res) {
  UNUSED(res);
}

void PubSubDialog::startLoadServerChannels(const proxy::events_info::LoadServerChannelsRequest& req) {
  UNUSED(req);

  channels_model_->clear();
}

void PubSubDialog::finishLoadServerChannels(const proxy::events_info::LoadServerChannelsResponce& res) {
  common::Error err = res.errorInfo();
  if (err) {
    return;
  }

  proxy::events_info::LoadServerChannelsResponce::channels_container_t channels = res.channels;

  for (core::NDbPSChannel channel : channels) {
    channels_model_->insertItem(new ChannelTableItem(channel));
  }
}

void PubSubDialog::searchClicked() {
  QString pattern = search_box_->text();
  if (pattern.isEmpty()) {
    return;
  }

  proxy::events_info::LoadServerChannelsRequest req(this, common::ConvertToString(pattern));
  server_->LoadChannels(req);
}

void PubSubDialog::showContextMenu(const QPoint& point) {
  QModelIndex sel = selectedIndex();
  if (!sel.isValid()) {
    return;
  }

  QPoint menuPoint = channels_table_->CalculateMenuPoint(point);
  QMenu* menu = new QMenu(channels_table_);

  QAction* publishAction = new QAction(translations::trPublish, this);
  VERIFY(connect(publishAction, &QAction::triggered, this, &PubSubDialog::publish));

  QAction* subscribeAction = new QAction(trSubscribeInNewConsole, this);
  VERIFY(connect(subscribeAction, &QAction::triggered, this, &PubSubDialog::subscribeInNewConsole));

  menu->addAction(publishAction);
  menu->addAction(subscribeAction);
  menu->exec(menuPoint);
  delete menu;
}

void PubSubDialog::publish() {
  QModelIndex sel = selectedIndex();
  if (!sel.isValid()) {
    return;
  }

  ChannelTableItem* node = common::qt::item<common::qt::gui::TableItem*, ChannelTableItem*>(sel);
  if (!node) {
    DNOTREACHED();
    return;
  }

  bool ok;
  QString publish_text =
      QInputDialog::getText(this, trPublishToChannel_1S.arg(node->GetName()), trEnterWhatYoWantToSend,
                            QLineEdit::Normal, QString(), &ok, Qt::WindowCloseButtonHint);
  if (ok && !publish_text.isEmpty()) {
    core::translator_t trans = server_->GetTranslator();
    core::command_buffer_t cmd_str;
    common::Error err = trans->PublishCommand(node->GetChannel(), common::ConvertToString(publish_text), &cmd_str);
    if (err) {
      LOG_ERROR(err, common::logging::LOG_LEVEL_ERR, true);
      return;
    }

    proxy::events_info::ExecuteInfoRequest req(this, cmd_str);
    server_->Execute(req);
  }
}

void PubSubDialog::subscribeInNewConsole() {
  QModelIndex sel = selectedIndex();
  if (!sel.isValid()) {
    return;
  }

  ChannelTableItem* node = common::qt::item<common::qt::gui::TableItem*, ChannelTableItem*>(sel);
  if (!node) {
    DNOTREACHED();
    return;
  }

  core::translator_t trans = server_->GetTranslator();
  core::command_buffer_t cmd_str;
  common::Error err = trans->SubscribeCommand(node->GetChannel(), &cmd_str);
  if (err) {
    LOG_ERROR(err, common::logging::LOG_LEVEL_ERR, true);
    return;
  }

  QString text;
  if (common::ConvertFromString(cmd_str, &text)) {
    emit consoleOpenedAndExecute(server_, text);
  }
}

QModelIndex PubSubDialog::selectedIndex() const {
  QModelIndexList indexses = channels_table_->selectionModel()->selectedRows();

  if (indexses.count() != 1) {
    return QModelIndex();
  }

  return proxy_model_->mapToSource(indexses[0]);
}

void PubSubDialog::searchLineChanged(const QString& text) {
  UNUSED(text);
}

void PubSubDialog::changeEvent(QEvent* e) {
  if (e->type() == QEvent::LanguageChange) {
    retranslateUi();
  }
  QDialog::changeEvent(e);
}

void PubSubDialog::retranslateUi() {
  search_button_->setText(translations::trSearch);
}

}  // namespace gui
}  // namespace fastonosql
