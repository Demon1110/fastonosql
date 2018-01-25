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

#include "gui/dialogs/info_server_dialog.h"

#include <QHBoxLayout>
#include <QTextEdit>

#include <common/qt/convert2string.h>

#ifdef BUILD_WITH_REDIS
#include "core/db/redis/server_info.h"
#endif

#ifdef BUILD_WITH_MEMCACHED
#include "core/db/memcached/server_info.h"
#endif

#ifdef BUILD_WITH_SSDB
#include "core/db/ssdb/server_info.h"
#endif

#ifdef BUILD_WITH_LEVELDB
#include "core/db/leveldb/server_info.h"
#endif

#ifdef BUILD_WITH_ROCKSDB
#include "core/db/rocksdb/server_info.h"
#endif

#ifdef BUILD_WITH_UNQLITE
#include "core/db/unqlite/server_info.h"
#endif

#ifdef BUILD_WITH_LMDB
#include "core/db/lmdb/server_info.h"
#endif

#ifdef BUILD_WITH_UPSCALEDB
#include "core/db/upscaledb/server_info.h"
#endif

#ifdef BUILD_WITH_FORESTDB
#include "core/db/forestdb/server_info.h"
#endif

#ifdef BUILD_WITH_PIKA
#include "core/db/pika/server_info.h"
#endif

#include "core/connection_types.h"     // for connectionTypes, etc
#include "proxy/events/events_info.h"  // for ServerInfoResponce, etc
#include "proxy/server/iserver.h"      // for IServer

#include <common/qt/gui/glass_widget.h>  // for GlassWidget

#include "gui/gui_factory.h"  // for GuiFactory

#include "translations/global.h"  // for trLoading

