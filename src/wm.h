#ifndef WM_H_
#define WM_H_

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

#define WM_EXIT 2
#define WM_OK 1
#define WM_NONE 0

struct key_pair {
  char* name;
  char* key;
  struct key_pair* next;
};
typedef struct key_pair key_pair;

struct wm_config {
  key_pair* keys;
  int main_mod;
  int mov_mod;
  int mut_mod;
};
typedef struct wm_config wm_config;

struct x_screen {
  int id;
  int w;
  int h;
};
typedef struct x_screen x_screen;

struct x_container {
  Display * dpy;
  Drawable draw;
  GC gc;
  Window root;
  Window foc;
  XWindowAttributes attr;
  XButtonEvent start;
  XEvent ev; 
  x_screen screen;
};
typedef struct x_container x_container;


wm_config* config(char* f);
x_container* init(wm_config* c);
int event(x_container* x, wm_config* c);
void cleanup(x_container* x, wm_config* c);

int handle_next (x_container* x, wm_config* c, XKeyPressedEvent kev);
int handle_move_resize (x_container* x, wm_config* c, XKeyPressedEvent kev);
int handle_quit (x_container* x, wm_config* c, XKeyPressedEvent kev);

#endif
