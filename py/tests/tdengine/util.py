import datetime
import taos
import logging

def init_logging(log_file: str = '/tmp/td.log', log_level = logging.DEBUG):
    logging.basicConfig(level=log_level, filename=log_file, format='%(asctime)s %(levelname)s %(message)s', datefmt='%Y/%m/%d %I:%M:%S %p')

db_name = 'test1'
db_flags = ''
stb_name = 'st'
stb_flags = ''

child_table_num = 10

child_table_row_num = 100000

start_datetime = datetime.datetime(year=2020, month=1, day=1, hour=1, minute=00)

skip_row_interval = 3


def get_connection(host: str = '127.0.0.1', port: int = 6030) ->taos.TaosConnection:
    try:
        conn = taos.connect(host=host,
                            port=port,
                            user='root',
                            password='taosdata',
                            datbase='')
    except Exception as e:
        logging.warning("failed to get connection: %s" % str(e))
        return None
    logging.debug('client info: %s' % conn.client_info)
    logging.debug('server info: %s' % conn.server_info)
    return conn

def prepare_data():
    conn = get_connection()
    if conn is None:
        raise Exception("prepare data failed cannot get conn")

    conn.execute("drop database if exists {}".format(db_name))
    conn.execute("create database {} {}".format(db_name, db_flags))
    conn.select_db(db_name)
    conn.execute("create stable {} (ts timestamp, c2 int) tags(tg int)".format(stb_name))

    cur_datetime = start_datetime
    for i in reversed(range(1, child_table_num+1)):
        for j in reversed(range(1, child_table_row_num+1)):
            cur_datetime = cur_datetime - datetime.timedelta(seconds=1)
            #if j % skip_row_interval or j % (1+skip_row_interval) == 0:
                #continue
            if j % (skip_row_interval+1) == 0 or j % skip_row_interval:
                continue
            conn.execute("insert into t{} using {} tags({}), values('{}', {})"
                         .format(i, stb_name, i, cur_datetime.replace(microsecond=0), j))
