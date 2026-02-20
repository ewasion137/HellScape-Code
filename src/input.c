#include <curses.h>
#include <string.h>
#include "include/input.h"

void cursorfix(editorstate *state) {
    if (state->y >= state->line_count) state->y = state->line_count - 1;
    if (state->y < 0) state->y = 0;
    
    int len = strlen(state->lines[state->y]);
    if (state->mode == MODE_INSERT) {
        if (state->x > len) state->x = len;
    } else {
        if (len == 0) state->x = 0;
        else if (state->x >= len) state->x = len - 1;
    }
    if (state->x < 0) state->x = 0;
}

void inp_handler(editorstate *state, int ch) {
    if (state->mode != MODE_COMMAND) state->status_msg[0] = '\0';

    if (state->mode == MODE_NORMAL) {
        switch (ch) {
            case 'h': case KEY_LEFT:  state->x--; break;
            case 'j': case KEY_DOWN:  state->y++; break;
            case 'k': case KEY_UP:    state->y--; break;
            case 'l': case KEY_RIGHT: state->x++; break;
            
            case 'i': state->mode = MODE_INSERT; break;
            case 'v': 
                state->mode = MODE_VISUAL; 
                state->v_start_x = state->x;
                state->v_start_y = state->y;
                break;
            case '>': 
                state->mode = MODE_COMMAND; 
                memset(state->command_buffer, 0, 256);
                break;

            case 'x':
                if (strlen(state->lines[state->y]) > 0) {
                    char *line = state->lines[state->y];
                    memmove(&line[state->x], &line[state->x + 1], strlen(line) - state->x);
                }
                break;
                
            case 'o':
                if (state->line_count < MAX_LINES) {
                    state->y++;
                    state->x = 0;
                    state->mode = MODE_INSERT;
                    state->line_count++;
                }
                break;
        }
    } 
    else if (state->mode == MODE_INSERT) {
        if (ch == 27) {
            state->mode = MODE_NORMAL;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (state->x > 0) {
                char *line = state->lines[state->y];
                memmove(&line[state->x - 1], &line[state->x], strlen(line) - state->x + 1);
                state->x--;
            }
        } else if (ch == '\n' || ch == '\r') {
            if (state->line_count < MAX_LINES) {
                state->y++;
                state->x = 0;
                state->line_count++;
            }
        } else if (ch >= 32 && ch <= 126) {
            char *line = state->lines[state->y];
            int len = strlen(line);
            if (len < MAX_LINE_LEN - 1) {
                memmove(&line[state->x + 1], &line[state->x], len - state->x + 1);
                line[state->x] = ch;
                state->x++;
            }
        }
    }
    else if (state->mode == MODE_VISUAL) {
        if (ch == 27) {
            state->mode = MODE_NORMAL;
        }
        switch (ch) {
            case 'h': case KEY_LEFT:  state->x--; break;
            case 'j': case KEY_DOWN:  state->y++; break;
            case 'k': case KEY_UP:    state->y--; break;
            case 'l': case KEY_RIGHT: state->x++; break;
            case 'd':
                state->mode = MODE_NORMAL; 
                break;
        }
    }
    else if (state->mode == MODE_COMMAND) {
        if (ch == 27) {
            state->mode = MODE_NORMAL;
        } else if (ch == '\n' || ch == '\r') {
            if (strcmp(state->command_buffer, "q") == 0) {
                state->running = false;
            } else if (strncmp(state->command_buffer, "o ", 2) == 0) {
                snprintf(state->status_msg, 128, "Opening: %s...", state->command_buffer + 2);
            } else {
                snprintf(state->status_msg, 128, "Hell Error: Unknown command '%s'", state->command_buffer);
            }
            state->mode = MODE_NORMAL;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            int len = strlen(state->command_buffer);
            if (len > 0) state->command_buffer[len - 1] = '\0';
        } else if (ch >= 32 && ch <= 126) {
            int len = strlen(state->command_buffer);
            if (len < 255) {
                state->command_buffer[len] = ch;
                state->command_buffer[len + 1] = '\0';
            }
        }
    }

    cursorfix(state);
}