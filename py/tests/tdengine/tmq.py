import taos
from taos.tmq import Consumer

def get_connection():
    try:
        conn = taos.connect(host='localhost',
                            port=6030,
                            user='root',
                            password='taosdata',
                            datbase='test')
    except Exception as e:
        print('get connection err: ', e)
        return None
    print('client info: ', conn.client_info)
    print('server info: ', conn.server_info)
    return conn;


def init_tmp_env(db: str, topic):
    conn = get_connection();
    if conn is None:
        raise Exception("get connection failed")
    conn.execute("drop topic if exists {}".format(topic))
    conn.execute("drop database if exists {}".format(db))
    conn.execute("create database if not exists {} wal_retention_period 3600".format(db))
    conn.select_db(db)
    conn.execute("create stable stb1 (ts timestamp, c1 int, c2 float, c3 varchar(16)) tags(t1 int, t3 varchar(16))")
    conn.execute("create table if not exists tb1 using stb1 tags(1, 't1')")
    conn.execute("create table if not exists tb2 using stb1 tags(2, 't2')")
    conn.execute("create table if not exists tb3 using stb1 tags(3, 't3')")
    conn.execute("create topic if not exists {} as select ts, c1, c2, c3 from stb1".format(topic))
    conn.execute("insert into tb1 values (now, 1, 1.0, 'tmq test')")
    conn.execute("insert into tb2 values (now, 2, 2.0, 'tmq test')")
    conn.execute("insert into tb3 values (now, 3, 3.0, 'tmq test')")

def cleanup(db, topic):
    conn = get_connection()
    if conn is None:
        raise Exception("clean up failed, get connection got None")
    conn.execute("drop topic if exists {}".format(topic))
    conn.execute("drop database if exists {}".format(db))


if __name__ == '__main__':
    init_tmp_env("test", "topic1")
    consumer = Consumer(
        {
            "group.id": "tg2",
            "td.connect.user": "root",
            "td.connect.pass": "taosdata",
            "enable.auto.commit": "true",
        })
    consumer.subscribe(["topic1"])
