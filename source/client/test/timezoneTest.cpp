/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>
#include <iostream>
#include "taoserror.h"
#include "tglobal.h"
#include "thash.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wsign-compare"

#include "executor.h"
#include "taos.h"


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}

TAOS* getConnWithGlobalOption(const char *tz){
  int code = taos_options(TSDB_OPTION_TIMEZONE, tz);
  ASSERT(code ==  0);
  TAOS* pConn = taos_connect("localhost", "root", "taosdata", NULL, 0);
  ASSERT(pConn != nullptr);
  return pConn;
}

TAOS* getConnWithOption(const char *tz){
  TAOS* pConn = taos_connect("localhost", "root", "taosdata", NULL, 0);
  ASSERT(pConn != nullptr);
  if (tz != NULL){
    int code = taos_options_connection(pConn, TSDB_OPTION_CONNECTION_TIMEZONE, tz);
    ASSERT(code == 0);
  }
  return pConn;
}

void execQuery(TAOS* pConn, const char *sql){
  TAOS_RES* pRes = taos_query(pConn, sql);
  ASSERT(taos_errno(pRes) == TSDB_CODE_SUCCESS);
  taos_free_result(pRes);
}

void execQueryFail(TAOS* pConn, const char *sql){
  TAOS_RES* pRes = taos_query(pConn, sql);
  ASSERT(taos_errno(pRes) != TSDB_CODE_SUCCESS);
  taos_free_result(pRes);
}

void checkRows(TAOS* pConn, const char *sql, int32_t expectedRows){
  TAOS_RES* pRes = taos_query(pConn, sql);
  ASSERT(taos_errno(pRes) == TSDB_CODE_SUCCESS);
  TAOS_ROW    pRow = NULL;
  int rows =  0;
  while ((pRow = taos_fetch_row(pRes)) != NULL) {
    rows++;
  }
  ASSERT(rows == expectedRows);
  taos_free_result(pRes);
}

void check_timezone(TAOS* pConn, const char *sql, const char* tz){
  TAOS_RES *pRes = taos_query(pConn, sql);
  ASSERT(taos_errno(pRes) == 0);
  TAOS_ROW row = NULL;
  while ((row = taos_fetch_row(pRes)) != NULL) {
    if (strcmp((const char*)row[0], "timezone") == 0){
      ASSERT(strstr((const char*)row[1], tz) != NULL);
    }
  }
  taos_free_result(pRes);
}

void check_sql_result_partial(TAOS* pConn, const char *sql, const char* result){
  TAOS_RES *pRes = taos_query(pConn, sql);
  ASSERT(taos_errno(pRes) == 0);
  TAOS_ROW row = NULL;
  while ((row = taos_fetch_row(pRes)) != NULL) {
    ASSERT(strstr((const char*)row[0], result) != NULL);
  }
  taos_free_result(pRes);
}

int64_t get_sql_result(TAOS* pConn, const char *sql){
  int64_t ts = 0;
  TAOS_RES *pRes = taos_query(pConn, sql);
  ASSERT(taos_errno(pRes) == 0);
  TAOS_ROW row = NULL;
  while ((row = taos_fetch_row(pRes)) != NULL) {
    ts = *(int64_t*)row[0];
  }
  taos_free_result(pRes);
  return ts;
}

void check_sql_result(TAOS* pConn, const char *sql, const char* result){
  TAOS_RES *pRes = taos_query(pConn, sql);
  ASSERT(taos_errno(pRes) == 0);
  TAOS_ROW row = NULL;
  while ((row = taos_fetch_row(pRes)) != NULL) {
    ASSERT (strcmp((const char*)row[0], result) == 0);
  }
  taos_free_result(pRes);
}

void check_sql_result_integer(TAOS* pConn, const char *sql, int64_t result){
  TAOS_RES *pRes = taos_query(pConn, sql);
  ASSERT(taos_errno(pRes) == 0);
  TAOS_ROW row = NULL;
  while ((row = taos_fetch_row(pRes)) != NULL) {
    ASSERT (*(int64_t*)row[0] == result);
  }
  taos_free_result(pRes);
}

