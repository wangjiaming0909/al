import pymysql
import pymysql.cursors
from pymysql.connections import Connection
import json

if __name__ != '__main__':
    pass

DATABASE_NAME = 'house'
DATABASE_CONF_FILE = 'mysql.conf'


class DBConf(object):
    def __init__(self, host, user, password, charset):
        self.host = host
        self.user = user
        self.password = password
        self.charset = charset


DataBaseConf = None


def parseDbConf():
    with open(DATABASE_CONF_FILE) as f:
        content = f.read()
        decoder = json.decoder.JSONDecoder()
        conf = decoder.decode(content)
         # = DBConf(['host'], conf['user'], conf['password'], conf['charset'])


parseDbConf()
print(DataBaseConf.host)
print(DataBaseConf.user)
print(DataBaseConf.password)
print(DataBaseConf.charset)
exit(0)


def buildDB():
    try:
        conn: Connection = pymysql.Connect(host='localhost',
                                           user='root',
                                           password='123456',
                                           charset='utf8mb4',
                                           cursorclass=pymysql.cursors.DictCursor)
        with conn.cursor() as cursor:
            createDbSql: str = 'CREATE DATABASE ' + DATABASE_NAME
            cursor.execute(createDbSql)
        conn.commit()
    except Exception as exc:
        print('creating database error: ' + exc.__str__())
        exit(-1)

    finally:
        conn.close()


try:
    connection = pymysql.Connect(host='localhost',
                                 user='root',
                                 password='123456',
                                 db=DATABASE_NAME,
                                 charset='utf8mb4',
                                 cursorclass=pymysql.cursors.DictCursor)

except pymysql.err.InternalError as e:
    if e.args[0] == 1049:
        print('first time to setup environment, creating Database...')
        buildDB()

finally:
    connection.close()
