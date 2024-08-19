
from taos import TaosConnection
import util
import threading

def fetch_data(conn: TaosConnection, sql):
    conn.execute(sql)

def show_cluster_alive(idx):
    conn = util.get_connection()
    if conn is not None:
        print('start to execute {}' % idx)
        res = conn.query("show cluster alive")
        resList = res.fetch_all()
        res.close()
        print('closed {}' % idx)
        conn.close()

if __name__ == "__main__":
    threads = []
    for i in range(0, 10):
        t = threading.Thread(target=show_cluster_alive, args=(i))
        t.start()
        threads.append(t)

    for t in threads:
        t.join()
