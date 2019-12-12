import pymysql
import records
import pymysql.cursors

connection = pymysql.Connect(host='localhost'
                            , user='root'
                            , password='123456'
                            , db='lianjia'
                            , charset='utf8mb4',
                            cursorclass=pymysql.cursors.DictCursor)

try:
    with connection.cursor() as cursor:
        sql = 'SELECT * FROM xiaoqu limit 1'
        cursor.execute(sql)
        result = cursor.fetchone()
        print(result)

    connection.commit()

finally:
    connection.close()

# pymysql.install_as_MySQLdb()


# db = records.Database('mysql://root:123456@localhost/lianjia?charset=utf8mb4', encoding='utf8')

# v = db.query('SELECT * FROM xiaoqu')