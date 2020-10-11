#!/usr/bin/python3
import http.client
import sqlite3
import urllib.parse

all_fund_url = 'http://fund.eastmoney.com/js/fundcode_search.js'
all_fund_company_url = 'http://fund.eastmoney.com/js/jjjz_gs.js?dt=1463791574015'
fund_info_url_prefix = 'http://fund.eastmoney.com/pingzhongdata/'
fund_info_url_postfix = '.js?v=20160518155842'
fund_realtime_data_url_prefix = 'http://fundgz.1234567.com.cn/js/'
fund_realtime_data_url_postfix = '.js?rt=1463558676006'


database_name = '../d1'
sql = sqlite3.connect(database_name)

def init_database():
    cur = sql.cursor()
    cur.execute('CREATE TABLE IF NOT EXISTS fund_company (code INT PRIMARY KEY, name VARCHAR(255))')
    pass

def get_all_funds():
    pass

def get_fund(name):
    pass

def get_fund_companies():
    o = urllib.parse.urlparse(all_fund_company_url)
    client = http.client.HTTPConnection(o.hostname, o.port)
    client.request('GET', o.path)
    response = client.getresponse()
    if response.code != 200:
        print('get_fund_companies get error: ' + response.code)
        raise Exception('http error', response.code)
    #print(response.getheaders())
    r = response.read()
    comps = r.split(b'=')
    comps = comps[-1].decode('utf-8')
    #print(comps)
    op = 'op'
    comp_dict = eval(comps)
    cur = sql.cursor()
    try:
        with sql:
            for comp in comp_dict[op]:
                print(comp)
                cur.execute('SELECT COUNT(*) FROM fund_company WHERE code = ?', int(comp[0]))
                print(cur.fetchone()[0])
                cur.execute('INSERT INTO fund_company VALUES(?, ?)', int(comp[0]), comp_dict[1])
    except Exception as err:
        print('get_fund_companies error when REPLACE INTO')
        print(err)
    pass

def get_fund_real_time(name):
    pass



print("hello")
init_database()
get_fund_companies()
