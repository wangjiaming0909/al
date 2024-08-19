import datetime
import time
from typing import List

from taos import TaosConnection
from util import init_logging
from util import get_connection
import threading
import logging
from taosutils import add_limit, add_hint, perf_sql, perf_sqls
from taosutils import TB_MAX, TB_MIN, TB_STEP
from taosutils import TG_MAX, TG_MIN, TG_STEP
from taosutils import MAX, MIN, STEP
from taosutils import LIMIT_MAX, LIMIT_MIN, LIMIT_STEP
import scenarios.part_col
import scenarios.group_col


def partition_by_tbname_slimit() -> List[str]:
    sql_template = "select tbname, count(*) from meters partition by tbname slimit "
    return add_limit(sql_template, TB_MIN, TB_MAX, TB_STEP)

def partition_by_tbname_interval_slimit() -> List[str]:
    sql_template = "select tbname, count(*) from meters partition by tbname interval(1s) slimit "
    return add_limit(sql_template, TB_MIN, TB_MAX, TB_STEP)

def partition_by_tag_slimit() -> List[str]:
    sql_template = "select cc, count(*) from meters partition by cc slimit "
    return add_limit(sql_template, TG_MIN, TG_MAX, TG_STEP)

def partition_by_tag_slimit_no_agg() -> list[str]:
    sql_template = 'select cc, ts from meters partition by cc slimit '
    return add_limit(sql_template, TG_MIN, TG_MAX, TG_STEP)

def partition_by_tag_interval_slimit() -> List[str]:
    sql_template = "select cc, count(*) from meters partition by cc interval(1s) slimit "
    return add_limit(sql_template, TG_MIN, TG_MAX, TG_STEP)

def partition_by_tbname_interval_slimit_limit() -> List[str]:
    slimit_sqls = partition_by_tbname_interval_slimit()
    sqls = []
    for sql in slimit_sqls:
        tmp_sqls = add_limit(sql + ' limit ', MIN, MAX, STEP)
        for s in tmp_sqls:
            sqls.append(s)
    return sqls

def partition_by_tag_interval_slimit_limit() -> List[str]:
    slimit_sqls = partition_by_tag_interval_slimit()
    sqls = []
    for sql in slimit_sqls:
        tmp_sqls = add_limit(sql + ' limit ', MIN, MAX, STEP)
        for s in tmp_sqls:
            sqls.append(s)
    return sqls

def partition_by_tbname_interval_limit() -> List[str]:
    sql_template = "select tbname, count(*) from meters partition by tbname interval(100a) slimit 1 limit "
    return add_limit(sql_template, LIMIT_MIN, LIMIT_MAX, LIMIT_STEP)

def partition_by_tag_interval_limit() -> List[str]:
    sql_template = "select cc, count(*) from meters partition by cc interval(100a) slimit 1 limit "
    return add_limit(sql_template, LIMIT_MIN, LIMIT_MAX, LIMIT_STEP)

def partition_by_col_varchar_slimit() -> List[str]:
    sql_template = "select count(*), c3 from meters partition by c3 slimit "
    ret = add_limit(sql_template, MIN, MAX, STEP)
    return ret

def partition_by_col_int_slimit() -> List[str]:
    sql_template = "select count(*), c0 from meters partition by c0 slimit "
    ret = add_limit(sql_template, MIN, MAX, STEP)
    return ret

def partition_by_col_varchar_interval_slimit() -> List[str]:
    sql_template = "select count(*), _wstart, c3 from meters partition by c3 interval(1s) slimit "
    return add_limit(sql_template, MIN, MAX, STEP)

def partition_by_col_int_interval_slimit() -> List[str]:
    sql_template = "select count(*), _wstart, c0 from meters partition by c0 interval(1s) slimit "
    return add_limit(sql_template, MIN, MAX, STEP)

def partition_by_col_with_sort() -> list[str]:
    sql_template = "select count(*), %s from meters partition by %s"
    sqls = []
    for i in range(0, 4):
        key = "c%d" % (i)
        sql = sql_template % (key, key)
        sqls.append(sql)
        sqls.append(add_hint(sql))
    return sqls