namespace {
#ifdef BUILD_WITH_REDIS
const QString trRedisTextServerTemplate = QObject::tr(
    "<h3>Server:</h3>"
    "Redis version: %1<br/>"
    "Redis git_sha1: %2<br/>"
    "Redis git_dirty: %3<br/>"
    "Redis mode: %4<br/>"
    "Os: %5<br/>"
    "Arch: %6<br/>"
    "Multiplexing Api: %7<br/>"
    "Gcc version: %8<br/>"
    "Process id: %9<br/>"
    "Run id: %10<br/>"
    "Tcp port: %11<br/>"
    "Uptime sec: %12<br/>"
    "Uptime days: %13<br/>"
    "Hz: %14<br/>"
    "Lru clock: %15");

const QString trRedisTextClientsTemplate = QObject::tr(
    "<h3>Clients:</h3>"
    "Connected clients_: %1<br/>"
    "Client longest output list: %2<br/>"
    "Client biggest input buf: %3<br/>"
    "Blocked clients: %4");

const QString trRedisTextMemoryTemplate = QObject::tr(
    "<h3>Memory:</h3>"
    "Used memory: %1<br/>"
    "Used memory human: %2<br/>"
    "Used memory rss: %3<br/>"
    "Used memory peak: %4<br/>"
    "Used memory peak human: %5<br/>"
    "Used memory lua: %6<br/>"
    "Mem fragmentation ratio: %7<br/>"
    "Mem allocator: %8");

const QString trRedisTextPersistenceTemplate = QObject::tr(
    "<h3>Persistence:</h3>"
    "Loading: %1<br/>"
    "Rdb changes since last save: %2<br/>"
    "Rdb bgsave in_progress: %3<br/>"
    "Rdb last save_time: %4<br/>"
    "Rdb last bgsave_status: %5<br/>"
    "Rdb last bgsave time sec: %6<br/>"
    "Rdb current bgsave time sec: %7<br/>"
    "Aof enabled: %8<br/>"
    "Aof rewrite in progress: %9<br/>"
    "Aof rewrite scheduled: %10<br/>"
    "Aof last rewrite time sec: %11<br/>"
    "Aof current rewrite time sec: %12<br/>"
    "Aof last bgrewrite status: %13<br/>"
    "Aof last write status: %14");

const QString trRedisTextStatsTemplate = QObject::tr(
    "<h3>Stats:</h3>"
    "Total connections received: %1<br/>"
    "Total commands processed: %2<br/>"
    "Instantaneous ops per sec: %3<br/>"
    "Rejected connections: %4<br/>"
    "Sync full: %5<br/>"
    "Sync partial ok: %6<br/>"
    "Sync partial err: %7<br/>"
    "Expired keys: %8<br/>"
    "Evicted keys: %9<br/>"
    "Keyspace hits: %10<br/>"
    "Keyspace misses: %11<br/>"
    "Pubsub channels: %12<br/>"
    "Pubsub patterns: %13<br/>"
    "Latest fork usec: %14");

const QString trRedisTextReplicationTemplate = QObject::tr(
    "<h3>Replication:</h3>"
    "Role: %1<br/>"
    "Connected slaves: %2<br/>"
    "Master reply offset: %3<br/>"
    "Backlog active: %4<br/>"
    "Backlog size: %5<br/>"
    "Backlog first byte offset: %6<br/>"
    "Backlog histen: %7");

const QString trRedisTextCpuTemplate = QObject::tr(
    "<h3>Cpu:</h3>"
    "Used cpu sys: %1<br/>"
    "Used cpu user: %2<br/>"
    "Used cpu sys children: %3<br/>"
    "Used cpu user children: %4");
#endif

#ifdef BUILD_WITH_MEMCACHED
const QString trMemcachedTextServerTemplate = QObject::tr(
    "<b>Common:</b><br/>"
    "Pid: %1<br/>"
    "Update time: %2<br/>"
    "Time: %3<br/>"
    "Version: %4<br/>"
    "Pointer size: %5<br/>"
    "Usage user: %6<br/>"
    "Usage system: %7<br/>"
    "Current items: %8<br/>"
    "Total items: %9<br/>"
    "Bytes: %10<br/>"
    "Current connections: %11<br/>"
    "Total connections: %12<br/>"
    "Connection structures: %13<br/>"
    "Cmd get: %14<br/>"
    "Cmd set: %15<br/>"
    "Get hits: %16<br/>"
    "Get misses: %17<br/>"
    "Evictions: %18<br/>"
    "Bytes read: %19<br/>"
    "Bytes written: %20<br/>"
    "Limit max bytes: %21<br/>"
    "Threads: %22");
#endif

#ifdef BUILD_WITH_SSDB
const QString trSsdbTextServerTemplate = QObject::tr(
    "<b>Common:</b><br/>"
    "Version: %1<br/>"
    "Links: %2<br/>"
    "Total calls: %3<br/>"
    "DB size: %4 bytes<br/>"
    "Binlogs: %5");
#endif

#ifdef BUILD_WITH_LEVELDB
const QString trLeveldbTextServerTemplate = QObject::tr(
    "<b>Stats:</b><br/>"
    "Compactions level: %1<br/>"
    "File size mb: %2<br/>"
    "Time sec: %3<br/>"
    "Read mb: %4<br/>"
    "Write mb: %5");
#endif

#ifdef BUILD_WITH_ROCKSDB
const QString trRocksdbTextServerTemplate = QObject::tr(
    "<b>Stats:</b><br/>"
    "Compactions level: %1<br/>"
    "File size mb: %2<br/>"
    "Time sec: %3<br/>"
    "Read mb: %4<br/>"
    "Write mb: %5");
#endif

#ifdef BUILD_WITH_UNQLITE
const QString trUnqliteTextServerTemplate = QObject::tr(
    "<b>Stats:</b><br/>"
    "DB path: %1<br/>"
    "DB size: %2 bytes<br/>");
#endif

#ifdef BUILD_WITH_LMDB
const QString trLmdbTextServerTemplate = QObject::tr(
    "<b>Stats:</b><br/>"
    "DB path: %1<br/>");
#endif

#ifdef BUILD_WITH_UPSCALEDB
const QString trUpscaledbTextServerTemplate = QObject::tr(
    "<b>Stats:</b><br/>"
    "DB path: %1<br/>");
#endif

#ifdef BUILD_WITH_FORESTDB
const QString trForestdbTextServerTemplate = QObject::tr(
    "<b>Stats:</b><br/>"
    "DB path: %1<br/>"
    "DB size: %2 bytes<br/>");
#endif

#ifdef BUILD_WITH_PIKA
const QString trPikaTextServerTemplate = QObject::tr(
    "<h3>Server:</h3>"
    "Pika version: %1<br/>"
    "Pika git_sha: %2<br/>"
    "Pika build compile date: %3<br/>"
    "Os: %4<br/>"
    "Arch bit: %5<br/>"
    "Process id: %6<br/>"
    "Tcp port: %7<br/>"
    "Thread num: %8<br/>"
    "Sync thread num: %9<br/>"
    "Uptime in seconds: %10<br/>"
    "Uptime in days: %11<br/>"
    "Config file: %12<br/>"
    "Server id: %13");

const QString trPikaTextDataTemplate = QObject::tr(
    "<h3>Data:</h3>"
    "DB size: %1<br/>"
    "DB size human: %2<br/>"
    "Compression: %3<br/>"
    "Used memory: %4<br/>"
    "Used memory human: %5<br/>"
    "DB memtable usage: %6<br/>"
    "DB table reader usage: %7");

const QString trPikaTextLogTemplate = QObject::tr(
    "<h3>Data:</h3>"
    "Log size: %1<br/>"
    "Log size human: %2<br/>"
    "Safety purge: %3<br/>"
    "Expire logs days: %4<br/>"
    "Expire logs nums: %5<br/>"
    "Binlog offset: %6");

const QString trPikaTextClientsTemplate = QObject::tr(
    "<h3>Clients:</h3>"
    "Connected clients_: %1");

const QString trPikaTextHubTemplate = QObject::tr("<h3>Hub:</h3>");

const QString trPikaTextStatsTemplate = QObject::tr(
    "<h3>Stats:</h3>"
    "Total connections received: %1<br/>"
    "Instantaneous ops per sec: %2<br/>"
    "Total commands processed: %3<br/>"
    "Is bg saving: %4<br/>"
    "Is slots reloading: %5<br/>"
    "Is slots clenuping: %6<br/>"
    "Is scanning keyspace: %7<br/>"
    "Is compact: %8<br/>"
    "Compact cron: %9<br/>"
    "Compact interval: %10");

const QString trPikaTextCpuTemplate = QObject::tr(
    "<h3>Cpu:</h3>"
    "Used cpu sys: %1<br/>"
    "Used cpu user: %2<br/>"
    "Used cpu sys children: %3<br/>"
    "Used cpu user children: %4");

const QString trPikaTextReplicationTemplate = QObject::tr(
    "<h3>Replication:</h3>"
    "Role: %1<br/>"
    "Connected slaves: %2");

const QString trPikaTextKeyspaceTemplate = QObject::tr(
    "<h3>Keyspace:</h3>"
    "kv keys: %1<br/>"
    "hash keys: %2<br/>"
    "list keys: %3<br/>"
    "zset keys: %4<br/>"
    "set keys: %5");

const QString trPikaTextDoubleMasterTemplate = QObject::tr("<h3>DoubleMaster:</h3>");

#endif

}  // namespace

