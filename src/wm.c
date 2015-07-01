#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

#include "wm.h"
#include "util.h"

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
  key_pair* rl = malloc(sizeof(key_pair));
  c->keys = k;
  return c;
}

int event(x_container* x, wm_config* c) {
  XNextEvent(x->dpy, &(x->ev));
  if (x->ev.type == KeyPress && x->ev.xkey.subwindow != None) {
    XKeyPressedEvent kev = x->ev.xkey;
    int xs = 10, ys = 10, xd = 0, yd = 0, ws = 10, hs = 10, wd = 0, hd = 0;
    if      (kev.keycode == get_key(x, c->keys, "left"))  xd -= xs;
    else if (kev.keycode == get_key(x, c->keys, "right")) xd += xs;
    else if (kev.keycode == get_key(x, c->keys, "down"))  yd += ys;
    else if (kev.keycode == get_key(x, c->keys, "up"))    yd -= ys;
    else if (kev.keycode == get_key(x, c->keys, "quit")) return 0;
    else return 1;
    XWindowAttributes attr;
    XGetWindowAttributes(x->dpy, kev.subwindow, &attr);
    XMoveResizeWindow(x->dpy, kev.subwindow,
      attr.x + xd, attr.y + yd, attr.width + wd, attr.height + hd
    ); 
  }
  return 1;
}

void cleanup (x_container* x, wm_config* c) {
  free(x);
  key_pair* k = c->keys,* nk = 0;
  for (; k != NULL; k = nk) {
    nk = k->next;
    free(k);
  } 
}
