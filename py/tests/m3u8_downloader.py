#!/bin/python3
import m3u8
import aria2c_rpc
import time
import multiprocessing

def download(url, retry_times = 9999):
    remain_retry_times = retry_times
    file_name = url.split('/')[-1]
    while remain_retry_times > 0:
        id = aria2c_rpc.aria2c_download(url)
        while True:
            status = aria2c_rpc.aria2c_status(id)
            if status.get('errorCode') is not None and status['errorCode'] != '0':
                remain_retry_times -= 1
                print('task failed: ' + id + ' file: ' + file_name + ' cause: ' + status.get('errorMessage'))
                if remain_retry_times > 0:
                    print('retrying')
                else:
                    print('give up')
                break
            elif status['status'] == 'complete':
                print('task completed: ' + id + ' file: ' + file_name)
                return True
            else:
                time.sleep(1)
                continue

def m3u8_download(url, thread_num = 128):
    playlist = None
    while True:
        try:
            playlist = m3u8.load(url)
        except Exception as e:
            print('failed retrying' + str(e.args))
            continue
        break

    all_urls = []
    while len(playlist.files) == 0:
        url = playlist.playlists[0].absolute_uri
        return m3u8_download(url)
    for file in playlist.files:
        all_urls.append(playlist.base_uri + file)

    threads = []
    pool = multiprocessing.Pool(thread_num)
    pool.map(download, all_urls)

