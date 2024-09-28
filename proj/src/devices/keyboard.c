#include <lcom/lcf.h>
#include "i8042.h"
#include "keyboard.h"
#include <stdint.h>

int hook_id_kbd = 1;
uint8_t scancode = 0;
uint8_t previousScanCode = 0;

int kbd_subscribe(uint8_t *bit){
  hook_id_kbd = 1;
  *bit = BIT(hook_id_kbd);
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd);
}

int kbd_unsubscribe(){
  return sys_irqrmpolicy(&hook_id_kbd);
}

void (kbc_ih)(){
  uint8_t status = 0;

  if(util_sys_inb(KBC_STATUS_REG, &status) != 0) {
    printf("Error reading kbc status register\n");
    return;
  }

  if(status & KBC_OUT_BUFFER_FULL) {
    if(util_sys_inb(KBC_OUTPUT_BUFFER_REG, &scancode) != 0) {
      printf("Error reading scan code\n");
      return;
    }
    if((status & (KBC_TIMEOUT_ERROR | KBC_PARITY_ERROR)) == 0) {
      return;
    } else {
      printf("Error reading the scan code\n");
    }
  } else {
    printf("Error output buffer not full\n");
    return;
  }

}

void (kbc_assemble_scan_code)(){
  bool make = false;
  uint8_t bytes[2];
  int size = 1;


  if(scancode == KBC_2BYTES_SCANCODE_PREFIX) {
    previousScanCode = scancode;
    return;
  }

  if(previousScanCode != 0) {
    size = 2;
    bytes[0] = previousScanCode;
    bytes[1] = scancode;
  } else {
    size = 1;
    bytes[0] = scancode;
  }

  previousScanCode = 0;

  if((scancode & KBC_MAKECODE_IDENTIFIER) == 0) {
    make = true;
  }
  
  kbd_print_scancode(make, size, bytes);
}

int (kbc_read_out_buffer)(){
  uint8_t status;

  while (true){
    if (util_sys_inb(KBC_STATUS_REG, &status) != OK) {
      printf("Error reading kbc status register\n");
      return 1;
    }
    if (status & KBC_OUT_BUFFER_FULL) {
      if (util_sys_inb(KBC_OUTPUT_BUFFER_REG, &scancode) != OK) {
        printf("Error reading scancode\n");
        return 1;
      }
      if ((status & (KBC_TIMEOUT_ERROR | KBC_PARITY_ERROR)) == 0) { 
        return 0;
      }
      else
        return 1;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}

void kbc_restore_interrupts() {
  uint8_t status;

  int tries = 10;

  //write read command
  while( tries > 0 ) {
    if(util_sys_inb(KBC_STATUS_REG, &status) != 0) {
      printf("Error reading kbc status register\n");
    }
    if( (status & KBC_IN_BUFFER_FULL) == 0 ) {
      sys_outb(KBC_INPUT_COMMANDS_REG, KBC_READ_COMMAND);
      break;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  if(tries < 0) {
    printf("Failed to write to the input commands register\n");
    return;
  }

  kbc_read_out_buffer();

  tries = 10;

  //write write command
  while( tries > 0 ) {
    if(util_sys_inb(KBC_STATUS_REG, &status) != 0) {
      printf("Error reading kbc status register\n");
    }
    if( (status & KBC_IN_BUFFER_FULL) == 0 ) {
      sys_outb(KBC_INPUT_COMMANDS_REG, KBC_WRITE_COMMAND);
      break;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  if(tries < 0) {
    printf("Failed to write to the input commands register\n");
    return;
  }


  tries = 10;

  //write arguments
  while( tries > 0 ) {
    if(util_sys_inb(KBC_STATUS_REG, &status) != 0) {
      printf("Error reading kbc status register\n");
    }
    if( (status & KBC_IN_BUFFER_FULL) == 0 ) {
      sys_outb(KBC_INPUT_ARGS_REG, scancode|BIT(0));
      break;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  if(tries < 0) {
    printf("Failed to write to the input Args register\n");
    return;
  }
}
