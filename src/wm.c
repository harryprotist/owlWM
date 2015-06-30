#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

#include "wm.h"

wm_config* config(char* f) {
  wm_config* c = malloc(sizeof(wm_config));
  key_pair* k = malloc(sizeof(key_pair));
  k->name = "quit";
  k->key = "q";
  c->keys = k;
  return c;
}

x_container* init(wm_config* c) {
  x_container* x = malloc(sizeof(x_container));

  if (!(x->dpy = XOpenDisplay(0x0))) {
    return NULL;
  }
  x->root = DefaultRootWindow(x->dpy);

  for (key_pair* k = c->keys; k != NULL; k = k->next) {
    XGrabKey(x->dpy,
      XKeysymToKeycode(x->dpy,XStringToKeysym(k->key)),
      Mod1Mask,
      x->root,
      1,
      GrabModeAsync,
      GrabModeAsync
    ); 
  }
  return x;
}

int event(x_container* x, wm_config* c) {
  XNextEvent(x->dpy, &(x->ev));
  printf("woopwoopwoop\n");
}

void cleanup (x_container* x, wm_config* c) {
  free(x);
  key_pair* k = c->keys,* nk = 0;
  for (; k != NULL; k = nk) {
    nk = k->next;
    free(k);
  } 
}
