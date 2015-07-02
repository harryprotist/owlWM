#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

#include "wm.h"

wm_config* config(char* f) {
  wm_config* c = malloc(sizeof(wm_config));
  key_pair* k = malloc(sizeof(key_pair));
  k->name = "quit";
  k->key = "Q";
  key_pair* ml = malloc(sizeof(key_pair));
  ml->name = "left"; ml->key = "H"; k->next = ml;
  key_pair* mr = malloc(sizeof(key_pair));
  mr->name = "right"; mr->key = "L"; ml->next = mr;
  key_pair* md = malloc(sizeof(key_pair));
  md->name = "down"; md->key = "J"; mr->next = md;
  key_pair* mu = malloc(sizeof(key_pair));
  mu->name = "up"; mu->key = "K"; md->next = mu;
  key_pair* fk = malloc(sizeof(key_pair));
  fk->name = "next"; fk->key = "F"; mu->next = fk;
  c->keys = k;
  c->main_mod = Mod1Mask;
  c->mov_mod = Mod1Mask | ShiftMask;
  c->mut_mod = Mod1Mask | ControlMask;
  return c;
}

x_container* init(wm_config* c) {
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

  for (key_pair* k = c->keys; k != NULL; k = k->next) {
    XGrabKey(x->dpy,
      XKeysymToKeycode(x->dpy,XStringToKeysym(k->key)),
      c->main_mod, x->root, 1, GrabModeAsync, GrabModeAsync
    ); 
    XGrabKey(x->dpy,
      XKeysymToKeycode(x->dpy,XStringToKeysym(k->key)),
      c->mov_mod, x->root, 1, GrabModeAsync, GrabModeAsync
    ); 
    XGrabKey(x->dpy,
      XKeysymToKeycode(x->dpy,XStringToKeysym(k->key)),
      c->mut_mod, x->root, 1, GrabModeAsync, GrabModeAsync
    ); 
  }
  return x;
}