void check_set_timezone(TAOS* optionFunc(const char *tz)){
  {
    TAOS* pConn = optionFunc("UTC-8");
    check_timezone(pConn, "show local variables", "UTC-8");

    execQuery(pConn, "drop database if exists db1");
    execQuery(pConn, "create database db1");
    execQuery(pConn, "create table db1.t1 (ts timestamp, v int)");

    execQuery(pConn, "insert into db1.t1 values('2023-09-16 17:00:00', 1)");
    checkRows(pConn, "select * from db1.t1 where ts == '2023-09-16 17:00:00'", 1);

    taos_close(pConn);
  }

  {
    TAOS* pConn = optionFunc("UTC+8");
    check_timezone(pConn, "show local variables", "UTC+8");
    checkRows(pConn, "select * from db1.t1 where ts == '2023-09-16 01:00:00'", 1);
    execQuery(pConn, "insert into db1.t1 values('2023-09-16 17:00:01', 1)");

    taos_close(pConn);
  }

  {
    TAOS* pConn = optionFunc("UTC+0");
    check_timezone(pConn, "show local variables", "UTC+0");
    checkRows(pConn, "select * from db1.t1 where ts == '2023-09-16 09:00:00'", 1);
    checkRows(pConn, "select * from db1.t1 where ts == '2023-09-17 01:00:01'", 1);

    taos_close(pConn);
  }
}

TEST(timezoneCase, set_timezone_Test) {
  check_set_timezone(getConnWithGlobalOption);
  check_set_timezone(getConnWithOption);
}

TEST(timezoneCase, alter_timezone_Test) {
  TAOS* pConn = getConnWithGlobalOption("UTC-8");
  check_timezone(pConn, "show local variables", "UTC-8");

  execQuery(pConn, "alter local 'timezone Asia/Kolkata'");
  check_timezone(pConn, "show local variables", "Asia/Kolkata");

  execQuery(pConn, "alter local 'timezone Asia/Shanghai'");
  check_timezone(pConn, "show local variables", "Asia/Shanghai");

  execQueryFail(pConn, "alter dnode 1 'timezone Asia/Kolkata'");
}

char *tz_test[] = {
    "2023-09-16 17:00:00+",
    "2023-09-16 17:00:00+8:",
    "2023-09-16 17:00:00+:30",
    "2023-09-16 17:00:00+:-30",
    "2023-09-16 17:00:00++:-30",
    "2023-09-16 17:00:00+:",
    "2023-09-16 17:00:00+8f:",
    "2023-09-16 17:00:00+080:",
    "2023-09-16 17:00:00+a",
    "2023-09-16 17:00:00+09:",
    "2023-09-16 17:00:00+09:a",
    "2023-09-16 17:00:00+09:abc",
    "2023-09-16 17:00:00+09:001",
};

void do_insert_failed(){
  TAOS* pConn = getConnWithGlobalOption("UTC-8");

  for (unsigned int i = 0; i < sizeof (tz_test) / sizeof (tz_test[0]); ++i){
    char sql[1024] = {0};
    (void)snprintf(sql, sizeof(sql), "insert into db1.ctb1 values('%s', '%s', 1)", tz_test[i], tz_test[i]);

    execQueryFail(pConn, sql);
  }
  taos_close(pConn);
}

struct insert_params
{
  const char *tz;
  const char *tbname;
  const char *t1;
  const char *t2;
};

struct insert_params params1[] = {
    {"UTC",              "ntb", "2023-09-16 17:00:00", "2023-09-16 17:00:00+08:00"},
    {"UTC",              "ctb1", "2023-09-16 17:00:00", "2023-09-16 17:00:00+08:00"},
};

struct insert_params params2[] = {
    {"UTC+9",              "ntb", "2023-09-16 08:00:00", "2023-09-16 08:00:00-01:00"},
    {"UTC+9",              "ctb1", "2023-09-16 08:00:00", "2023-09-16 11:00:00+02:00"},
};

void do_insert(struct insert_params params){
  TAOS* pConn = getConnWithOption(params.tz);
  char sql[1024] = {0};
  (void)snprintf(sql, sizeof(sql), "insert into db1.%s values('%s', '%s', 1)", params.tbname, params.t1, params.t2);
  execQuery(pConn, sql);
  taos_close(pConn);
}

