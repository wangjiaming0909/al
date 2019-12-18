from urllib import request
import json
import sys
import time
#import os

if len(sys.argv) < 2:
    print('argv error')
    exit(-1)
url = sys.argv[1]

#os.spawnl(os.P_DETACH, 'aria2c/aria2c.exe', '--conf-path=aria2c.conf')

def getUrl(id, method, params):
    return json.dumps({'jsonrpc': '2.0',
                       'id': id,
                       'method': method,
                       'params': [[params]]}).encode()


def getReq(id, method, params):
    return json.dumps({'jsonrpc': '2.0',
                       'id': id,
                       'method': method,
                       'params': [params]}).encode()


id = 'jiaming'
jsonReq = getUrl(id, 'aria2.addUri', url)
print(jsonReq)

rpcUrl = 'http://localhost:6800/jsonrpc'

res = request.urlopen(rpcUrl, jsonReq)

if res.status != 200:
    print(res.status)
    exit(-1)
data = res.read()
data = json.loads(data)
result = str(data['result'])
#print(data)
print(result)

for i in range(0, 100):
    time.sleep(0.5)

    jsonReq = getReq(id, 'aria2.tellStatus', result)

    #print(jsonReq)
    res = request.urlopen(rpcUrl, jsonReq)

    if res.status != 200:
        print(res.data)
        exit(-1)
    j = json.loads(res.read())
    print('speed: ' + j['result']['downloadSpeed'])
    completed = j['result']['files'][0]['completedLength']
    total = j['result']['totalLength']
    if completed == '0' or total == '0':
        print('waiting...')
        continue
    percent = float(completed) / float(total)
    print(percent, '%')
    if(percent == 1):
        print('completed...')
        break
