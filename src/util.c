#include "util.h"
#include "wm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int get_key(x_container* x, key_pair* keys, char* name) {
  for (key_pair* kp = keys; kp != NULL; kp = kp->next) {
    if (!strcmp(kp->name, name))
      return XKeysymToKeycode(x->dpy, XStringToKeysym(kp->key));
  }
}
