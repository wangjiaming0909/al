#!/usr/bin/python3
import http.client
import sqlite3
import urllib.parse
import ast

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
    cur.execute('CREATE TABLE IF NOT EXISTS fund_company (code VARCHAR(64) PRIMARY KEY, name VARCHAR(255))')
    cur.execute('CREATE TABLE IF NOT EXISTS fund (code VARCHAR(64) PRIMARY KEY, name_cap VARCHAR(64), name VARCHAR(255), type VARCHAR(64), name_full VARCHAR(255))')
    pass

def get_all_funds():
    o = urllib.parse.urlparse(all_fund_url)
    client = http.client.HTTPConnection(o.hostname, o.port)
    client.request('GET', o.path)
    response = client.getresponse()
    if response.status != 200:
        print('get_all_fund get error: ' + response.code)
        raise Exception('http error', response.code)
    r = response.read()
    client.close()
    funds = r.split(b' = ')
    funds = funds[-1].decode('utf-8')
    funds = funds[:-1]
    fund_arr = ast.literal_eval(funds)
    cur = sql.cursor()
    try:
        with sql:
            for fund in fund_arr:
                cur.execute('SELECT name FROM fund WHERE code = ?', (fund[0],))
                name = cur.fetchone()
                if name is None:
                    print('found new fund {0} {1}'.format(fund[0], fund[2]))
                    cur.execute('INSERT INTO fund VALUES(?,?,?,?,?)', (fund[0], fund[1], fund[2], fund[3], fund[4],))
                elif name[0] != fund[2]:
                    print('found modified fund {0} {1}'.format(fund[0], fund[2]))
                    cur.execute('INSERT INTO fund VALUES(?,?,?,?,?)', (fund[0], fund[1], fund[2], fund[3], fund[4],))
    except Exception as err:
        print('got error when get all funds')
        print(err)
        raise
    print('init funds finished..')
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
    r = response.read()
    client.close()
    comps = r.split(b'=')
    comps = comps[-1].decode('utf-8')
    op = 'op'
    comp_dict = eval(comps)
    cur = sql.cursor()
    try:
        with sql:
            for comp in comp_dict[op]:
                cur.execute("SELECT name FROM fund_company WHERE code = ?", (comp[0],))
                exist_comp = cur.fetchone()
                if exist_comp is None:
                    print('found new added company {0} {1}'.format(comp[1], comp[0]))
                    cur.execute('INSERT INTO fund_company VALUES(?, ?)', (comp[0], comp[1],))
                elif exist_comp[0] != comp[1]:
                    print('modifying company code: {0} from {1} to {2}'.format(comp[0], exist_comp, comp[1]))
                    cur.execute('INSERT INTO fund_company VALUES(?, ?)', (comp[0], comp[1],))
    except Exception as err:
        print('get_fund_companies error when INSERT INTO')
        print(err)
        raise
    print('init companies finished...')
    pass

def get_fund_real_time(name):
    pass

def init():
    init_database()
    get_fund_companies()
    get_all_funds()

print("hello")
init()
