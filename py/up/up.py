#!/usr/bin/python3
import db
import argparse
import player
import concurrent.futures
import os, time, sys


arg_parser = argparse.ArgumentParser()

args = arg_parser.parse_args()
pool = concurrent.futures.ThreadPoolExecutor(5)

def init():
    db.init_database()
    pass

def clean():
    db.clean_database()
    pass


init()
all_songs = db.get_all_local_songs()
p = player.Player()
songs = []
for song_arr in all_songs:
    songs.append(song_arr[0])














def interative(index):
    while 1:
        line = sys.stdin.readline()
        if line == 'f\n':
            p.stop()
            index += 1
            if index > len(songs) - 1:
                index = 0
            p.play(songs[index:])
        elif line == 'a\n':
            index -= 1
            if index < 0:
                index = len(songs) - 1
            p.stop()
            p.play(songs[index:])
        elif line == 'd\n':
            break
        elif line == 'q\n':
            p.stop()
            exit(0)
            break
    pass

if __name__ == '__main__':
    index = 0
    p.play(songs)
    try:
        interative(index)
    except Exception as err:
        print(err)
        p.stop()
    #ret = playsound.playsound(source[0])
    #playsound('/home/wjm/Music/liexin.m4a')
    #playsound.playsound('https://webfs.yun.kugou.com/202010211052/2f40172849e55b13ee3be030b9e2c217/G205/M06/02/00/DQ4DAF5wVC6AFF_FAEJlgSiNIZA279.mp3')
    clean()
