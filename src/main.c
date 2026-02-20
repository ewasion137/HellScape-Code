#include "include/common.h"
#include "include/display.h"
#include "include/input.h"
#include <string.h>
#include <curses.h>

int main() {
    editorstate state;
    memset(&state, 0, sizeof(editorstate));

    state.running = true;
    state.mode = MODE_NORMAL;
    state.line_count = 1;

    for (int i = 0; i < MAX_LINES; i++) {
        state.lines[i] = malloc(MAX_LINE_LEN);
        if (state.lines[i]) memset(state.lines[i], 0, MAX_LINE_LEN);
    }
    
    strcpy(state.lines[0], "Welcome to HellScape.");

    d_init();

    while (state.running) {
        d_clear();
        
        d_renderbuffer(&state);
        d_statusline(&state);
        
        if (state.mode == MODE_COMMAND) {
            d_cmdline(state.command_buffer);
        }

        move(state.y, state.x);
        d_refresh();

        int ch = getch();
        inp_handler(&state, ch);
    }

    for (int i = 0; i < MAX_LINES; i++) free(state.lines[i]);
    d_shut();
    return 0;
}