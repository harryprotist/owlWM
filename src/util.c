#include "util.h"
#include "wm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json.h>
#include <json_object.h>
#include <X11/Xlib.h>

void die(char* msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

int get_key(x_container* x, const char* key) {
  return XKeysymToKeycode(x->dpy, XStringToKeysym(key));
}

int get_key_name(x_container* x, json_object* job, char* key) {
  json_object* jv,* jk;

  if (!json_object_object_get_ex(job, "keys", &jk)) die("No field 'keys'");
  if (!json_object_object_get_ex(jk, key, &jv)) die("missing field for key");

  if (json_object_is_type(jv, json_type_string)) {
    return get_key(x, json_object_get_string(jv));
  } else {
    die("wrong type; object should contain strings");
  }
  return 0;
}

void grab_key(x_container* x, int mod, int key) {
  XGrabKey(x->dpy, key, mod, x->root, 1, GrabModeAsync, GrabModeAsync); 
}

void get_cmd_arr(x_container* x, json_object* job, cmd_arr* cmd_arr) {
  json_object* ja,* je;
  
  if (!json_object_object_get_ex(job, "commands", &ja)) die("No field 'commands'");

  cmd_arr->len = json_object_array_length(ja); 
  cmd_arr->cmds = malloc(sizeof(cmd) * cmd_arr->len);

  for (int i = 0; i < cmd_arr->len; i++) {
    je = json_object_array_get_idx(ja, i); 
    json_object* jc,* jk;
  
    if (!json_object_object_get_ex(je, "command", &jc)) die("No field 'command'");
    if (!json_object_object_get_ex(je, "key", &jk)) die("No field 'key'");
    if (
      !json_object_is_type(jc, json_type_string) ||
      !json_object_is_type(jk, json_type_string)
    ) die("invalid type in command");

    cmd_arr->cmds[i].command = json_object_get_string(jc);
    cmd_arr->cmds[i].key = get_key(x, json_object_get_string(jk));
  }
}