void do_select(struct insert_params params){
  TAOS* pConn = getConnWithOption(params.tz);
  char sql[1024] = {0};
  (void)snprintf(sql, sizeof(sql), "select * from db1.%s where ts == '%s' and c1 == '%s'", params.tbname, params.t1, params.t2);
  checkRows(pConn, sql, 1);
  taos_close(pConn);
}

// test insert string and integer to timestamp both normal table and child table(and tag)
TEST(timezoneCase, insert_with_timezone_Test) {
  /*
   * 1. prepare data, create db and tables
   */
  TAOS* pConn1 = getConnWithOption("UTC+2");
  execQuery(pConn1, "drop database if exists db1");
  execQuery(pConn1, "create database db1");
  execQuery(pConn1, "create table db1.ntb (ts timestamp, c1 timestamp, c2 int)");
  execQuery(pConn1, "create table db1.stb (ts timestamp, c1 timestamp, c2 int) tags(t1 timestamp, t2 timestamp, t3 int)");
  execQuery(pConn1, "create table db1.ctb1 using db1.stb tags(\"2023-09-16 17:00:00+05:00\", \"2023-09-16 17:00:00\", 1)");
  execQuery(pConn1, "create table db1.ctb2 using db1.stb tags(1732178775000, 1732178775000, 1)");
  execQuery(pConn1, "insert into db1.ntb values(1732178775133, 1732178775133, 1)");
  execQuery(pConn1, "insert into db1.ctb1 values(1732178775133, 1732178775133, 1)"); //2024-11-21 10:46:15.133+02:00
  execQuery(pConn1, "insert into db1.ctb2 values(1732178775133, 1732178775133, 1)");

  /*
   * 2. test tag and timestamp with integer format
   */
  TAOS* pConn2 = getConnWithOption("UTC-2");
  checkRows(pConn2, "select * from db1.stb where t1 == '2023-09-16 17:00:00+05:00' and t2 == '2023-09-16 21:00:00'", 1);
  checkRows(pConn2, "select * from db1.stb where t1 == '2024-11-21 16:46:15+08:00' and t2 == '2024-11-21 09:46:15+01:00'", 1);
  checkRows(pConn2, "select * from db1.ntb where ts == '2024-11-21 09:46:15.133+01:00' and c1 == '2024-11-21 10:46:15.133'", 1);
  checkRows(pConn2, "select * from db1.ctb1 where ts == '2024-11-21 09:46:15.133+01:00' and c1 == '2024-11-21 10:46:15.133'", 1);

  check_sql_result(pConn2, "select TO_ISO8601(ts) from db1.ctb1", "2024-11-21T10:46:15.133+0200");   // 2024-01-01 23:00:00+0200


  /*
   * 3. test timestamp with string format
   */
  for (unsigned int i = 0; i < sizeof (params1) / sizeof (params1[0]); ++i){
    do_insert(params1[i]);
    do_select(params1[i]);
    do_select(params2[i]);
  }

  do_insert_failed();
  /*
   * 4. test NULL timezone, use default timezone UTC-8
   */
  TAOS* pConn3 = getConnWithOption(NULL);
  checkRows(pConn3, "select * from db1.stb where t1 == '2023-09-16 20:00:00' and t2 == '2023-09-17 03:00:00'", 2);
  checkRows(pConn3, "select * from db1.stb where t1 == 1732178775000 and t2 == 1732178775000", 1);
  checkRows(pConn3, "select * from db1.ntb where ts == '2024-11-21 16:46:15.133' and c1 == '2024-11-21 16:46:15.133'", 1);
  checkRows(pConn3, "select * from db1.ctb1 where ts == '2023-09-17 01:00:00' and c1 == '2023-09-16 17:00:00'", 1);

  /*
   * 5. test multi connection with different timezone
   */
  checkRows(pConn2, "select * from db1.ctb1 where ts == '2024-11-21 09:46:15.133+01:00' and c1 == '2024-11-21 10:46:15.133'", 1);
  checkRows(pConn1, "select * from db1.ctb1 where ts == '2024-11-21 09:46:15.133+01:00' and c1 == '2024-11-21 06:46:15.133'", 1);

  taos_close(pConn1);
  taos_close(pConn2);
  taos_close(pConn3);
}

