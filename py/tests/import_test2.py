import urllib3
import os
import sys


header = {"Range" : "bytes=0-99"}

http = urllib3.PoolManager(headers = header)

url = sys.argv[1]

# r = http.request('GET', "https://github.com/wangjiaming0909/unp/archive/master.zip")
# r = http.request('GET', "http://d1.music.126.net/dmusic/netease-cloud-music_1.2.1_amd64_ubuntu_20190428.deb")
# r = http.request('GET', "https://dldir1.qq.com/weixin/Windows/WeChat_C1018.exe")

r = http.request('GET', url)

if r.status != 200 and r.status != 206:
    print(r.status)
    exit()

fileName = 'defaultName'
if r.headers.get('Content-Disposition'):
    contentDispositionHeader = r.headers['Content-Disposition']
    nameStartIndex = contentDispositionHeader.find('=') + 1
    nameEndIndex = len(contentDispositionHeader)
    if contentDispositionHeader[nameStartIndex] == '\'' or contentDispositionHeader[nameStartIndex] == '"':
        nameStartIndex += 1
    if contentDispositionHeader[nameEndIndex-1] == '\'' or contentDispositionHeader[nameEndIndex-1] == '"':
        nameEndIndex -= 1
    fileName = contentDispositionHeader[nameStartIndex:nameEndIndex]

print(fileName)
print(r.headers)
print('--------------------')
# print(r.data)
print(len(r.data))
r.data

if os.path.exists(fileName):
    os.remove(fileName)

f = open(fileName, 'wb')
f.write(r.data)
f.close()

