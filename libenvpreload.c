#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

__attribute__((constructor))
void init(void){

  FILE* f = fopen("/etc/X11/environment", "r");
  if(!f){
    perror("Failed to open /etc/X11/environment");
    return;
  }

  // parse and set the env
  char buf[4096];
  bool skip_overlong_line = false;
  while(fgets(buf, sizeof(buf), f)){
    // get next line and check that we actually got a whole one, discard too long ones
    size_t len = strlen(buf);
    if(!len) continue;
    if(buf[len-1] != '\n'){
      skip_overlong_line = true;
      continue;
    }
    if(skip_overlong_line){
      skip_overlong_line = false;
      continue;
    }
    // parse next line
    { char* it=strchr(buf, '#'); if(it) *it=0; } // Remove comments. Don't use strtok!
    char* key = strtok(buf, "=");
    char* value = strtok(0, "");
    if(!key || !value)
      continue;
    // Let's truncate spaces in key and value
    while(isspace(*key)) key++;
    for(char* it=key; *it; it++) if(isspace(*it)) *(it--) = 0;
    while(isspace(*value)) value++;
    for(char* it=value; *it; it++) if(isspace(*it)) *(it--) = 0;
    if(!*key || !*value)
      continue;
    // set env
    setenv(key, value, true);
  }

  // cleanup
  fclose(f);
  // clear any leftover internal settok state
  strtok((char[]){""}, "");
  strtok(NULL, "");
}
