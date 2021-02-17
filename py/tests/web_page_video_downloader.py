#!/bin/python3
import sys
import find_m3u8
import m3u8_downloader


if __name__ == '__main__':
    url = sys.argv[1]
    http_or_https = 'https'
    if len(sys.argv) > 2:
        http_or_https = sys.argv[2]
    m3u8s = find_m3u8.get_m3u8s(url, http_or_https)

    for m3u8 in m3u8s:
        print('download m3u8: ' + m3u8)
        m3u8_downloader.m3u8_download(m3u8)
