#include <lcom/lcf.h>
#include "i8042.h"
#include "keyboard.h"
#include <stdint.h>

int hook_id = KEYBOARD_IRQ;
uint8_t scancode;

int (kbd_subscribe_i)(){
  int policy = IRQ_REENABLE | IRQ_EXCLUSIVE;
  sys_irqsetpolicy(KEYBOARD_IRQ, policy, &hook_id);
  return 0;
}

int (kbd_unsubscribe_i)(){
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void (kbc_ih)(){
  
  uint8_t status = 0;
  if(util_sys_inb( KBC_STATUS_REG, &status) != 0) {
    printf("Error in sys_inb\n");
    return;
  }
  if(status & OUT_BUFFER_FULL) {

    if(status & PARITY_ERROR && status & TIMEOUT_ERROR) {
      printf("TIMEOUT or PARITY ERROR\n");
      return;
    }

    if(util_sys_inb(KBC_OUTPUT_BUFFER_REG, &scancode) != 0) {
      printf("Error retrieving scancode\n");
      return;
    }

    printf("Scancode = %x\n", scancode);

  } else {
    printf("Error OUTPUT BUFFER NOT FULL\n");
    return;
  }
  
}

int (kbc_read)(){
  uint8_t status;

  while (true){
    if (util_sys_inb(KBC_STATUS_REG, &status) != OK) {
      printf("Couldn't read status from keyboard.\n");
      return 1;
    }
    if (status & OUT_BUFFER_FULL) {
      if (util_sys_inb(KBC_OUTPUT_BUFFER_REG, &scancode) != OK) {
        printf("Error util sys inb reading scancode\n");
        return 1;
      }
      if ((status & (TIMEOUT_ERROR | PARITY_ERROR)) == 0) { 
        return 0;
      }
      else
        return 1;
    }
  }
}
