import taos

from util.log import *
from util.sql import *
from util.cases import *
from util.dnodes import *
from util.common import *

class TDTestCase:
    def init(self, conn, logSql, replicaVar=1):
        self.replicaVar = int(replicaVar)
        tdLog.debug(f"start to excute {__file__}")
        tdSql.init(conn.cursor(), True)
        self.dbname = 'db'
        self.stbname = 'st'

    def prepareData(self):
        # db
        tdSql.execute(f"create database db;")
        tdSql.execute(f"use db")

        # super tableUNSIGNED
        tdSql.execute("CREATE TABLE t1( time TIMESTAMP, c1 BIGINT, c2 smallint, c3 double, c4 int UNSIGNED, c5 bool, c6 binary(32), c7 nchar(32));")

        # create index for all tags
        tdSql.execute("INSERT INTO t1 VALUES (1641024000000, 1, 1, 1, 1, 1, '1', '1.7')")
        tdSql.execute("INSERT INTO t1 VALUES (1641024000001, 0, 0, 1.7, 0, 0, '0', '')")
        tdSql.execute("INSERT INTO t1 VALUES (1641024000002, 1, 1, 1, 1, 1, '1', 'er')")

    def check(self):
        tdSql.query(f"SELECT * FROM t1 WHERE c1 = 1.7")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c1 in (1.7, 2)")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c1 not in (1.7, 2)")
        tdSql.checkRows(3)

        tdSql.query(f"SELECT * FROM t1 WHERE c2 = 1.7")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c2 in (1.7, 2)")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c2 not in (1.7, 2)")
        tdSql.checkRows(3)

        tdSql.query(f"SELECT * FROM t1 WHERE c3 = 1.7")
        tdSql.checkRows(1)
        tdSql.query(f"SELECT * FROM t1 WHERE c3 in (1.7, 2)")
        tdSql.checkRows(1)
        tdSql.query(f"SELECT * FROM t1 WHERE c3 not in (1.7, 2)")
        tdSql.checkRows(2)

        tdSql.query(f"SELECT * FROM t1 WHERE c4 = 1.7")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c4 in (1.7, 2)")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c4 not in (1.7, 2)")
        tdSql.checkRows(3)

        tdSql.query(f"SELECT * FROM t1 WHERE c5 = 1.7")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c5 in (1.7, 2)")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c5 not in (1.7, 2)")
        tdSql.checkRows(3)

        tdSql.query(f"SELECT * FROM t1 WHERE c6 = 1.7")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c6 in (1.7, 2)")
        tdSql.checkRows(0)
        tdSql.query(f"SELECT * FROM t1 WHERE c6 not in (1.7, 2)")
        tdSql.checkRows(3)

        tdSql.query(f"SELECT * FROM t1 WHERE c6 = 1")
        tdSql.checkRows(2)
        tdSql.query(f"SELECT * FROM t1 WHERE c6 in (1, 2)")
        tdSql.checkRows(2)
        tdSql.query(f"SELECT * FROM t1 WHERE c6 not in (1, 2)")
        tdSql.checkRows(1)

        tdSql.query(f"SELECT * FROM t1 WHERE c6 = 0")
        tdSql.checkRows(1)
        tdSql.query(f"SELECT * FROM t1 WHERE c6 in (0, 2)")
        tdSql.checkRows(1)
        tdSql.query(f"SELECT * FROM t1 WHERE c6 not in (0, 2)")
        tdSql.checkRows(2)

        tdSql.query(f"SELECT * FROM t1 WHERE c7 = 1.7")
        tdSql.checkRows(1)
        tdSql.query(f"SELECT * FROM t1 WHERE c7 in (1.7, 2)")
        tdSql.checkRows(1)
        tdSql.query(f"SELECT * FROM t1 WHERE c7 not in (1.7, 2)")
        tdSql.checkRows(2)

        tdSql.query(f"SELECT * FROM t1 WHERE c7 = 0")
        tdSql.checkRows(2)
        tdSql.query(f"SELECT * FROM t1 WHERE c7 in (0, 2)")
        tdSql.checkRows(2)
        tdSql.query(f"SELECT * FROM t1 WHERE c7 not in (0, 2)")
        tdSql.checkRows(1)

        tdSql.query(f"SELECT * FROM t1 WHERE c7 = ''")
        tdSql.checkRows(1)
        tdSql.query(f"SELECT * FROM t1 WHERE c7 in ('', 2)")
        tdSql.checkRows(1)
        tdSql.query(f"SELECT * FROM t1 WHERE c7 not in ('', 2)")
        tdSql.checkRows(2)

    def run(self):
        self.prepareData()
        self.check()

    def stop(self):
        tdSql.close()
        tdLog.success(f"{__file__} successfully executed")



tdCases.addLinux(__file__, TDTestCase())
tdCases.addWindows(__file__, TDTestCase())