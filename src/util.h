#ifndef _WM_UTIL_H_
#define _WM_UTIL_H_

#include "wm.h"
#include <json.h>

void die(char* mgs);
int get_key(x_container* x, const char* name);
int get_key_name(x_container* x, json_object* job, char* key);
void grab_key(x_container* x, int mod, int key);
void get_cmd_arr(x_container* x, json_object* job, cmd_arr* cmd_arr);

#endif
