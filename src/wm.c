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
  key_pair* fk = malloc(sizeof(key_pair));
  fk->name = "next"; fk->key = "F"; mu->next = fk;
  c->keys = k;
  return c;
}

int event(x_container* x, wm_config* c) {
  int ret;
  XNextEvent(x->dpy, &(x->ev));
  if (x->ev.type == KeyPress ) {
    XKeyPressedEvent kev = x->ev.xkey;
    if ((ret = handle_next(x, c, kev)) != WM_NONE) return ret;
    if ((ret = handle_move_resize(x, c, kev)) != WM_NONE) return ret;
  }
  return 1;
}

int handle_next(x_container* x, wm_config* c, XKeyPressedEvent kev) {
  if (kev.keycode == get_key(x, c->keys, "next")) {
    Window* children, root_return, parent_return, new_foc;
    int nc;
    XQueryTree(x->dpy, x->root, &root_return, &parent_return, &children, &nc);
    for (int i = 0; i < nc; i++) {
      if (children[i] == x->foc) {
        if (i == nc - 1) new_foc = children[0];
        else new_foc = children[i + 1];
        break;
      }
      if (i == nc - 1) new_foc = children[0];
    }
    x->foc = new_foc;
    XSetInputFocus(x->dpy, x->foc, RevertToParent, CurrentTime);
    XRaiseWindow(x->dpy, x->foc);

    XFree(children);
    return WM_OK;
  }
  return WM_NONE;
}
int handle_move_resize (x_container* x, wm_config* c, XKeyPressedEvent kev) {
  int s = 100, xd = 0, yd = 0, wd = 0, hd = 0;
  if      (kev.keycode == get_key(x, c->keys, "left"))  xd -= s;
  else if (kev.keycode == get_key(x, c->keys, "right")) xd += s;
  else if (kev.keycode == get_key(x, c->keys, "down"))  yd += s;
  else if (kev.keycode == get_key(x, c->keys, "up"))    yd -= s;
  else if (kev.keycode == get_key(x, c->keys, "quit")) return WM_EXIT;
  else return WM_NONE;
  if (kev.state & 0x40) {
    wd = xd;
    hd = yd; 
    xd = yd = 0;
  }
  XWindowAttributes attr;
  XGetWindowAttributes(x->dpy, x->foc, &attr);
  XMoveResizeWindow(x->dpy, x->foc,
    attr.x + xd, attr.y + yd, attr.width + wd, attr.height + hd
  ); 
  return WM_OK;
}

void cleanup (x_container* x, wm_config* c) {
  free(x);
  key_pair* k = c->keys,* nk = 0;
  for (; k != NULL; k = nk) {
    nk = k->next;
    free(k);
  } 
}
