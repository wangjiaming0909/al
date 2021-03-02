#!/bin/python3
import multiprocessing
import sys
import find_m3u8
import m3u8_downloader
import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument('-w', '--web', action='store', help="web page url to find m3u8")
parser.add_argument('--http', action='store_true', help='using http to fetch m3u8 url')
parser.add_argument('--https', action='store_true', help='using https to fetch m3u8 url')
parser.add_argument('-m', '--m3u8', action='store', help='m3u8 url')
parser.add_argument('-o', "--out", action='store', help='save file name')
parser.add_argument('--timeout', action='store', help='load m3u8 timeout in seconds')
parser.add_argument('-t', '--thread', action='store', help='download thread num')
args = parser.parse_args()

out_file = '1.mp4'
thread_num = 128
if args.out is not None:
    out_file = args.out

if args.thread is not None:
    thread_num = args.thread

def cat_to_out_file(prefix):
    os.system('cat %s* > %s' % (prefix, out_file))

if __name__ == '__main__':
    if args.web is not None:
        http_or_https = None
        if args.http:
            http_or_https = 'http'
        else:
            http_or_https = 'https'
        m3u8s = find_m3u8.get_m3u8s(args.web, http_or_https)

    if args.m3u8 is not None:
        m3u8s = [args.m3u8]

    download_file_prefix = ''
    for m3u8 in m3u8s:
        print('download m3u8: ' + m3u8)
        if args.timeout is not None:
            download_file_prefix = m3u8_downloader.m3u8_download(m3u8, thread_num, timeout=args.timeout)
        else:
            download_file_prefix = m3u8_downloader.m3u8_download(m3u8, thread_num)

    cat_to_out_file(download_file_prefix)


if __name__ == '__main__1':
    cat_to_out_file('D://0')
