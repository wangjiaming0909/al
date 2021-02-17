from urllib import request
import json
import sys
import time
#import os

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

rpcUrl = 'http://localhost:6800/jsonrpc'
def aria2c_download(url, retry_times = 1):
    jsonreq = getUrl('jiaming', 'aria2.addUri', url)
    retry_times_remain = retry_times
    try:
        while True:
            id = ''
            res = request.urlopen(rpcUrl, jsonreq)
            if res.status == 200:
                print(res.status)
                data = res.read()
                data = json.loads(data)
                id = str(data['result'])
                print(id)
                break
            else:
                raise Exception('aria2c rpc error')
        return id
    except Exception as e:
        print(e.args)
        raise

def aria2c_status(id):
    jsonReq = getReq('jiaming', 'aria2.tellStatus', id)
    res = request.urlopen(rpcUrl, jsonReq)
    if res.status != 200:
        raise Exception('status failed')
    data = res.read()
    data = json.loads(data)
    return data['result']

if __name__ == '__main__':
    status = aria2c_status('ba12cf475f731509')
    print(status['status'])
    exit(0)

    aria2c_download('https://cdn-yong.bejingyongjiu.com/20210212/32719_e8b2942b/1000k/hls/index.m3u8')

    id = 'jiaming'
    jsonReq = getUrl(id, 'aria2.addUri', 'https://cdn-yong.bejingyongjiu.com/20210212/32719_e8b2942b/1000k/hls/index.m3u8')
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
