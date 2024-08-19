from taos import TaosConnection
import logging
import datetime
import time

MIN = 1
MAX = 10001
STEP = 1000

TB_MIN = 1
TB_MAX = 50002
TB_STEP = 5000

TG_MIN = 1
TG_MAX = 250
TG_STEP = 25

LIMIT_MIN = 1
LIMIT_MAX = 100000
LIMIT_STEP = 1000


def perf_sql(conn: TaosConnection, sql: str) ->float:
    #time.sleep(0.2)
    logging.debug(sql)
    start_t = datetime.datetime.now()
    res = conn.query(sql)
    res.fetch_all()
    row_count = res.row_count
    res.close()
    end_t = datetime.datetime.now()
    used_time_ms = (end_t.timestamp() - start_t.timestamp()) * 1000
    logging.debug("%f ms, sql: %s, row_count %d: " % (used_time_ms, sql, row_count))
    return used_time_ms

def add_limit(sql_template: str, min: int, max: int, step: int) -> list[str]:
    sqls = []
    for slimit in range(min, max, step):
        sqls.append(sql_template + str(slimit))
    return sqls

def perf_sqls(conn, func, times: int = 10):
    sqls = func()
    for sql in sqls:
        time_sum = 0
        for i in range(0, times):
            try:
                t = perf_sql(conn, sql)
                logging.debug("time: %f ms" % t)
                time_sum = time_sum + t
            except Exception as e:
                logging.error('perf sql failed with sql: %s, err: %s' % (sql, str(e)))
                raise
        logging.info("%f ms, sql: %s" % (time_sum / times, sql))

def add_hint(sql: str) -> str:
    return "select /*+ sort_for_group() */ %s" % sql.removeprefix("select ")

