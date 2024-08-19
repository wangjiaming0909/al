

import logging
from taosutils import add_hint, perf_sqls


def generate_base_sqls():
    sql_template = 'select count(*), %s from meters %s by %s'

    clauses = ['partition', 'group']
    by_what = ['tbname', 'cc', 'c0', 'c1', 'c2', 'c3']

    sqls = []

    for by in by_what:
        for clause in clauses:
            sql = sql_template % (by, clause, by)
            sqls.append(sql)
            if clause == 'partition':
                sqls.append(add_hint(sql))


    return sqls

def run(conn):
    conn.select_db('test')
    logging.info('db test')
    perf_sqls(conn, generate_base_sqls, 1);
    conn.select_db('test100b')
    logging.info('db test100b')
    perf_sqls(conn, generate_base_sqls, 1);
