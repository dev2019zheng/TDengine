/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, AND/or modify
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

#include "planTestUtil.h"
#include "planner.h"

using namespace std;

class PlanBasicTest : public PlannerTestBase {};

TEST_F(PlanBasicTest, selectClause) {
  useDb("root", "test");

  run("SELECT * FROM t1");
  run("SELECT 1 FROM t1");
  run("SELECT * FROM st1");
  run("SELECT 1 FROM st1");
}

TEST_F(PlanBasicTest, whereClause) {
  useDb("root", "test");

  run("SELECT * FROM t1 WHERE c1 > 10");

  run("SELECT * FROM t1 WHERE ts > TIMESTAMP '2022-04-01 00:00:00' and ts < TIMESTAMP '2022-04-30 23:59:59'");
}

TEST_F(PlanBasicTest, func) {
  useDb("root", "test");

  run("SELECT DIFF(c1) FROM t1");

  run("SELECT PERCENTILE(c1, 60) FROM t1");

  run("SELECT TOP(c1, 60) FROM t1");

  run("SELECT TOP(c1, 60) FROM st1");
}

TEST_F(PlanBasicTest, uniqueFunc) {
  useDb("root", "test");

  run("SELECT UNIQUE(c1) FROM t1");

  run("SELECT UNIQUE(c2 + 10) FROM t1 WHERE c1 > 10");

  run("SELECT UNIQUE(c2 + 10), c2 FROM t1 WHERE c1 > 10");

  run("SELECT UNIQUE(c2 + 10), ts, c2 FROM t1 WHERE c1 > 10");

  run("SELECT UNIQUE(c1) a FROM t1 ORDER BY a");
}

TEST_F(PlanBasicTest, tailFunc) {
  useDb("root", "test");

  run("SELECT TAIL(c1, 10) FROM t1");

  run("SELECT TAIL(c2 + 10, 10, 80) FROM t1 WHERE c1 > 10");

  run("SELECT TAIL(c2 + 10, 10, 80) FROM t1 WHERE c1 > 10 PARTITION BY c1");

  run("SELECT TAIL(c2 + 10, 10, 80) FROM t1 WHERE c1 > 10 ORDER BY 1");

  run("SELECT TAIL(c2 + 10, 10, 80) FROM t1 WHERE c1 > 10 LIMIT 5");

  run("SELECT TAIL(c2 + 10, 10, 80) FROM t1 WHERE c1 > 10 PARTITION BY c1 LIMIT 5");
}

TEST_F(PlanBasicTest, interpFunc) {
  useDb("root", "test");

  run("SELECT INTERP(c1) FROM t1");

  run("SELECT INTERP(c1) FROM t1 RANGE('2017-7-14 18:00:00', '2017-7-14 19:00:00') EVERY(5s) FILL(LINEAR)");
}

TEST_F(PlanBasicTest, lastRowFunc) {
  useDb("root", "test");

  run("SELECT LAST_ROW(c1) FROM t1");

  run("SELECT LAST_ROW(*) FROM t1");

  run("SELECT LAST_ROW(c1, c2) FROM t1");

  run("SELECT LAST_ROW(c1) FROM st1");
}

TEST_F(PlanBasicTest, withoutFrom) {
  useDb("root", "test");

  run("SELECT 1");
}
