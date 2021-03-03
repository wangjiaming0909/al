#!/bin/python3
import xalpha as xa
import datetime
import time
from collections import OrderedDict
from pymysql.cursors import DictCursorMixin, Cursor, SSCursor
import pymysql
from dateutil.relativedelta import relativedelta
import ncurses

funds_to_get = ['F000690', 'F009644', 'F008633', 'F161903', 'F519712', 'F519736', 'F008955', 'F540010', 'F519772', 'F519773']


class OrderedDictCursor(DictCursorMixin, SSCursor):
  dict_type = OrderedDict

def get_query_result_set(query_sql, cursor):
    cursor.execute(query_sql)
    rows = cursor.fetchall()
    return rows

def get_database_conn(conn_config):
    conn = pymysql.connect(**conn_config)
    cursor = conn.cursor(OrderedDictCursor)
    get_query_result_set('select 1', cursor)
    return (conn, cursor)

conn = None
cursor = None
db_conn_config = {
   'host': '127.0.0.1',
   'user': 'root',
   'password': '',
   'port': 3305,
   'db': 'fund',
   'charset': "utf8",
   'autocommit': True
   }
(conn, cursor) = get_database_conn(db_conn_config)


class fund_rt_info:
    def __init__(self, code, current, estimate, estimate_time, up, name = ''):
        self.name = name
        self.code = code
        self.current = current
        self.estimate = estimate
        self.estimate_time = estimate_time
        self.up = up

funds_to_get = []
def get_estimated_netvalue():
    fund_rts = []
    try:
        tmp = get_query_result_set('select code from fund.fund_repo', cursor)
        funds_to_get = tmp
    except:
        pass
    #print('get rt for: ', end = ' ')
    for fund in funds_to_get:
        fund = 'F'+fund['code']
        try:
            #print(fund, end=' ', flush=True)
            ret = xa.get_rt(fund, double_check=False)
        except Exception as e:
            print(str(e.args))
            continue
        estimate = ret['estimate']
        current = ret['current']
        up = (estimate - current) / current
        name = ''
        name = ret['name']
        fund_rt = fund_rt_info(fund, current, estimate, ret['estimate_time'], up*100, name)
        fund_rts.append(fund_rt)
    #print(' ')
    return fund_rts

def update_fund_info():
    while True:
        try:
            codes = get_query_result_set('select code from fund.fund_repo where user="jiaming"', cursor)

        except Exception as e:
            print(str(e.args))

if __name__ == '__main__':
    today = None
    last_day = None
    ncurses.init_std_win()
    while True:
        now = datetime.datetime.now()
        today = datetime.datetime.today()
        if last_day is None or (last_day + relativedelta(days=+2)) == today:
            last_day = today
        if now.time() > datetime.time(12, 35) and now.time() < datetime.time(12, 55):
            print(now.time(), end=' ', flush=True)
            time.sleep(60)
            continue
        if now.time() == datetime.time(4,0):
            update_fund_info()
            continue
        if now.time() > datetime.time(15, 5) or now.time() < datetime.time(8, 55):
            print(now.time(), end=' ', flush=True)
            time.sleep(60)
            continue

        total_money_up = 0
        try:
            fund_rts = get_estimated_netvalue()
            fund_rts.sort(key=lambda x:x.up, reverse=True)
            form = ncurses.Form()
            for ret in fund_rts:
                money = get_query_result_set("select money from fund.fund_repo where code = '%s'" % ret.code[1:], cursor)
                up_money = ret.up * float(money[0]['money']) / 100
                total_money_up += up_money
                form.add_line("code:")
                form.add_row(ret.code)
                form.add_row(ret.name)
                form.add_row(ret.estimate_time)
                form.add_row(str(ret.estimate))
                form.add_row("up:")
                form.add_row(str(ret.up))
                form.add_row("money:")
                form.add_row(str(up_money))
                #s = 'code: ' + ret.code + '\t'+ret.name + '\t' + ret.estimate_time + ": " + str(ret.estimate) + '\t\tup: ' + str(ret.up) + '\t\tmoney: ' + str(up_money)
                #print(s)
            form.add_line("total:")
            form.add_row(str(total_money_up))
            form.refresh()
            #print("total: " + str(total_money_up))
            #print('------------------------')
            i = 20
            form.add_line("20")
            while i > 0:
                time.sleep(1)
                i -= 1
                form.set(len(form.lines) - 1, 0, str(i) + "            ")
                form.refresh()
        except Exception as e:
            ncurses.end_std_win()
