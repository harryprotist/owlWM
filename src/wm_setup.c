#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

#include "wm.h"

x_container* init(wm_config* c) {
  x_container* x = malloc(sizeof(x_container));

  if (!(x->dpy = XOpenDisplay(0x0))) {
    return NULL;
  }
  x->screen.id = DefaultScreen(x->dpy);
  x->screen.w = DisplayWidth(x->dpy, x->screen.id);
  x->screen.h = DisplayHeight(x->dpy, x->screen.id);
  x->root = DefaultRootWindow(x->dpy);

  for (key_pair* k = c->keys; k != NULL; k = k->next) {
    XGrabKey(x->dpy,
      XKeysymToKeycode(x->dpy,XStringToKeysym(k->key)),
      Mod1Mask, x->root, 1, GrabModeAsync, GrabModeAsync
    ); 
    XGrabKey(x->dpy,
      XKeysymToKeycode(x->dpy,XStringToKeysym(k->key)),
      Mod1Mask | Mod4Mask, x->root, 1, GrabModeAsync, GrabModeAsync
    ); 
  }
  return x;
}
