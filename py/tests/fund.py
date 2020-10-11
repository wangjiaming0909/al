import http.client
import sqlite3

all_fund_url = 'http://fund.eastmoney.com/js/fundcode_search.js'
all_fund_company_url = 'http://fund.eastmoney.com/js/jjjz_gs.js?dt=1463791574015'
fund_info_url_prefix = 'http://fund.eastmoney.com/pingzhongdata/'
fund_info_url_postfix = '.js?v=20160518155842'
fund_realtime_data_url_prefix = 'http://fundgz.1234567.com.cn/js/'
fund_realtime_data_url_postfix = '.js?rt=1463558676006'


def init_database():
    sql = sqlite3.connect('d1')
    cur = sql.cursor()
    cur.execute('create table if not exists fund (c1 int)')
    pass

def get_all_funds():
    pass


print("hello")