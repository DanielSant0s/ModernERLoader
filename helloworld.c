/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#include <stdio.h>
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <string.h>
#include <debug.h>
#include <unistd.h>
#include "erl.h"

extern struct export_list_t {
    char * name;
    void * pointer;
} export_list[];
 
static void parse_boot_path(int argc, char *argv[]) {
    char * p;
 
    if (argc == 0) // Are people still using naplink ? :P
    strcpy(_init_erl_prefix, "host:");
 
    strcpy(_init_erl_prefix, argv[0]);
 
    p = strrchr(_init_erl_prefix, '/');
 
    if (!p)
    p = strrchr(_init_erl_prefix, '\\');
 
    if (!p)
    p = strrchr(_init_erl_prefix, ':');
 
    if (!p)
    return;
 
    *(++p) = 0;
    _init_erl_prefix[p - _init_erl_prefix] = 0;
}

typedef void (*func_t)(void);
 
int main(int argc, char *argv[]) {
    struct symbol_t * s;

    erl_add_global_symbol("printf", (u32)printf);
    erl_add_global_symbol("puts", (u32)puts);
 
    parse_boot_path(argc, argv);
    
    if (argc >= 2) {
      int i;
      for (i = 1; i < argc; i++) {
          _init_load_erl_from_file(argv[i], 0);
      }
    } else {
      _init_load_erl_from_file("main.erl", 0);
    }
  
    if ((s = erl_find_symbol("main"))) {
      printf("main found \n", 1, 2, 3, 4);
      ((func_t) s->address)();
    }

    for (int i = 0; i < 10; i++) {
      sleep(2);
    }
 
    return 0;
}

/*
int main(int argc, char *argv[])
{
  SifInitRpc(0);
  init_scr();

  scr_printf("Hello, World!\n");

  // After 5 seconds, clear the screen.
  sleep(5);
  scr_clear();

  // Move cursor to 20, 20
  scr_setXY(20, 20);
  scr_printf("Hello Again, World!\n");

  sleep(10);

  return 0;
}
*/