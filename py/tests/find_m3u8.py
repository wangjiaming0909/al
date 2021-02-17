#!/bin/python3

from urllib import request
import sys
import re

def find_m3u8_in_html(contents):
    results = []
    start_pos = 0
    while True:
        r = re.search(r'//[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]\.m3u8', contents[start_pos:])
        if r is None:
            break
        results.append(r.group())
        start_pos = r.span()[1]
    return results


def get_contents(url):
    req = request.urlopen(url)
    if req.status != 200:
        raise Exception('url open failed')
    return str(req.read())

def get_m3u8s(url, http_or_https = 'http'):
    http_or_https += ':'
    contents = get_contents(url)
    m3u8s = find_m3u8_in_html(contents)
    full_urls = []
    for m3u8 in m3u8s:
        full_urls.append(http_or_https + m3u8)
    return full_urls

if __name__ == '__main__':
    contents = '//asd.asd.m3u8 //asdasddas.asdasa.m3u8'
    gs = find_m3u8_in_html(contents)
    print(gs)
