#ifndef WM_H_
#define WM_H_

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

struct key_pair {
  char* name;
  char* key;
  struct key_pair* next;
};
typedef struct key_pair key_pair;

struct wm_config {
  key_pair* keys;
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

#endif
