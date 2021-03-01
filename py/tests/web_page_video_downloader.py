#!/bin/python3
import sys
import find_m3u8
import m3u8_downloader
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-w', '--web', action='store', help="web page url to find m3u8")
parser.add_argument('--http', action='store_true', help='using http to fetch m3u8 url')
parser.add_argument('--https', action='store_true', help='using https to fetch m3u8 url')
parser.add_argument('-m', '--m3u8', action='store', help='m3u8 url')
parser.add_argument_group('-o', "--out", action='store', help='save file name')
args = parser.parse_args()

out_file: str = '1.mp4'
if __name__ == '__main__':
    if args.out is not None:
        global out_file
        out_file = args.out
    if args.web is not None:
        http_or_https = None
        if args.http:
            http_or_https = 'http'
        else:
            http_or_https = 'https'
        m3u8s = find_m3u8.get_m3u8s(args.web, http_or_https)

    if args.m3u8 is not None:
        m3u8s = [args.m3u8]
    for m3u8 in m3u8s:
        print('download m3u8: ' + m3u8)
        m3u8_downloader.m3u8_download(m3u8)