TEST(timezoneCase, func_timezone_Test) {
  TAOS* pConn = getConnWithGlobalOption("UTC+8");
  check_sql_result(pConn, "select timezone()", "UTC+8 (UTC, -0800)");
  taos_close(pConn);

  pConn = getConnWithOption("UTC-2");

  execQuery(pConn, "drop database if exists db1");
  execQuery(pConn, "create database db1");
  execQuery(pConn, "create table db1.ntb (ts timestamp, c1 binary(32), c2 int)");
  execQuery(pConn, "insert into db1.ntb values(1704142800000, '2024-01-01 23:00:00', 1)");   // 2024-01-01 23:00:00+0200

  // test timezone
  check_sql_result(pConn, "select timezone()", "UTC-2 (UTC, +0200)");

  // test timetruncate
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-01 23:00:00', 1d, 0))", "2024-01-01T02:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-01 01:00:00', 1d, 0))", "2023-12-31T02:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-01 01:00:00+0300', 1d, 0))", "2023-12-31T02:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-01 01:00:00-0300', 1d, 0))", "2024-01-01T02:00:00.000+0200");

  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-04 23:00:00', 1w, 0))", "2024-01-04T02:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-04 01:00:00', 1w, 0))", "2023-12-28T02:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-04 01:00:00+0300', 1w, 0))", "2023-12-28T02:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-04 01:00:00-0300', 1w, 0))", "2024-01-04T02:00:00.000+0200");

  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-01 23:00:00', 1d, 1))", "2024-01-01T00:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-01 01:00:00', 1d, 1))", "2024-01-01T00:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-01 01:00:00+0500', 1d, 1))", "2023-12-31T00:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-01 01:00:00-0300', 1d, 1))", "2024-01-01T00:00:00.000+0200");

  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-04 23:00:00', 1w, 1))", "2024-01-04T00:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-04 01:00:00', 1w, 1))", "2024-01-04T00:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-04 01:00:00+0500', 1w, 1))", "2023-12-28T00:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE('2024-01-04 01:00:00-0300', 1w, 1))", "2024-01-04T00:00:00.000+0200");

  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE(1704142800000, 1d, 0))", "2024-01-01T02:00:00.000+0200");   // 2024-01-01 23:00:00+0200
  check_sql_result(pConn, "select TO_ISO8601(TIMETRUNCATE(ts, 1w, 1)) from db1.ntb", "2023-12-28T00:00:00.000+0200");   // 2024-01-01 23:00:00+0200

  // TODAY
  check_sql_result_partial(pConn, "select TO_ISO8601(today())", "T00:00:00.000+0200");

  // NOW
  check_sql_result_partial(pConn, "select TO_ISO8601(now())", "+0200");

  // WEEKDAY
  check_sql_result_integer(pConn, "select WEEKDAY('2024-01-01')", 0);
  check_sql_result_integer(pConn, "select WEEKDAY('2024-01-01 03:00:00')", 0);
  check_sql_result_integer(pConn, "select WEEKDAY('2024-01-01 23:00:00+0200')", 0);
  check_sql_result_integer(pConn, "select WEEKDAY('2024-01-01 23:00:00-1100')", 1);
  check_sql_result_integer(pConn, "select WEEKDAY(1704142800000)", 0);
  check_sql_result_integer(pConn, "select WEEKDAY(ts) from db1.ntb", 1);

  // DAYOFWEEK
  check_sql_result_integer(pConn, "select DAYOFWEEK('2024-01-01')", 2);
  check_sql_result_integer(pConn, "select DAYOFWEEK('2024-01-01 03:00:00')", 2);
  check_sql_result_integer(pConn, "select DAYOFWEEK('2024-01-01 23:00:00+0200')", 2);
  check_sql_result_integer(pConn, "select DAYOFWEEK('2024-01-01 23:00:00-1100')", 3);
  check_sql_result_integer(pConn, "select DAYOFWEEK(1704142800000)", 2);
  check_sql_result_integer(pConn, "select DAYOFWEEK(ts) from db1.ntb", 3);

  // WEEK
  check_sql_result_integer(pConn, "select WEEK('2024-01-07')", 1);
  check_sql_result_integer(pConn, "select WEEK('2024-01-07 02:00:00')", 1);
  check_sql_result_integer(pConn, "select WEEK('2024-01-07 02:00:00+0200')", 1);
  check_sql_result_integer(pConn, "select WEEK('2024-01-07 02:00:00+1100')", 0);
  check_sql_result_integer(pConn, "select WEEK(1704142800000)", 0);     // 2024-01-01 23:00:00+0200
  check_sql_result_integer(pConn, "select WEEK(ts) from db1.ntb", 0);   // 2024-01-01 23:00:00+0200

  check_sql_result_integer(pConn, "select WEEK('2024-01-07', 3)", 1);
  check_sql_result_integer(pConn, "select WEEK('2024-01-07 02:00:00', 3)", 1);
  check_sql_result_integer(pConn, "select WEEK('2024-01-07 02:00:00+0200', 3)", 1);
  check_sql_result_integer(pConn, "select WEEK('2024-01-01 02:00:00+1100', 3)", 52);
  check_sql_result_integer(pConn, "select WEEK(1704142800000, 3)", 1);     // 2024-01-01 23:00:00+0200
  check_sql_result_integer(pConn, "select WEEK(ts, 3) from db1.ntb", 1);   // 2024-01-01 23:00:00+0200

  // WEEKOFYEAR
  check_sql_result_integer(pConn, "select WEEKOFYEAR('2024-01-07')", 1);
  check_sql_result_integer(pConn, "select WEEKOFYEAR('2024-01-07 02:00:00')", 1);
  check_sql_result_integer(pConn, "select WEEKOFYEAR('2024-01-07 02:00:00+0200')", 1);
  check_sql_result_integer(pConn, "select WEEKOFYEAR('2024-01-01 02:00:00+1100')", 52);
  check_sql_result_integer(pConn, "select WEEKOFYEAR(1704142800000)", 1);     // 2024-01-01 23:00:00+0200
  check_sql_result_integer(pConn, "select WEEKOFYEAR(ts) from db1.ntb", 1);   // 2024-01-01 23:00:00+0200

  // TO_ISO8601
  check_sql_result(pConn, "select TO_ISO8601(ts) from db1.ntb", "2024-01-01T23:00:00.000+0200");
  check_sql_result(pConn, "select TO_ISO8601(ts,'-08') from db1.ntb", "2024-01-01T13:00:00.000-08");
  check_sql_result(pConn, "select TO_ISO8601(1)", "1970-01-01T02:00:00.001+0200");
  check_sql_result(pConn, "select TO_ISO8601(1,'+0800')", "1970-01-01T08:00:00.001+0800");

  // TO_UNIXTIMESTAMP
  check_sql_result_integer(pConn, "select TO_UNIXTIMESTAMP(c1) from db1.ntb", 1704121200000);   // use timezone in server UTC-8
  check_sql_result_integer(pConn, "select TO_UNIXTIMESTAMP('2024-01-01T23:00:00.000+0200')", 1704142800000);
  check_sql_result_integer(pConn, "select TO_UNIXTIMESTAMP('2024-01-01T13:00:00.000-08')", 1704142800000);
  check_sql_result_integer(pConn, "select TO_UNIXTIMESTAMP('2024-01-01T23:00:00.001')", 1704142800001);

  // TO_TIMESTAMP
  check_sql_result_integer(pConn, "select TO_TIMESTAMP(c1,'yyyy-mm-dd hh24:mi:ss') from db1.ntb", 1704121200000);   // use timezone in server UTC-8
  check_sql_result_integer(pConn, "select TO_TIMESTAMP('2024-01-01 23:00:00+02:00', 'yyyy-mm-dd hh24:mi:ss tzh')", 1704142800000);
  check_sql_result_integer(pConn, "select TO_TIMESTAMP('2024-01-01T13:00:00-08', 'yyyy-mm-ddThh24:mi:ss tzh')", 1704142800000);
  check_sql_result_integer(pConn, "select TO_TIMESTAMP('2024/01/01 23:00:00', 'yyyy/mm/dd hh24:mi:ss')", 1704142800000);

  // TO_CHAR
  check_sql_result(pConn, "select TO_CHAR(ts,'yyyy-mm-dd hh24:mi:ss') from db1.ntb", "2024-01-02 05:00:00");   // use timezone in server UTC-8
  check_sql_result(pConn, "select TO_CHAR(cast(1704142800000 as timestamp), 'yyyy-mm-dd hh24:mi:ss tzh')", "2024-01-01 23:00:00 +02");
  check_sql_result(pConn, "select TO_CHAR(cast(1704142800000 as timestamp), 'yyyy-mm-dd hh24:mi:ss')", "2024-01-01 23:00:00");

  // TIMEDIFF
  check_sql_result_integer(pConn, "select TIMEDIFF(c1, '2024-01-01T23:00:00.001+02') from db1.ntb", -21600001);   // use timezone in server UTC-8
  check_sql_result_integer(pConn, "select TIMEDIFF(c1, '2024-01-01T23:00:00.001') from db1.ntb", -1);   // use timezone in server UTC-8
  check_sql_result_integer(pConn, "select TIMEDIFF('2024-01-01T23:00:00.001', '2024-01-01T13:00:00.000-08')", 1);

  // CAST
  check_sql_result_integer(pConn, "select CAST(c1 as timestamp) from db1.ntb", 1704121200000);
  check_sql_result_integer(pConn, "select CAST('2024-01-01T23:00:00.000+02' as timestamp)", 1704142800000);
  check_sql_result_integer(pConn, "select CAST('2024-01-01T23:00:00.000' as timestamp)", 1704142800000);

  taos_close(pConn);

  // hash join
  pConn = getConnWithOption("UTC+1");

  execQuery(pConn, "drop database if exists db1");
  execQuery(pConn, "create database db1");
  execQuery(pConn, "create table db1.ntb (ts timestamp, c1 binary(32), c2 int)");
  execQuery(pConn, "create table db1.ntb1 (ts timestamp, c1 binary(32), c2 int)");
  execQuery(pConn, "insert into db1.ntb values(1703987400000, '2023-12-31 00:50:00', 1)");   // 2023-12-31 00:50:00-0100
  execQuery(pConn, "insert into db1.ntb1 values(1704070200000, '2023-12-31 23:50:00', 11)");   // 2023-12-31 23:50:00-0100
  checkRows(pConn, "select a.ts,b.ts from db1.ntb a join db1.ntb1 b on timetruncate(a.ts, 1d) = timetruncate(b.ts, 1d)", 1);

  taos_close(pConn);

}

