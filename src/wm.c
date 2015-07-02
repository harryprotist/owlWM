#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

#include "wm.h"
#include "util.h"

int event(x_container* x, wm_config* c) {
  int ret;
  XNextEvent(x->dpy, &(x->ev));
  if (x->ev.type == KeyPress ) {
    XKeyPressedEvent kev = x->ev.xkey;
    if ((ret = handle_next(x, c, kev)) != WM_NONE) return ret;
    else if ((ret = handle_move_resize(x, c, kev)) != WM_NONE) return ret;
    else if ((ret = handle_quit(x, c, kev)) != WM_NONE) return ret;
  }
  return 1;
}

int handle_next(x_container* x, wm_config* c, XKeyPressedEvent kev) {
  if (kev.state == c->main_mod) {
    int dir = 0;
    if (kev.keycode == get_key(x, c->keys, "down")
    || kev.keycode == get_key(x, c->keys, "right")) {
      dir = 1;   
    } else if (kev.keycode == get_key(x, c->keys, "up")
    || kev.keycode == get_key(x, c->keys, "left")) {
      dir = -1;
    } else {
      return WM_NONE;
    }
    XWindowAttributes attr;
    Window* children, root_return, parent_return, new_foc = x->foc;
    int nc;
    XQueryTree(x->dpy, x->root, &root_return, &parent_return, &children, &nc);
    for (int i = 0; i < nc; i++) {
      XGetWindowAttributes(x->dpy, children[i], &attr);
      if (attr.width * attr.height <= 1) {
        if (nc > 1) children[i] = children[(i + 1) % nc];
        else return WM_OK;
      }
      new_foc = children[i];
      if (children[i] == x->foc) {
        if (dir == 1 && i == nc - 1) new_foc = children[0];
        else if (dir == -1 && i == 0) new_foc = children[nc - 1];
        else new_foc = children[i + dir];
        break;
      }
    }
    x->foc = new_foc;
    XSetInputFocus(x->dpy, x->foc, RevertToNone, CurrentTime);
    XRaiseWindow(x->dpy, x->foc);

    XFree(children);
    return WM_OK;
  }
  return WM_NONE;
}
int handle_move_resize (x_container* x, wm_config* c, XKeyPressedEvent kev) {
  if (kev.state != c->mov_mod && kev.state != c->mut_mod) return WM_NONE;
  int s = 100, xd = 0, yd = 0, wd = 0, hd = 0;
    if      (kev.keycode == get_key(x, c->keys, "left"))  xd -= s;
    else if (kev.keycode == get_key(x, c->keys, "right")) xd += s;
    else if (kev.keycode == get_key(x, c->keys, "down"))  yd += s;
    else if (kev.keycode == get_key(x, c->keys, "up"))    yd -= s;
    else return WM_NONE;
  if (kev.state == c->mut_mod) {
    wd = xd;
    hd = yd; 
    xd = yd = 0;
  }
  XWindowAttributes attr;
  XGetWindowAttributes(x->dpy, x->foc, &attr);
  if (attr.width + wd < 0 || attr.height + hd < 0)
    return WM_OK;
  XMoveResizeWindow(x->dpy, x->foc,
    attr.x + xd, attr.y + yd, attr.width + wd, attr.height + hd
  ); 
  return WM_OK;
}
int handle_quit (x_container* x, wm_config* c, XKeyPressedEvent kev) {
  if (kev.keycode == get_key(x, c->keys, "quit") && kev.state == c->main_mod)
    return WM_EXIT;
  return WM_NONE;
}

void cleanup (x_container* x, wm_config* c) {
  free(x);
  key_pair* k = c->keys,* nk = 0;
  for (; k != NULL; k = nk) {
    nk = k->next;
    free(k);
  } 
}
