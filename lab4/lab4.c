#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "i8042.h"

extern int mouse_counter;
extern struct packet mouse_packet;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {

  if (mouse_enable_data() != 0){
    printf("Error in mouse_enable_data_reporting\n");
    return 1;
  }

  uint8_t bit_no;
  if (mouse_subscribe_int(&bit_no) != 0){
    printf("Error in mouse_subscribe_int\n");
    return 1;
  }

  unsigned int num_packets = 0;
  int ipc_status;
  message msg;
  int r;

  while (num_packets < cnt) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 // hardware interrupt notification
          if (msg.m_notify.interrupts & bit_no) { // subscribed interrupt
            mouse_ih();
            assemble_mouse_packet();
            if(mouse_counter == 3) {
              parse_mouse_packet();
              mouse_print_packet(&mouse_packet);
              num_packets++;
            }
          }
        break;
      } 
    }
  }

  mouse_unsubscribe_int();
  mouse_disable_data();

  return 0;
}


int (mouse_test_async)(uint8_t idle_time) {

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {

  return 0;
}
