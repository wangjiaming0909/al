#!/bin/python3
import m3u8
import aria2c_rpc
import time
import multiprocessing
import sys

def download(url, retry_times = 100):
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


if __name__ == '__main__':
    #url = 'https://cdn-yong.bejingyongjiu.com/20210212/32719_e8b2942b/1000k/hls/index.m3u8'
    url = sys.argv[1]
    thread_num = 128
    if len(sys.argv) > 2:
        thread_num = int(sys.argv[2])
    playlist = None
    while True:
        try:
            playlist = m3u8.load(url)
        except:
            print('failed retrying')
            continue
        break

    all_urls = []
    for file in playlist.files:
        all_urls.append(playlist.base_uri + file)

    threads = []
    pool = multiprocessing.Pool(thread_num)
    pool.map(download, all_urls)