def partition_by_col_interval() ->list[str]:
    sql_template = "select _wstart, _wend, count(*), %s from meters partition by %s interval(%s)"
    sqls = []
    sqls.append(sql_template % ('c0', 'c0', '1s'))
    sqls.append(sql_template % ('c0', 'c0', '30s'))
    sqls.append(sql_template % ('c0', 'c0', '1m'))
    sqls.append(sql_template % ('c0', 'c0', '30m'))

    sqls.append(sql_template % ('c1', 'c1', '1s'))
    sqls.append(sql_template % ('c1', 'c1', '30s'))
    sqls.append(sql_template % ('c1', 'c1', '1m'))
    sqls.append(sql_template % ('c1', 'c1', '30m'))

    sqls.append(sql_template % ('c2', 'c2', '1s'))
    sqls.append(sql_template % ('c2', 'c2', '30s'))
    sqls.append(sql_template % ('c2', 'c2', '1m'))
    sqls.append(sql_template % ('c2', 'c2', '30m'))

    sqls.append(sql_template % ('c3', 'c3', '1s'))
    sqls.append(sql_template % ('c3', 'c3', '30s'))
    sqls.append(sql_template % ('c3', 'c3', '1m'))
    sqls.append(sql_template % ('c3', 'c3', '30m'))

    return sqls


def perf_partition_by_tag_tbname(conn):
    conn.execute("use test")
    sql = "select cc, count(*) from meters where ts < '2023-08-08 18:17:40' partition by cc slimit 250"
    perf_sqls(conn, partition_by_tbname_slimit)
    perf_sqls(conn, partition_by_tbname_interval_slimit)
    perf_sqls(conn, partition_by_tag_slimit)
    perf_sqls(conn, partition_by_tag_interval_slimit)
    perf_sqls(conn, partition_by_tbname_interval_slimit_limit)
    perf_sqls(conn, partition_by_tag_interval_slimit_limit)

    conn.select_db("test1")
    perf_sqls(conn, partition_by_tbname_interval_limit)
    perf_sqls(conn, partition_by_tag_interval_limit)
    conn.select_db("test1")
    perf_sqls(conn, partition_by_col_varchar_slimit, 1)
    perf_sqls(conn, partition_by_col_int_slimit, 1)
    perf_sqls(conn, partition_by_col_varchar_interval_slimit)
    perf_sqls(conn, partition_by_col_int_interval_slimit)

def perf_partition_col_with_sort(conn):
    conn.select_db("test")
    perf_sqls(conn, partition_by_col_with_sort, 5)
    conn.select_db("test100b")
    perf_sqls(conn, partition_by_col_with_sort, 1)

def partition_by_col_interval_with_sort() -> list[str]:
    sql_template = 'select _wstart, count(*), %s from meters partition by %s interval(%s)'
    sqls = []
    intervals = ['1s', '30s', '1m', '30m', '1h']
    for i in range(0, 4):
        key = 'c%d' % (i)
        sql = sql_template % (key, key, '%s')
        for interval in intervals:
            interval_sql = sql % (interval)
            sqls.append(interval_sql)
            sqls.append(add_hint(interval_sql))
    return sqls

def perf_partition_col_interval_with_sort(conn):
    conn.select_db('test')
    perf_sqls(conn, partition_by_col_interval_with_sort, 10)
    conn.select_db('test100b')
    perf_sqls(conn, partition_by_col_interval_with_sort, 1)


def perf_partition_col_interval_output_block_with_order(conn):
    conn.select_db("test")
    logging.debug("changed to db test")
    perf_sqls(conn, partition_by_col_interval, 10)
    conn.select_db("test100b")
    logging.debug("changed to db test100b")
    perf_sqls(conn, partition_by_col_interval, 1)

def perf_partition_tag_no_agg(conn):
    conn.select_db('test100b')
    perf_sqls(conn, partition_by_tag_slimit_no_agg, 5)

