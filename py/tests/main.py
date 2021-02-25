#!/bin/python3


import curses
import time

class Ele:
    def __init__(self, y:int, x:int, content:str):
        self.x = x
        self.y = y
        self.content = content
        self.attr = None

def refresh_win(w, eles: list):
    for e in eles:
        if e.attr:
            w.addstr(e.y, e.x, e.content, attr=e.attr)
        else:
            w.addstr(e.y, e.x, e.content)

stdw = curses.initscr()
curses.cbreak()
curses.raw()
e1 = Ele(1, 0, "100100100100\t100100100100\n")
e2 = Ele(2, 0, "200200200200\t200200200\n")
eles = [e1,e2]
try:
    c = '1'
    while c != 'q':
        line = "cols: " + str(curses.COLS) + " rows: " + str(curses.LINES)
        ele_col_line = Ele(0,0,line)
        eles.append(ele_col_line)
        refresh_win(stdw, eles)
        c = stdw.getch()
except Exception as e:
    print(str(e.args))
finally:
    curses.endwin()

