#!/usr/bin/python3
import fetch
import playsound
import multiprocessing


class Player:
    def __init__(self):
        self.pro = None
        self.current_song = ''

    def do_play(self, songs: [str]):
        for song in songs:
            self.current_song = song
            print('playing {0}'.format(song))
            playsound.playsound(song)

    def play(self, names: [str]):
        if self.pro is not None and self.pro.is_alive():
            self.pro.terminate()
        self.pro = multiprocessing.Process(target=self.do_play, args=(names,))
        self.pro.start()

    def stop(self):
        if self.pro is not None and self.pro.is_alive():
            self.pro.terminate()
            print('song: {0} terminated...'.format(self.current_song))
            self.pro.join()
            self.pro = None

    def join(self):
        if self.pro is not None and self.pro.is_alive():
            self.pro.join()
if __name__ == '__main__':
    pass
