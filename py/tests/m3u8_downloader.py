#!/bin/python3
import m3u8
import aria2c_rpc
import time
import multiprocessing
from pathlib import Path

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


def find_max_prefix(s: str, p: str):
    p_len = len(p)
    if s.startswith(p):
        return p
    for i in range(1, p_len):
        tmp_prefix = p[:-i]
        if s.startswith(tmp_prefix):
            return tmp_prefix

def m3u8_download(url, thread_num = 128, timeout=5):
    global_options = aria2c_rpc.aria2c_get_global_option()
    download_dir = global_options['dir']
    path = Path(download_dir)
    playlist = None
    while True:
        try:
            playlist = m3u8.load(url, timeout=5)
        except Exception as e:
            print('failed retrying' + str(e.args))
            continue
        break

    all_urls = []
    while len(playlist.files) == 0:
        url = playlist.playlists[0].absolute_uri
        print('using absolute uri: %s' % url)
        return m3u8_download(url)
    prefix = playlist.files[0]
    for file in playlist.files:
        prefix = find_max_prefix(file, prefix)
        all_urls.append(playlist.base_uri + file)

    pool = multiprocessing.Pool(thread_num)
    pool.map(download, all_urls)
    return (path / prefix).as_posix()


if __name__ == '__main__':
    s1 = 'asdasd101'
    s2 = 'asdasd102'
    s3 = 'asdasd110'
    s4 = 'asdasd210'
    ss = [s1,s2,s3,s4]
    p = 'asdasd101'
    for s in ss:
        p = find_max_prefix(s, p)
    print(p)