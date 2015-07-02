#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <json.h>

#include "wm.h"
#include "util.h"

void setup(char* f, x_container** x, wm_config** c) {
  *x = init();
  *c = config(*x, f);
  grab_keys(*x, *c); 
}

wm_config* config(x_container* x, char* f) {
  char* cont = 0;
  FILE* fi = 0;
  if ((fi = fopen(f, "rb"))) {
    int l;
    fseek(fi, 0, SEEK_END);
    l = ftell(fi);
    fseek(fi, 0, SEEK_SET);
    cont = malloc(l);
    if (cont) fread(cont, 1, l, fi);
    fclose(fi);
  } else die("could not open config");
  json_object* job = json_tokener_parse(cont);
  if (!job) die("could not parse config file");

  wm_config* c = malloc(sizeof(wm_config));
  c->k_quit = get_key_name(x, job, "quit");
  c->k_left = get_key_name(x, job, "left");
  c->k_right = get_key_name(x, job, "right");
  c->k_down = get_key_name(x, job, "down");
  c->k_up = get_key_name(x, job, "up");
  c->main_mod = Mod1Mask;
  c->mov_mod = Mod1Mask | ShiftMask;
  c->mut_mod = Mod1Mask | ControlMask;
  get_cmd_arr(x, job, &(c->cmd_arr));

  free(job);
  free(cont);
  return c;
}

x_container* init() {
  x_container* x = malloc(sizeof(x_container));

  if (!(x->dpy = XOpenDisplay(0x0))) {
    return NULL;
  }
  x->screen.id = DefaultScreen(x->dpy);
  x->screen.w = DisplayWidth(x->dpy, x->screen.id);
  x->screen.h = DisplayHeight(x->dpy, x->screen.id);
  x->root = DefaultRootWindow(x->dpy);

  XSetInputFocus(x->dpy, x->root, RevertToNone, CurrentTime);
  x->foc = x->root;

  return x;
}

void grab_keys(x_container* x, wm_config* c) {
  for (int i = 0; i < 3; i++ ) {
    int mod = (i == 0)? (c->main_mod):((i == 1)? (c->mut_mod):(c->mov_mod));
    grab_key(x, mod, c->k_left);
    grab_key(x, mod, c->k_right);
    grab_key(x, mod, c->k_up);
    grab_key(x, mod, c->k_down);
  }
  grab_key(x, c->main_mod, c->k_quit);
  for (int i = 0; i < c->cmd_arr.len; i++) {
    grab_key(x, c->main_mod, c->cmd_arr.cmds[i].key);
  }
}