time_t time_winter = 1731323281;    // 2024-11-11 19:08:01+0800
time_t time_summer = 1731323281 - 120 * 24 * 60 * 60;

struct test_times
{
  const char *name;
  time_t  t;
  const char *timezone;
} test_tz[] = {
    {"",                 time_winter, " (UTC, +0000)"},
    {"America/New_York", time_winter, "America/New_York (EST, -0500)"},  // 2024-11-11 19:08:01+0800
    {"America/New_York", time_summer, "America/New_York (EDT, -0400)"},
    {"Asia/Kolkata",     time_winter, "Asia/Kolkata (IST, +0530)"},
    {"Asia/Shanghai",    time_winter, "Asia/Shanghai (CST, +0800)"},
    {"Europe/London",    time_winter, "Europe/London (GMT, +0000)"},
    {"Europe/London",    time_summer, "Europe/London (BST, +0100)"}
};

void timezone_str_test(const char* tz, time_t t, const char* tzStr) {
  int code = setenv("TZ", tz, 1);
  ASSERT(-1 != code);
  tzset();

  char str1[TD_TIMEZONE_LEN] = {0};
  ASSERT(taosFormatTimezoneStr(t, tz, NULL, str1) == 0);
  ASSERT_STREQ(str1, tzStr);
}

