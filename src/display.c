#include <curses.h>
#include "include/display.h"
#include "common.h"

void d_init() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    use_default_colors();
}

void d_shut() {
    endwin();
}

void d_refresh() {
    refresh();
}

void d_clear() {
    erase();
}

void d_cmdline(const char* buf) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(max_y - 2, 0, "> %s", buf);
}

void d_renderbuffer(editorstate *state) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    for (int i = 0; i < max_y - 1; i++) {
        if (i < state->line_count) {
            mvprintw(i, 0, "%s", state->lines[i]);
        } else {
            attron(COLOR_PAIR(1));
            mvprintw(i, 0, "~");
            attroff(COLOR_PAIR(1));
        }
    }
}

void d_statusline(editorstate *state) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (state->status_msg[0] != '\0') {
        mvprintw(max_y - 2, 0, "%s", state->status_msg);
    }

    attron(A_REVERSE);
    const char* mode_str;
    switch(state->mode) {
        case MODE_NORMAL: mode_str = "NORMAL"; break;
        case MODE_INSERT: mode_str = "INSERT"; break;
        case MODE_VISUAL: mode_str = "VISUAL"; break;
        case MODE_COMMAND: mode_str = "COMMAND"; break;
        default: mode_str = "UNKNOWN"; break;
    }

    mvprintw(max_y - 1, 0, " %s | L: %d C: %d ", mode_str, state->y + 1, state->x + 1);
    
    for (int i = getcurx(stdscr); i < max_x; i++) printw(" ");
    attroff(A_REVERSE);
}