def partition_col_no_agg() -> list[str]:
    sql_template = 'select ts, c1,c2,cc from meters partition by %s slimit 10 limit 1'
    sqls = []
    for i in range(0, 4):
        key = 'c%d' % (i)
        sql = sql_template % (key)
        sqls.append(sql)
        sqls.append(add_hint(sql))
    return sqls

def perf_partition_col_no_agg(conn):
    conn.select_db('test')
    logging.debug("changed to db test")
    perf_sqls(conn, partition_col_no_agg, 1)
    conn.select_db('test100b')
    logging.debug("changed to db test100b")
    perf_sqls(conn, partition_col_no_agg, 1)

def partition_with_last() -> list[str]:
    sql_template = 'select last(%s) from meters %s by %s'
    sqls = []
    for i in range(0,4):
        key = 'c%d' % (i)
        sqls.append(sql_template % (key, 'group', 'tbname'))
        sqls.append(sql_template % (key, 'partition', 'tbname'))

        sqls.append(sql_template % (key, 'group', 'cc'))
        sqls.append(sql_template % (key, 'partition', 'cc'))
    return sqls

def perf_partition_with_last(conn):
    conn.select_db('test')
    logging.debug("changed to db test")
    perf_sqls(conn, partition_with_last, 1)
    conn.select_db('test100b')
    logging.debug("changed to db test100b")
    perf_sqls(conn, partition_with_last, 1)

def partition_with_count() -> list[str]:
    sql_template = 'select count(*), %s from meters %s by %s'
    sqls = []
    sqls.append(sql_template % ('tbname', 'group', 'tbname'))
    sqls.append(sql_template % ('tbname', 'partition', 'tbname'))

    sqls.append(sql_template % ('cc', 'group', 'cc'))
    sqls.append(sql_template % ('cc', 'partition', 'cc'))
    return sqls

def perf_partition_with_count(conn):
    conn.select_db('test')
    logging.debug("changed to db test")
    perf_sqls(conn, partition_with_count, 1)
    conn.select_db('test100b')
    logging.debug("changed to db test100b")
    perf_sqls(conn, partition_with_count, 1)


def test_utf8(conn: TaosConnection):
    conn.select_db('test')
    sql = 'select ts as 时间戳 from meters limit 10'
    perf_sql(conn, sql)
    sql = 'select 时间戳 from (select ts as 时间戳, c0 as c第一列, cc as 标签cc from meters where c第一列 != 0 limit 10) where 时间戳 > "2023-10-10" limit 1'
    perf_sql(conn, sql)

def test_select_to_char(conn: TaosConnection):
    conn.select_db('test')
    format = 'yyyy-mm-dd hh:mi:ss'
    for i in range(500):
        format = format + '1234567890'
    #sql = "select to_char(ts, '%s') from meters limit 1" % (format)
    sql = "select to_timestamp('2023年10-10 10:10:10', 'yyyy年 MM-DD hh:MI:SS')"
    res = conn.query(sql)
    res = res.fetch_all()
    print(str(res))

def test_insert_data(conn: TaosConnection):
    conn.select_db('test')
    sql = 'insert into t0 values(now, 1, 1, 501, now, "asd")'
    for i in range(1, 21):
        conn.execute(sql)

if __name__ == "__main__":
    init_logging(log_file='a.log', log_level=logging.DEBUG)
    conn = get_connection('127.0.0.2', 6031)
    if conn is None:
        exit(-1)
    logging.info("succeed to connect to server")

    #test_utf8(conn)
    #test_select_to_char(conn)

    #perf_partition_by_tag_tbname(conn)
    #perf_partition_col_with_sort(conn)
    #perf_partition_col_interval_output_block_with_order(conn)
    #perf_partition_col_interval_with_sort(conn)
    #perf_partition_tag_no_agg(conn)
    #perf_partition_col_no_agg(conn)
    #perf_partition_with_last(conn)
    #perf_partition_with_count(conn)
    #scenarios.part_col.run(conn)
    #scenarios.group_col.run(conn)
    test_insert_data(conn)
    conn.close()
