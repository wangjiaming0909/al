import urllib3

url = 'https://hong.tianzhen-zuida.com/20191211/15884_73d2e0f1/1000k/hls/340c8d7b32'


pool = urllib3.PoolManager(1000)

for i in range(4000000, 4000999):
    u = url + str(i) + '.ts'
    print(u)
    response = pool.request('GET', u)
    status = response.status
    if status != 200 and status != 206:
        print('status is: ' + str(status))
        exit(-1)
    with open(str(i) + '.ts', 'wb') as f:
        f.write(response.data)