void timezone_rz_str_test(const char* tz, time_t t, const char* tzStr) {
  timezone_t sp = tzalloc(tz);
  ASSERT(sp);

  char str1[TD_TIMEZONE_LEN] = {0};
  ASSERT(taosFormatTimezoneStr(t, tz, sp, str1) == 0);
  ASSERT_STREQ(str1, tzStr);
  tzfree(sp);
}

TEST(timezoneCase, format_timezone_Test) {
  for (unsigned int i = 0; i < sizeof (test_tz) / sizeof (test_tz[0]); ++i){
    timezone_str_test(test_tz[i].name, test_tz[i].t, test_tz[i].timezone);
    timezone_str_test(test_tz[i].name, test_tz[i].t, test_tz[i].timezone);
  }
}

TEST(timezoneCase, get_tz_Test) {
  {
    char tz[TD_TIMEZONE_LEN] = {0};
    getTimezoneStr(tz);
    ASSERT_STREQ(tz, "Asia/Shanghai");

//    getTimezoneStr(tz);
//    ASSERT_STREQ(tz, "Asia/Shanghai");
//
//    getTimezoneStr(tz);
//    ASSERT_STREQ(tz, TZ_UNKNOWN);
  }
}

struct {
  const char *	env;
  time_t	expected;
} test_mk[] = {
    {"MST",	832935315},
    {"",		832910115},
    {":UTC",	832910115},
    {"UTC",	832910115},
    {"UTC0",	832910115}
};


