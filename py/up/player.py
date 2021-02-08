#!/usr/bin/python3
import fetch
import playsound
import multiprocessing
from concurrent.futures import ThreadPoolExecutor
import pyaudio

class Player:
    def __init__(self):
        self.current_song = ''
        self.future = None
        self.pool = ThreadPoolExecutor(1)

    def do_play(self, songs: [str]):
        for song in songs:
            self.current_song = song
            print('playing {0}'.format(song))

    def play(self, names: [str]):
        self.future = self.pool.submit(self.do_play, names)

    def stop(self):
        if self.future is not None:
            self.future.cancel()

    def join(self):
        self.pool.shutdown(True)
if __name__ == '__main__':
    pass
