#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINES 1024
#define MAX_LINE_LEN 256

typedef enum {
    MODE_NORMAL,
    MODE_INSERT,
    MODE_COMMAND,
    MODE_VISUAL
} editormode;

typedef struct {
    char *lines[MAX_LINES];
    int line_count;
    int x, y;
    int v_start_x, v_start_y;
    editormode mode;
    bool running;
    char command_buffer[256];
    char status_msg[128];
} editorstate;

#endif