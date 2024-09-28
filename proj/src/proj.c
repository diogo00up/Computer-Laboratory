// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// Any header files included below this line should have been created by you
#include "game.h"
#include <devices/graphicscard.h>
#include <img/pacman.xpm>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/**
 * @brief Here we call the main functionalities implemented
 * 
 */

int(proj_main_loop)(int argc, char *argv[]) {

  if (initialize_values() != 0) {vg_exit(); return 1;}

  if (game_loop() != 0) {return 1;}

  if (close_game() != 0) {return 1;}
  
  return 0;
}