TEST(timezoneCase, mktime_Test){
  struct tm tm;
  time_t t;

  memset (&tm, 0, sizeof (tm));
  tm.tm_isdst = 0;
  tm.tm_year  = 96;	/* years since 1900 */
  tm.tm_mon   = 4;
  tm.tm_mday  = 24;
  tm.tm_hour  =  3;
  tm.tm_min   = 55;
  tm.tm_sec   = 15;

  for (unsigned int i = 0; i < sizeof (test_mk) / sizeof (test_mk[0]); ++i)
  {
    setenv ("TZ", test_mk[i].env, 1);
    t = taosMktime (&tm, NULL);
    ASSERT (t == test_mk[i].expected);
  }
}

TEST(timezoneCase, mktime_rz_Test){
  struct tm tm;
  time_t t;

  memset (&tm, 0, sizeof (tm));
  tm.tm_isdst = 0;
  tm.tm_year  = 96;	/* years since 1900 */
  tm.tm_mon   = 4;
  tm.tm_mday  = 24;
  tm.tm_hour  =  3;
  tm.tm_min   = 55;
  tm.tm_sec   = 15;

  for (unsigned int i = 0; i < sizeof (test_mk) / sizeof (test_mk[0]); ++i)
  {
    timezone_t tz = tzalloc(test_mk[i].env);
    ASSERT(tz);
    t = taosMktime(&tm, tz);
    ASSERT (t == test_mk[i].expected);
    tzfree(tz);
  }
}

TEST(timezoneCase, localtime_performance_Test) {
  timezone_t sp = tzalloc("Asia/Shanghai");
  ASSERT(sp);

  int cnt = 10000000;
  int times = 10;
  int64_t time_localtime = 0;
  int64_t time_localtime_rz = 0;
//  int cnt = 1000000;
  for (int i = 0; i < times; ++i) {
    int64_t t1 = taosGetTimestampNs();
    for (int j = 0; j < cnt; ++j) {
      time_t t = time_winter - j;
      struct tm tm1;
      ASSERT (taosLocalTime(&t, &tm1, NULL, 0, NULL));
    }
    int64_t tmp = taosGetTimestampNs() - t1;
    printf("localtime cost:%" PRId64 " ns, run %d times", tmp, cnt);
    time_localtime += tmp/cnt;

    printf("\n");



    int64_t t2 = taosGetTimestampNs();
    for (int j = 0; j < cnt; ++j) {
      time_t t = time_winter - j;
      struct tm tm1;
      ASSERT (taosLocalTime(&t, &tm1, NULL, 0, sp));
    }
    tmp = taosGetTimestampNs() - t2;
    printf("localtime_rz cost:%" PRId64 " ns, run %d times", tmp, cnt);
    time_localtime_rz += tmp/cnt;
    printf("\n\n");
  }
  printf("average: localtime cost:%" PRId64 " ns, localtime_rz cost:%" PRId64 " ns", time_localtime/times, time_localtime_rz/times);
  tzfree(sp);
}


#pragma GCC diagnostic pop
