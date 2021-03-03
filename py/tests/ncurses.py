import curses

class Ele:
    def __init__(self, y:int, x:int, content:str):
        self.x = x
        self.y = y
        self.content = content
        self.attr = None

stdw = None

class Form:
    def __init__(self):
        self.eles_list = []
        self.lines = []
        self.line_length = []
        self.attrs = {}

    def add_line(self, s: str, attr=None):
        new_added_line = []
        new_added_line.append(s)
        self.line_length.append(len(s))
        self.lines.append(new_added_line)
        cur_line_num = len(self.lines) - 1
        cur_row_num = 0
        self.attrs[cur_line_num] = {}
        self.attrs[cur_line_num][cur_row_num] = attr

    def add_row(self, s: str,line = -1, attr = None):
        if line == -1:
            line = len(self.lines) - 1
        cur_line = self.lines[line]
        self.line_length[line] += len(s)
        cur_line.append(s)
        cur_line_num = line
        cur_row_num = len(cur_line) - 1
        self.attrs[cur_line_num][cur_row_num] = attr

    def set(self, line, col, s):
        self.lines[line][col] = s

    def refresh(self, win=None):
        global stdw
        if win == None:
            win = stdw
        total_cols = curses.COLS
        total_lines = curses.LINES
        cur_y = 0
        for line in self.lines:
            cur_x = 0
            for col in line:
                if cur_x > total_cols or cur_y > total_lines:
                    continue
                win.addstr(cur_y, cur_x, col)
                cur_x += len(col) + 6
            cur_y += 1
        win.refresh()

def init_std_win():
    global stdw
    stdw = curses.initscr()
    curses.cbreak()
    stdw.refresh()
    #curses.raw()

def end_std_win():
    curses.endwin()

def refresh_win(w, eles: list):
    for e in eles:
        if e.attr:
            w.addstr(e.y, e.x, e.content, attr=e.attr)
        else:
            w.addstr(e.y, e.x, e.content)