namespace fastonosql {
namespace gui {

InfoServerDialog::InfoServerDialog(proxy::IServerSPtr server, QWidget* parent) : QDialog(parent), server_(server) {
  CHECK(server_);

  core::connectionTypes type = server->GetType();
  setWindowIcon(GuiFactory::GetInstance().GetIcon(type));
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);  // Remove help
                                                                     // button (?)

  server_text_info_ = new QTextEdit;
  server_text_info_->setReadOnly(true);
  QHBoxLayout* mainL = new QHBoxLayout;
  mainL->setContentsMargins(0, 0, 0, 0);
  mainL->addWidget(server_text_info_);

  setMinimumSize(QSize(min_width, min_height));
  setLayout(mainL);

  glass_widget_ = new common::qt::gui::GlassWidget(GuiFactory::GetInstance().GetPathToLoadingGif(),
                                                   translations::trLoading, 0.5, QColor(111, 111, 100), this);
#ifdef BUILD_WITH_REDIS
  if (type == core::REDIS) {
    updateText(core::redis::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_MEMCACHED
  if (type == core::MEMCACHED) {
    updateText(core::memcached::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_SSDB
  if (type == core::SSDB) {
    updateText(core::ssdb::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_LEVELDB
  if (type == core::LEVELDB) {
    updateText(core::leveldb::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_ROCKSDB
  if (type == core::ROCKSDB) {
    updateText(core::rocksdb::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_UNQLITE
  if (type == core::UNQLITE) {
    updateText(core::unqlite::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_LMDB
  if (type == core::LMDB) {
    updateText(core::lmdb::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_UPSCALEDB
  if (type == core::UPSCALEDB) {
    updateText(core::upscaledb::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_FORESTDB
  if (type == core::FORESTDB) {
    updateText(core::forestdb::ServerInfo());
  }
#endif
#ifdef BUILD_WITH_PIKA
  if (type == core::PIKA) {
    updateText(core::pika::ServerInfo());
  }
#endif

  VERIFY(connect(server.get(), &proxy::IServer::LoadServerInfoStarted, this, &InfoServerDialog::startServerInfo));
  VERIFY(connect(server.get(), &proxy::IServer::LoadServerInfoFinished, this, &InfoServerDialog::finishServerInfo));
  retranslateUi();
}

void InfoServerDialog::startServerInfo(const proxy::events_info::ServerInfoRequest& req) {
  UNUSED(req);

  glass_widget_->start();
}

void InfoServerDialog::finishServerInfo(const proxy::events_info::ServerInfoResponce& res) {
  glass_widget_->stop();
  common::Error err = res.errorInfo();
  if (err) {
    return;
  }

  core::IServerInfoSPtr inf = res.info();
  if (!inf) {
    return;
  }

  core::connectionTypes type = server_->GetType();
  CHECK(type == inf->GetType());
#ifdef BUILD_WITH_REDIS
  if (type == core::REDIS) {
    core::redis::ServerInfo* infr = static_cast<core::redis::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_MEMCACHED
  if (type == core::MEMCACHED) {
    core::memcached::ServerInfo* infr = static_cast<core::memcached::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_SSDB
  if (type == core::SSDB) {
    core::ssdb::ServerInfo* infr = static_cast<core::ssdb::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_LEVELDB
  if (type == core::LEVELDB) {
    core::leveldb::ServerInfo* infr = static_cast<core::leveldb::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_ROCKSDB
  if (type == core::ROCKSDB) {
    core::rocksdb::ServerInfo* infr = static_cast<core::rocksdb::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_UNQLITE
  if (type == core::UNQLITE) {
    core::unqlite::ServerInfo* infr = static_cast<core::unqlite::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_LMDB
  if (type == core::LMDB) {
    core::lmdb::ServerInfo* infr = static_cast<core::lmdb::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_UPSCALEDB
  if (type == core::UPSCALEDB) {
    core::upscaledb::ServerInfo* infr = static_cast<core::upscaledb::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_FORESTDB
  if (type == core::FORESTDB) {
    core::forestdb::ServerInfo* infr = static_cast<core::forestdb::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
#ifdef BUILD_WITH_PIKA
  if (type == core::PIKA) {
    core::pika::ServerInfo* infr = static_cast<core::pika::ServerInfo*>(inf.get());
    CHECK(infr);
    updateText(*infr);
  }
#endif
}

void InfoServerDialog::showEvent(QShowEvent* e) {
  QDialog::showEvent(e);
  proxy::events_info::ServerInfoRequest req(this);
  server_->LoadServerInfo(req);
}

void InfoServerDialog::changeEvent(QEvent* e) {
  if (e->type() == QEvent::LanguageChange) {
    retranslateUi();
  }
  QDialog::changeEvent(e);
}

void InfoServerDialog::retranslateUi() {
  QString qname;
  if (common::ConvertFromString(server_->GetName(), &qname)) {
    setWindowTitle(tr("%1 info").arg(qname));
  }
}

#ifdef BUILD_WITH_REDIS
void InfoServerDialog::updateText(const core::redis::ServerInfo& serv) {
  core::redis::ServerInfo::Server ser = serv.server_;
  QString qredis_version;
  common::ConvertFromString(ser.redis_version_, &qredis_version);

  QString qredis_git_sha1;
  common::ConvertFromString(ser.redis_git_sha1_, &qredis_git_sha1);

  QString qredis_git_dirty;
  common::ConvertFromString(ser.redis_git_dirty_, &qredis_git_dirty);

  QString qredis_mode;
  common::ConvertFromString(ser.redis_mode_, &qredis_mode);

  QString qos;
  common::ConvertFromString(ser.os_, &qos);

  QString qmultiplexing_api;
  common::ConvertFromString(ser.multiplexing_api_, &qmultiplexing_api);

  QString qgcc_version;
  common::ConvertFromString(ser.gcc_version_, &qgcc_version);

  QString qrun_id;
  common::ConvertFromString(ser.run_id_, &qrun_id);

  QString textServ = trRedisTextServerTemplate.arg(qredis_version)
                         .arg(qredis_git_sha1)
                         .arg(qredis_git_dirty)
                         .arg(qredis_mode)
                         .arg(qos)
                         .arg(ser.arch_bits_)
                         .arg(qmultiplexing_api)
                         .arg(qgcc_version)
                         .arg(ser.process_id_)
                         .arg(qrun_id)
                         .arg(ser.tcp_port_)
                         .arg(ser.uptime_in_seconds_)
                         .arg(ser.uptime_in_days_)
                         .arg(ser.hz_)
                         .arg(ser.lru_clock_);

  core::redis::ServerInfo::Clients cl = serv.clients_;
  QString textCl = trRedisTextClientsTemplate.arg(cl.connected_clients_)
                       .arg(cl.client_longest_output_list_)
                       .arg(cl.client_biggest_input_buf_)
                       .arg(cl.blocked_clients_);

  core::redis::ServerInfo::Memory mem = serv.memory_;
  QString qused_memory_human;
  common::ConvertFromString(mem.used_memory_human_, &qused_memory_human);

  QString qused_memory_peak_human;
  common::ConvertFromString(mem.used_memory_peak_human_, &qused_memory_peak_human);

  QString qmem_allocator;
  common::ConvertFromString(mem.mem_allocator_, &qmem_allocator);

  QString textMem = trRedisTextMemoryTemplate.arg(mem.used_memory_)
                        .arg(qused_memory_human)
                        .arg(mem.used_memory_rss_)
                        .arg(mem.used_memory_peak_)
                        .arg(qused_memory_peak_human)
                        .arg(mem.used_memory_lua_)
                        .arg(mem.mem_fragmentation_ratio_)
                        .arg(qmem_allocator);

  core::redis::ServerInfo::Persistence per = serv.persistence_;
  QString qrdb_last_bgsave_status;
  common::ConvertFromString(per.rdb_last_bgsave_status_, &qrdb_last_bgsave_status);

  QString qaof_last_bgrewrite_status;
  common::ConvertFromString(per.aof_last_bgrewrite_status_, &qaof_last_bgrewrite_status);

  QString qaof_last_write_status;
  common::ConvertFromString(per.aof_last_write_status_, &qaof_last_write_status);

  QString textPer = trRedisTextPersistenceTemplate.arg(per.loading_)
                        .arg(per.rdb_changes_since_last_save_)
                        .arg(per.rdb_bgsave_in_progress_)
                        .arg(per.rdb_last_save_time_)
                        .arg(qrdb_last_bgsave_status)
                        .arg(per.rdb_last_bgsave_time_sec_)
                        .arg(per.rdb_current_bgsave_time_sec_)
                        .arg(per.aof_enabled_)
                        .arg(per.aof_rewrite_in_progress_)
                        .arg(per.aof_rewrite_scheduled_)
                        .arg(per.aof_last_rewrite_time_sec_)
                        .arg(per.aof_current_rewrite_time_sec_)
                        .arg(qaof_last_bgrewrite_status)
                        .arg(qaof_last_write_status);

  core::redis::ServerInfo::Stats stat = serv.stats_;
  QString textStat = trRedisTextStatsTemplate.arg(stat.total_connections_received_)
                         .arg(stat.total_commands_processed_)
                         .arg(stat.instantaneous_ops_per_sec_)
                         .arg(stat.rejected_connections_)
                         .arg(stat.sync_full_)
                         .arg(stat.sync_partial_ok_)
                         .arg(stat.sync_partial_err_)
                         .arg(stat.expired_keys_)
                         .arg(stat.evicted_keys_)
                         .arg(stat.keyspace_hits_)
                         .arg(stat.keyspace_misses_)
                         .arg(stat.pubsub_channels_)
                         .arg(stat.pubsub_patterns_)
                         .arg(stat.latest_fork_usec_);

  core::redis::ServerInfo::Replication repl = serv.replication_;
  QString qrole;
  common::ConvertFromString(repl.role_, &qrole);

  QString textRepl = trRedisTextReplicationTemplate.arg(qrole)
                         .arg(repl.connected_slaves_)
                         .arg(repl.master_repl_offset_)
                         .arg(repl.backlog_active_)
                         .arg(repl.backlog_size_)
                         .arg(repl.backlog_first_byte_offset_)
                         .arg(repl.backlog_histen_);

  core::redis::ServerInfo::Cpu cpu = serv.cpu_;
  QString textCpu = trRedisTextCpuTemplate.arg(cpu.used_cpu_sys_)
                        .arg(cpu.used_cpu_user_)
                        .arg(cpu.used_cpu_sys_children_)
                        .arg(cpu.used_cpu_user_children_);
  server_text_info_->setText(textServ + textMem + textCpu + textCl + textPer + textStat + textRepl);
}
#endif

#ifdef BUILD_WITH_MEMCACHED
void InfoServerDialog::updateText(const core::memcached::ServerInfo& serv) {
  core::memcached::ServerInfo::Stats com = serv.stats_;
  QString qverson;
  common::ConvertFromString(com.version, &qverson);

  QString textServ = trMemcachedTextServerTemplate.arg(com.pid)
                         .arg(com.uptime)
                         .arg(com.time)
                         .arg(qverson)
                         .arg(com.pointer_size)
                         .arg(com.rusage_user)
                         .arg(com.rusage_system)
                         .arg(com.curr_items)
                         .arg(com.total_items)
                         .arg(com.bytes)
                         .arg(com.curr_connections)
                         .arg(com.total_connections)
                         .arg(com.connection_structures)
                         .arg(com.cmd_get)
                         .arg(com.cmd_set)
                         .arg(com.get_hits)
                         .arg(com.get_misses)
                         .arg(com.evictions)
                         .arg(com.bytes_read)
                         .arg(com.bytes_written)
                         .arg(com.limit_maxbytes)
                         .arg(com.threads);

  server_text_info_->setText(textServ);
}
#endif

#ifdef BUILD_WITH_SSDB
void InfoServerDialog::updateText(const core::ssdb::ServerInfo& serv) {
  core::ssdb::ServerInfo::Stats com = serv.stats_;
  QString qverson;
  common::ConvertFromString(com.version, &qverson);
  QString qbinlogs;
  common::ConvertFromString(com.binlogs, &qbinlogs);

  QString textServ =
      trSsdbTextServerTemplate.arg(qverson).arg(com.links).arg(com.total_calls).arg(com.dbsize).arg(qbinlogs);

  server_text_info_->setText(textServ);
}
#endif

#ifdef BUILD_WITH_LEVELDB
void InfoServerDialog::updateText(const core::leveldb::ServerInfo& serv) {
  core::leveldb::ServerInfo::Stats stats = serv.stats_;
  QString textServ = trLeveldbTextServerTemplate.arg(stats.compactions_level)
                         .arg(stats.file_size_mb)
                         .arg(stats.time_sec)
                         .arg(stats.read_mb)
                         .arg(stats.write_mb);

  server_text_info_->setText(textServ);
}
#endif
#ifdef BUILD_WITH_ROCKSDB
void InfoServerDialog::updateText(const core::rocksdb::ServerInfo& serv) {
  core::rocksdb::ServerInfo::Stats stats = serv.stats_;
  QString textServ = trRocksdbTextServerTemplate.arg(stats.compactions_level)
                         .arg(stats.file_size_mb)
                         .arg(stats.time_sec)
                         .arg(stats.read_mb)
                         .arg(stats.write_mb);

  server_text_info_->setText(textServ);
}
#endif
#ifdef BUILD_WITH_UNQLITE
void InfoServerDialog::updateText(const core::unqlite::ServerInfo& serv) {
  core::unqlite::ServerInfo::Stats stats = serv.stats_;
  QString qfile_name;
  common::ConvertFromString(stats.db_path, &qfile_name);

  QString textServ = trUnqliteTextServerTemplate.arg(qfile_name).arg(stats.db_size);
  server_text_info_->setText(textServ);
}
#endif
#ifdef BUILD_WITH_LMDB
void InfoServerDialog::updateText(const core::lmdb::ServerInfo& serv) {
  core::lmdb::ServerInfo::Stats stats = serv.stats_;
  QString qdb_path;
  common::ConvertFromString(stats.db_path, &qdb_path);

  QString textServ = trLmdbTextServerTemplate.arg(qdb_path);
  server_text_info_->setText(textServ);
}
#endif
#ifdef BUILD_WITH_UPSCALEDB
void InfoServerDialog::updateText(const core::upscaledb::ServerInfo& serv) {
  core::upscaledb::ServerInfo::Stats stats = serv.stats_;
  QString qdb_path;
  common::ConvertFromString(stats.db_path, &qdb_path);

  QString textServ = trUpscaledbTextServerTemplate.arg(qdb_path);
  server_text_info_->setText(textServ);
}
#endif
#ifdef BUILD_WITH_FORESTDB
void InfoServerDialog::updateText(const core::forestdb::ServerInfo& serv) {
  core::forestdb::ServerInfo::Stats stats = serv.stats_;
  QString qdb_path;
  common::ConvertFromString(stats.db_path, &qdb_path);

  QString textServ = trForestdbTextServerTemplate.arg(qdb_path).arg(stats.db_size);
  server_text_info_->setText(textServ);
}
#endif

#ifdef BUILD_WITH_PIKA
void InfoServerDialog::updateText(const core::pika::ServerInfo& serv) {
  // Server
  core::pika::ServerInfo::Server ser = serv.server_;
  QString qpika_version;
  common::ConvertFromString(ser.pika_version_, &qpika_version);

  QString qpika_git_sha;
  common::ConvertFromString(ser.pika_git_sha_, &qpika_git_sha);

  QString qpika_build_compile_date;
  common::ConvertFromString(ser.pika_build_compile_date_, &qpika_build_compile_date);

  QString qos;
  common::ConvertFromString(ser.os_, &qos);

  QString qconfig_file;
  common::ConvertFromString(ser.config_file_, &qconfig_file);

  QString textServ = trPikaTextServerTemplate.arg(qpika_version)
                         .arg(qpika_git_sha)
                         .arg(qpika_build_compile_date)
                         .arg(qos)
                         .arg(ser.arch_bits_)
                         .arg(ser.process_id_)
                         .arg(ser.tcp_port_)
                         .arg(ser.thread_num_)
                         .arg(ser.sync_thread_num_)
                         .arg(ser.uptime_in_seconds_)
                         .arg(ser.uptime_in_days_)
                         .arg(qconfig_file)
                         .arg(ser.server_id_);

  // Data
  core::pika::ServerInfo::Data data = serv.data_;
  QString qdb_size_human;
  common::ConvertFromString(data.db_size_human_, &qdb_size_human);
  QString qcompression;
  common::ConvertFromString(data.compression_, &qcompression);
  QString qused_memory_human;
  common::ConvertFromString(data.used_memory_human_, &qused_memory_human);

  QString textData = trPikaTextDataTemplate.arg(data.db_size_)
                         .arg(qdb_size_human)
                         .arg(qcompression)
                         .arg(data.used_memory_)
                         .arg(qused_memory_human)
                         .arg(data.db_memtable_usage_)
                         .arg(data.db_tablereader_usage_);

  // Log
  core::pika::ServerInfo::Log log = serv.log_;
  QString qlog_size_human;
  common::ConvertFromString(log.log_size_human_, &qlog_size_human);
  QString qsafety_purge;
  common::ConvertFromString(log.safety_purge_, &qsafety_purge);
  QString qbinlog_offset;
  common::ConvertFromString(log.binlog_offset_, &qbinlog_offset);

  QString textLog = trPikaTextLogTemplate.arg(log.log_size_)
                        .arg(qlog_size_human)
                        .arg(qsafety_purge)
                        .arg(log.expire_logs_days_)
                        .arg(log.expire_logs_nums_)
                        .arg(qbinlog_offset);

  // Clients
  core::pika::ServerInfo::Clients clients = serv.clients_;

  QString textClients = trPikaTextClientsTemplate.arg(clients.connected_clients_);

  // Hub
  core::pika::ServerInfo::Hub hub = serv.hub_;
  UNUSED(hub);
  QString textHub = trPikaTextHubTemplate;

  // Stats
  core::pika::ServerInfo::Stats stats = serv.stats_;
  QString qis_bgsaving;
  common::ConvertFromString(stats.is_bgsaving_, &qis_bgsaving);
  QString qis_slots_reloading;
  common::ConvertFromString(stats.is_slots_reloading_, &qis_slots_reloading);
  QString qis_slots_cleanuping;
  common::ConvertFromString(stats.is_slots_cleanuping_, &qis_slots_cleanuping);
  QString qis_scaning_keyspace;
  common::ConvertFromString(stats.is_scaning_keyspace_, &qis_scaning_keyspace);
  QString qis_compact;
  common::ConvertFromString(stats.is_compact_, &qis_compact);
  QString qcompact_cron;
  common::ConvertFromString(stats.compact_cron_, &qcompact_cron);
  QString qcompact_interval;
  common::ConvertFromString(stats.compact_interval_, &qcompact_interval);

  QString textStats = trPikaTextStatsTemplate.arg(stats.total_connections_received_)
                          .arg(stats.instantaneous_ops_per_sec_)
                          .arg(stats.total_commands_processed_)
                          .arg(qis_bgsaving)
                          .arg(qis_slots_reloading)
                          .arg(qis_slots_cleanuping)
                          .arg(qis_scaning_keyspace)
                          .arg(qis_compact)
                          .arg(qcompact_cron)
                          .arg(qcompact_interval);

  // CPU
  core::pika::ServerInfo::Cpu cpu = serv.cpu_;

  QString textCpu = trPikaTextCpuTemplate.arg(cpu.used_cpu_sys_)
                        .arg(cpu.used_cpu_user_)
                        .arg(cpu.used_cpu_sys_children_)
                        .arg(cpu.used_cpu_user_children_);

  // Replication
  core::pika::ServerInfo::Replication repl = serv.replication_;
  QString qrole;
  common::ConvertFromString(repl.role_, &qrole);

  QString textRepl = trPikaTextReplicationTemplate.arg(qrole).arg(repl.connected_slaves_);

  // Keyspace
  core::pika::ServerInfo::KeySpace key_space = serv.key_space_;
  QString textKeyspace = trPikaTextKeyspaceTemplate.arg(key_space.kv_)
                             .arg(key_space.hash_)
                             .arg(key_space.list_)
                             .arg(key_space.zset_)
                             .arg(key_space.set_);

  // DoubleMaster
  core::pika::ServerInfo::DoubleMaster doub_master = serv.double_master_;
  UNUSED(doub_master);
  QString textDoubleMaster = trPikaTextDoubleMasterTemplate;

  server_text_info_->setText(textServ + textData + textLog + textClients + textHub + textStats + textCpu + textRepl +
                             textKeyspace + textDoubleMaster);
}
#endif

}  // namespace gui
}  // namespace fastonosql
