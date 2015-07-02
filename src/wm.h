#ifndef WM_H_
#define WM_H_

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

#define WM_EXIT 2
#define WM_OK 1
#define WM_NONE 0

typedef struct cmd {
  const char* command;
  int key;
} cmd;
typedef struct cmd_arr {
  int len;
  cmd* cmds;
} cmd_arr;

typedef struct wm_config {
  int main_mod;
  int mov_mod;
  int mut_mod;
  int k_quit;
  int k_left;
  int k_right;
  int k_down;
  int k_up;
  cmd_arr cmd_arr;
} wm_config;

typedef struct x_screen {
  int id;
  int w;
  int h;
} x_screen;

typedef struct x_container {
  Display * dpy;
  Drawable draw;
  GC gc;
  Window root;
  Window foc;
  XWindowAttributes attr;
  XButtonEvent start;
  XEvent ev; 
  x_screen screen;
} x_container;

wm_config* config(x_container* x, char* f);
x_container* init();
int event(x_container* x, wm_config* c);
void cleanup(x_container* x, wm_config* c);
void setup(char* f, x_container** x, wm_config** c);
void grab_keys(x_container* x, wm_config* c);

int handle_next (x_container* x, wm_config* c, XKeyPressedEvent kev);
int handle_move_resize (x_container* x, wm_config* c, XKeyPressedEvent kev);
int handle_quit (x_container* x, wm_config* c, XKeyPressedEvent kev);
int handle_command (x_container* x, wm_config* c, XKeyPressedEvent kev);

#endif
