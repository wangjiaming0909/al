import records;
import csv;
import os;
import pandas as pd;
import sys;
import pymysql;

pymysql.install_as_MySQLdb()


argv = sys.argv

db = records.Database('mysql://root:123456@localhost/lianjia?charset=utf8', encoding='utf-8')


if len(argv) < 2:
    print("lianjia_todb.py <folder of csv files>")

filesList = []

for root, dirs, files in os.walk(argv[1], topdown=False):
    for file in files:
        if file.find('csv') != -1:
            filesList.append(file)

for f in filesList:
    print(f)

for f in filesList:
    csv_txt = pd.read_csv(f, encoding='gb2312')
    row = next(csv_txt.iterrows())[1]
    print(row)
        # for date, district, area, desc, totalprice, floor, year, format, size, imgUrl in item:
            # print(date)

