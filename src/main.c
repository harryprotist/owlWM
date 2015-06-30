#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm.h"

int main(int argc, char** argv) {
  wm_config* c = config("doesn't matter");
  x_container* x = init(c);
  while (1) {
    if (!event(x, c)) {
      cleanup(x, c);
      exit(0); 
    }
  }
}
