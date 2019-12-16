import requests
import json
import peewee
import pymysql
import datetime

totalFundSize = 2000

typeGuPiao = 1
typeHunHe = 2
typeZhaiQuan = 3

orderToday = 'td'
size = 1000
page = 1

url = 'https://danjuanapp.com/djapi/v3/filter/fund'

headers = {'Connection':'keep-alive', 
           'Host':'danjuanapp.com', 
           'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:72.0) Gecko/20100101 Firefox/72.0',
           'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
           'Accept-Language': 'en-US,zh-CN;q=0.8,zh;q=0.7,zh-TW;q=0.5,zh-HK;q=0.3,en;q=0.2'
           }

def getUrl(type, order, size, page):
    return url + '?type=' + str(type) + '&order_by=' + order + '&size=' + str(size) + '&page=' + str(page)

dbName = 'fund'

database = peewee.MySQLDatabase(
    dbName,
    host = 'localhost',
    user='root',
    port=3306,
    passwd='123456',
    charset='utf8',
    use_unicode=True)

class BaseModel(peewee.Model):
    class Meta:
        database = database

class Fund(BaseModel):
    FdCode = peewee.CharField(primary_key=True)
    FdName = peewee.CharField()
    SfType = peewee.IntegerField()

class FundHis(BaseModel):
    FdCode = peewee.CharField()
    Date = peewee.DateField(default=datetime.datetime.today)
    UnitNav = peewee.DoubleField()
    Yield = peewee.DoubleField()

    class Meta:
        primary_key = peewee.CompositeKey('FdCode', 'Date')

funds = []

hises = []



def getJiJin(type):
    u = getUrl(type, orderToday, size, page)
    r = requests.get(u, headers=headers)
    content = r.content
    content = content.decode('utf-8')
    js = json.loads(content)
    data = js['data']
    items = data['items']
    for i in range(0, len(items)):
        fd_dic = {}
        his_dic = {}
        fd_dic.update({u'FdCode': items[i]['fd_code']})
        his_dic.update({u'FdCode': fd_dic['FdCode']})
        fd_dic.update({u'FdName': items[i]['fd_name']})
        fd_dic.update({u'SfType': items[i]['sf_type']})
        his_dic.update({u'UnitNav': items[i]['unit_nav']})
        his_dic.update({u'Yield': items[i]['yield']})
        funds.append(fd_dic)
        hises.append(his_dic)


try:
    database.connect()
    database.create_tables([Fund, FundHis], safe=True)
    database.close()
    getJiJin(typeGuPiao)
    with database.atomic():
        Fund.insert_many(funds).execute()
        FundHis.insert_many(hises).execute()

except Exception as e:
    print(e)

