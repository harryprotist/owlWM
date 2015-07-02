#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm.h"

int main(int argc, char** argv) {
  x_container* x = 0;
  wm_config* c = 0;
  setup("./config/config.json", &x, &c);
  while (1) {
    if (!event(x, c)) {
      cleanup(x, c);
      exit(0); 
    }
  }
}
