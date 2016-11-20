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

#include <common/error.h>

#include "core/command_info.h"
#include "core/internal/command_handler.h"

namespace fastonosql {
namespace core {
namespace ssdb {

common::Error select(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error set(internal::CommandHandler* handler, int argc, const char** argv, FastoObject* out);
common::Error get(internal::CommandHandler* handler, int argc, const char** argv, FastoObject* out);
common::Error del(internal::CommandHandler* handler, int argc, const char** argv, FastoObject* out);
common::Error rename(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error set_ttl(internal::CommandHandler* handler,
                      int argc,
                      const char** argv,
                      FastoObject* out);

common::Error info(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error auth(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error setx(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error incr(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error keys(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error scan(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error rscan(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error multi_get(internal::CommandHandler* handler,
                        int argc,
                        const char** argv,
                        FastoObject* out);
common::Error multi_set(internal::CommandHandler* handler,
                        int argc,
                        const char** argv,
                        FastoObject* out);
common::Error multi_del(internal::CommandHandler* handler,
                        int argc,
                        const char** argv,
                        FastoObject* out);
common::Error hget(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error hgetall(internal::CommandHandler* handler,
                      int argc,
                      const char** argv,
                      FastoObject* out);
common::Error hset(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error hdel(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error hincr(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error hsize(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error hclear(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error hkeys(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error hscan(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error hrscan(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error multi_hget(internal::CommandHandler* handler,
                         int argc,
                         const char** argv,
                         FastoObject* out);
common::Error multi_hset(internal::CommandHandler* handler,
                         int argc,
                         const char** argv,
                         FastoObject* out);
common::Error zget(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error zset(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error zdel(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error zincr(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error zsize(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error zclear(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error zrank(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error zrrank(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error zrange(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error zrrange(internal::CommandHandler* handler,
                      int argc,
                      const char** argv,
                      FastoObject* out);
common::Error zkeys(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error zscan(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error zrscan(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error multi_zget(internal::CommandHandler* handler,
                         int argc,
                         const char** argv,
                         FastoObject* out);
common::Error multi_zset(internal::CommandHandler* handler,
                         int argc,
                         const char** argv,
                         FastoObject* out);
common::Error multi_zdel(internal::CommandHandler* handler,
                         int argc,
                         const char** argv,
                         FastoObject* out);
common::Error qpush(internal::CommandHandler* handler,
                    int argc,
                    const char** argv,
                    FastoObject* out);
common::Error qpop(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error qslice(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error qclear(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);
common::Error dbsize(internal::CommandHandler* handler,
                     int argc,
                     const char** argv,
                     FastoObject* out);

common::Error dbkcount(internal::CommandHandler* handler,
                       int argc,
                       const char** argv,
                       FastoObject* out);
common::Error help(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);
common::Error flushdb(internal::CommandHandler* handler,
                      int argc,
                      const char** argv,
                      FastoObject* out);
common::Error quit(internal::CommandHandler* handler,
                   int argc,
                   const char** argv,
                   FastoObject* out);

// TODO: SETNX command imlementation
static const std::vector<CommandHolder> ssdbCommands = {
    CommandHolder("AUTH",
                  "<password>",
                  "Authenticate to the server",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &auth),
    CommandHolder("SET",
                  "<key> <value>",
                  "Set the value of the key.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &set),
    CommandHolder("GET",
                  "<key>",
                  "Get the value related to the specified key.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &get),
    CommandHolder("RENAME",
                  "<key> <newkey>",
                  "Rename a key",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &rename),
    CommandHolder("SETX",
                  "<key> <value> <ttl>",
                  "Set the value of the key, with a time to live.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &setx),
    CommandHolder("DEL",
                  "<key> [key ...]",
                  "Delete key.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  INFINITE_COMMAND_ARGS,
                  &del),
    CommandHolder("INCR",
                  "<key> [num]",
                  "Increment the number stored at key by num.\n"
                  "The num argument could be a negative integer.\n"
                  "The old number is first converted to an integer "
                  "before "
                  "increment, assuming it "
                  "was stored as literal integer.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  1,
                  &incr),
    CommandHolder("KEYS",
                  "<key_start> <key_end> <limit>",
                  "List keys in range (key_start, key_end].",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &keys),
    CommandHolder("SCAN",
                  "<key_start> <key_end> <limit>",
                  "List keys in range (key_start, key_end].",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &scan),
    CommandHolder("RSCAN",
                  "<key_start> <key_end> <limit>",
                  "List keys in range (key_start, key_end].",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &rscan),
    CommandHolder("MULTI_GET",
                  "<keys> [keys ...]",
                  "Get the values related to the specified "
                  "multiple keys",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &multi_get),
    CommandHolder("MULTI_SET",
                  "<key> <value> [key value ...]",
                  "Set multiple key-value pairs(kvs) in "
                  "one method call.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  INFINITE_COMMAND_ARGS,
                  &multi_set),
    CommandHolder("MULTI_DEL",
                  "<keys> [keys ...]",
                  "Delete specified multiple keys.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  INFINITE_COMMAND_ARGS,
                  &multi_del),
    CommandHolder("HSET",
                  "<name> <key> <value>",
                  "Set the string value in argument as "
                  "value of the key of a hashmap.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &hset),
    CommandHolder("HGET",
                  "<name> <key>",
                  "Get the value related to the specified "
                  "key of a hashmap",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &hget),
    CommandHolder("HDEL",
                  "<name> <key>",
                  "Delete specified key of a hashmap.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &hdel),
    CommandHolder("HINCR",
                  "<name> <key> <num>",
                  "Increment the number stored at key in a hashmap "
                  "by num.\n"
                  "The num argument could be a negative integer.\n"
                  "The old number is first converted to an integer "
                  "before "
                  "increment, assuming it "
                  "was stored as literal integer.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &hincr),
    CommandHolder("HSIZE",
                  "<name>",
                  "Return the number of pairs of a hashmap.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &hsize),
    CommandHolder("HCLEAR",
                  "<name>",
                  "Delete all keys in a hashmap.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &hclear),
    CommandHolder("HKEYS",
                  "<name> <key_start> <key_end> <limit>",
                  "List keys of a hashmap in range "
                  "(key_start, key_end].",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  4,
                  0,
                  &hkeys),
    CommandHolder("HGETALL",
                  "<name>",
                  "Returns the whole hash, as an array of "
                  "strings indexed by strings.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &hgetall),
    CommandHolder("HSCAN",
                  "<name> <key_start> <key_end> <limit>",
                  "List key-value pairs of a hashmap with "
                  "keys in range "
                  "(key_start, key_end].",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  4,
                  0,
                  &hscan),
    CommandHolder("HRSCAN",
                  "<name> <key_start> <key_end> <limit>",
                  "List key-value pairs of a hashmap with "
                  "keys in range "
                  "(key_start, key_end], in "
                  "reverse order.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  4,
                  0,
                  &hrscan),
    CommandHolder("MULTI_HGET",
                  "<name> <keys>",
                  "Get the values related to the specified "
                  "multiple keys of a hashmap.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &multi_hget),
    CommandHolder("MULTI_HSET",
                  "<name> <key> <value> [key value ...]",
                  "Set multiple key-value pairs(kvs) of a "
                  "hashmap in one method call.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  INFINITE_COMMAND_ARGS,
                  &multi_hset),
    CommandHolder("ZSET",
                  "<name> <key> <score>",
                  "Set the score of the key of a zset.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  4,
                  0,
                  &zset),
    CommandHolder("ZGET",
                  "<name> <key>",
                  "Get the score related to the specified "
                  "key of a zset",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &zget),
    CommandHolder("ZDEL",
                  "<name> <key>",
                  "Delete specified key of a zset.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &zdel),
    CommandHolder("ZINCR",
                  "<name> <key> <num>",
                  "Increment the number stored at key in a zset by "
                  "num.\n"
                  "The num argument could be a negative integer.\n"
                  "The old number is first converted to an integer "
                  "before "
                  "increment, assuming it "
                  "was stored as literal integer.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &zincr),
    CommandHolder("ZSIZE",
                  "<name>",
                  "Return the number of pairs of a zset.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &zsize),
    CommandHolder("ZCLEAR",
                  "<name>",
                  "Delete all keys in a zset.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &zclear),
    CommandHolder("ZRANK",
                  "<name> <key>",
                  "Returns the rank(index) of a given key in the "
                  "specified "
                  "sorted set, starting at "
                  "0 for the item with the smallest score.\n"
                  "zrrank starts at 0 for the item with the largest "
                  "score.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &zrank),
    CommandHolder("ZRRANK",
                  "<name> <key>",
                  "Returns the rank(index) of a given key in the "
                  "specified "
                  "sorted set, starting at "
                  "0 for the item with the smallest score.\n"
                  "zrrank starts at 0 for the item with the largest "
                  "score.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &zrrank),
    CommandHolder("ZRANGE",
                  "<name> <offset> <limit>",
                  "Returns a range of key-score pairs by "
                  "index range [offset, "
                  "offset + limit). "
                  "Zrrange iterates in reverse order.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &zrange),
    CommandHolder("ZRRANGE",
                  "<name> <offset> <limit>",
                  "Returns a range of key-score pairs by "
                  "index range [offset, "
                  "offset + limit). "
                  "Zrrange iterates in reverse order.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &zrrange),
    CommandHolder("ZKEYS",
                  "<name> <key_start> <score_start> "
                  "<score_end> <limit>",
                  "List keys in a zset.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  4,
                  0,
                  &zkeys),
    CommandHolder("ZSCAN",
                  "<name> <key_start> <score_start> "
                  "<score_end> <limit>",
                  "List key-score pairs in a zset, where "
                  "key-score in range "
                  "(key_start+score_start, "
                  "score_end].\n"
                  "If key_start is empty, keys with a "
                  "score greater than or equal to "
                  "score_start will be "
                  "returned.\n"
                  "If key_start is not empty, keys with "
                  "score larger than score_start, "
                  "and keys larger than key_start also "
                  "with score equal to score_start "
                  "will be returned.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  5,
                  0,
                  &zscan),
    CommandHolder("ZRSCAN",
                  "<name> <key_start> <score_start> <score_end> "
                  "<limit>",
                  "List key-score pairs of a zset, in reverse order.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  5,
                  0,
                  &zrscan),
    CommandHolder("MULTI_ZGET",
                  "<name> <keys>",
                  "Get the values related to the specified "
                  "multiple keys of a zset.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &multi_zget),
    CommandHolder("MULTI_ZSET",
                  "<name> <key> <score> [key score ...]",
                  "Set multiple key-score pairs(kvs) of a "
                  "zset in one method call.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  INFINITE_COMMAND_ARGS,
                  &multi_zset),
    CommandHolder("MULTI_ZDEL",
                  "<name> <keys>",
                  "Delete specified multiple keys of a zset.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &multi_zdel),
    CommandHolder("INFO",
                  "[opt]",
                  "Return information about the server.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  0,
                  1,
                  &info),
    CommandHolder("QPUSH",
                  "<name> <item>",
                  "Adds an or more than one element to the "
                  "end of the queue.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &qpush),
    CommandHolder("QPOP",
                  "<name> <size>",
                  "Pop out one or more elements from the "
                  "head of a queue.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  2,
                  0,
                  &qpop),
    CommandHolder("QSLICE",
                  "<name> <begin> <end>",
                  "Returns a portion of elements from the "
                  "queue at the "
                  "specified range [begin, "
                  "end]. begin and end could be negative.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  3,
                  0,
                  &qslice),
    CommandHolder("QCLEAR",
                  "<name>",
                  "Clear the queue.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  1,
                  0,
                  &qclear),
    CommandHolder("DBSIZE",
                  "-",
                  "Return the approximate size of the database, in "
                  "bytes. If "
                  "compression is "
                  "enabled, the size will be of the compressed data.",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  0,
                  0,
                  &dbsize),
    CommandHolder("QUIT",
                  "-",
                  "Close the connection",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  0,
                  0,
                  &quit),
    CommandHolder("DBKCOUNT",
                  "-",
                  "Return the number of keys in the "
                  "selected database",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  0,
                  0,
                  &dbkcount),
    CommandHolder("HELP",
                  "[command]",
                  "Return how to use command",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  0,
                  1,
                  &help),
    CommandHolder("FLUSHDB",
                  "-",
                  "Remove all keys from the current database",
                  UNDEFINED_SINCE,
                  UNDEFINED_EXAMPLE_STR,
                  0,
                  1,
                  &flushdb)};

}  // namespace ssdb
}  // namespace core
}  // namespace fastonosql
