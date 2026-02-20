#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"

void d_init();
void d_shut();
void d_refresh();
void d_clear();
void d_renderbuffer(editorstate *state);
void d_statusline(editorstate *state);
void d_cmdline(const char* buf);

#endif