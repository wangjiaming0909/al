
import sqlite3

database_name = 'musics'
database_conn = sqlite3.connect(database_name)
cur = database_conn.cursor()

def init_database():
    cur.execute('CREATE TABLE IF NOT EXISTS musics(id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR, singer VARCHAR, source VARCHAR)')
    cur.execute('CREATE TABLE IF NOT EXISTS musics_local(name varchar, singer varchar, path VARCHAR)')
    cur.execute('CREATE TABLE IF NOT EXISTS musics_like(name varchar, singer varchar, path varchar)')

def clean_database():
    database_conn.close()

def get_id(name: str, singer: str):
    pass

def is_local(name: str):
    pass

def is_local(id: int):
    pass

def get_source(name: str):
    cur.execute('select source from musics where name = ?', (name,))
    return cur.fetchall()

def _get_source(id: int):
    pass

def save_song(name: str, singer: str, source: str):
    cur.execute('select id, name from musics where name = ? and singer = ?', (name, singer))
    results = cur.fetchall()
    if len(results) > 0:
        return
    cur.execute('INSERT INTO musics (name, singer, source) values(?, ?, ?)', (name, singer, source))

def save_song_local(name: str, singer: str, path: str):
    cur.execute('select name from musics_local where name = ? and singer = ?', (name, singer))
    results = cur.fetchall()
    if len(results) > 0:
        return
    cur.execute('INSERT INTO musics_local (name, singer, path) values(?, ?, ?)', (name, singer, path))

def update_song_local(id: int, path: str):
    pass

def update_song(id: int, source: str):
    pass

def get_all_songs():
    cur.execute('select source from musics')
    return cur.fetchall()

def get_all_local_songs():
    cur.execute('select path from musics_local')
    return cur.fetchall()

def like(name):
    cur.execute('select singer, path from musics_local where name = ?', (name,))
    results = cur.fetchall()
    if len(results) <= 0:
        print("can't like a non existed song")
        return
    try:
        cur.execute('INSERT INTO musics_like (name, singer, path) VALUES(?, ?, ?)', name, results[0][0], results[0][1])
    except Exception as err:
        print('like song: {0} got error: {1}'.format(name, str(err)))


if __name__ == '__main__':
    pass
