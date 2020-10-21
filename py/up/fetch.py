#!/usr/bin/python3

import sys
import db
import subprocess





def fetch(name: str):
    pass

def fetch(name: str, author: str):
    pass


if __name__ == '__main__':
    pattern = sys.argv[1]
    pro = subprocess.run(['locate', pattern], capture_output=True)
    songs = str(pro.stdout)[2:-1]
    db.init_database()
    #print(songs)
    for line in songs.split('\\n'):
        if line == '':
            continue
        file_name_with_ext = line.split('/')[-1]
        file_name = file_name_with_ext.split('.')[0]
        #print(file_name)
        print(line)
        db.save_song_local(file_name, '', line)
    db.database_conn.commit()
    db.clean_database()

