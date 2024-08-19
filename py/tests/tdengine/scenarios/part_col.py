import logging
from taosutils import LIMIT_MAX, LIMIT_MIN, LIMIT_STEP, add_hint, add_limit, perf_sqls

def generate_base_sqls() -> list[str]:
    sqls = []
    sql_template = 'select count(*), %s from meters partition by %s'
    for i in range(1, 3):
        key = "c%d" % (i)
        sql = sql_template % (key, key)
        sqls.append(sql)
        original_hint_sql = add_hint(sql)
        sql = sql + ' slimit '
        sqls.extend(add_limit(sql, LIMIT_MIN, 10000, 2000))
        hint_sql = add_hint(sql)
        sqls.append(original_hint_sql)
        sqls.extend(add_limit(hint_sql, LIMIT_MIN, 10000, 2000))
    return sqls

def run(conn):
    conn.select_db('test')
    logging.info('test database')
    #perf_sqls(conn, generate_base_sqls, 10)
    conn.select_db('test100b')
    logging.info('test100b database')
    perf_sqls(conn, generate_base_sqls, 1)
