#!/bin/python3
# -*- coding: utf-8 -*-
import pymysql
from collections import OrderedDict
from pymysql.cursors import DictCursorMixin, Cursor, SSCursor
from datetime import datetime
import csv
import argparse

mysql_host = '172.16.70.234'
mysql_port = 3305
mysql_user = 'root'
mysql_pswd = ''

parser = argparse.ArgumentParser()
parser.add_argument('--xls', help='xls file name')
args = parser.parse_args()

def fatal_error(mes):
    pass

if args.xls is None:
    fatal_error('must give xls file name')

class OrderedDictCursor(DictCursorMixin, SSCursor):
  dict_type = OrderedDict

def get_mysql_connection():
    db_conn_config = {
        'host': mysql_host,
        'user': mysql_user,
        'password': mysql_pswd,
        'port': int(mysql_port),
        'db': 'fund',
        'charset': "utf8",
        'autocommit': True
    }
    conn = pymysql.connect(**db_conn_config)
    cursor = conn.cursor(OrderedDictCursor)
    return cursor

class Fund:
    def __init__(self, name, code):
        self.name = name
        self.code = code
        self.ins = None
        self.outs = None
        self.growth = 0

    def add_in(self, inin):
        self.ins = inin
    
    def add_out(self, out):
        self.outs = out

class In:
    def __init__(self, date, how_much, noise):
        self.date = datetime.strptime(date, "%Y%m%d").strftime("%Y-%m-%d")
        self.how_much = how_much
        self.noise = noise

class Out:
    def __init__(self, date, noise):
        self.date = datetime.strptime(date, "%Y%m%d").strftime("%Y-%m-%d")
        self.noise = noise

funds_to_calc = {}

columns_to_read = ['基金代码', '基金名称', '交易类型', '确认日期', '确认份额', '交易金额', '手续费']
columns_index = [0,0,0,0,0,0,0]

all_funds = {}

def read_xls(file_name):
    global all_funds
    global columns_index
    global columns_to_read

    with open(file_name) as f:
        f_csv = csv.reader(f)
        getting_index = True
        for row in f_csv:
            if getting_index:
                code_index = row.index(columns_to_read[0])
                if code_index == -1:
                    continue
                else:
                    for i in range(0, len(columns_to_read)):
                        columns_index[i] = row.index(columns_to_read[i])
                    getting_index = False
            else:
                code = row[columns_index[0]]
                name = row[columns_index[1]]
                if all_funds.get(code) is not None:
                    fund = all_funds[code]
                else:
                    fund = Fund(name, code)
                type = row[columns_index[2]]
                date = row[columns_index[3]]
                number = row[columns_index[4]]
                how_much = row[columns_index[5]]
                noise = row[columns_index[6]]
                if type == '认购确认':
                    inin = In(date, how_much, noise)
                    fund.add_in(inin)
                    pass
                elif type == '申购':
                    inin = In(date, how_much, noise)
                    fund.add_in(inin)
                    pass
                elif type == '赎回':
                    out = Out(date, noise)
                    fund.add_out(out)
                    pass
                else:
                    continue
                all_funds[fund.code] = fund


def calc():
    conn = get_mysql_connection()
    for code, f in all_funds.items():
        if f.ins is None or f.outs is None:
            continue
        sql = "select sum(fund.fund_growth.growth) from fund.funds join fund.fund_growth on fund.funds.code = fund.fund_growth.code where fund.funds.code = '%s' and day >= '%s' and day <= '%s'" % (code, f.ins.date, f.outs.date)
        conn.execute(sql)
        rows = conn.fetchall()
        f.growth = float(rows[0]['sum(fund.fund_growth.growth)'])

read_xls(args.xls)
calc()
for code, f in all_funds.items():
    print('code: %s name: %s growth: %s' % (code, f.name, f.growth))