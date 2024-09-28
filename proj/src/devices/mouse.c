#include <lcom/lcf.h>
#include <stdint.h>
#include "mouse.h"
#include "i8042.h"
#include "keyboard.h"

int hook_id_mouse = 2;
uint8_t mouse_data;
bool mouse_reading_error = false;
uint8_t mouse_counter = 0;
bool is_first_packet = false;
uint8_t packet[3];

struct packet mouse_packet;

int (mouse_subscribe_int)(uint8_t *bit_no) {
  hook_id_mouse = 2;
  *bit_no = BIT(hook_id_mouse);
  sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
  return 0;
}

int (mouse_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id_mouse);
  return 0;
}

int (mouse_enable_data)() {
  uint8_t status;

  do {
    if(mouse_write_command(MOUSE_WRITE_BYTE_COMMAND) != 0) return 1;
    if(mouse_write_arg(MOUSE_ENABLE_DATA) != 0) return 1;
    if(util_sys_inb(KBC_OUTPUT_BUFFER_REG, &status) != 0) return 1;
  } while(status != MOUSE_ACK);
  return 0; 
}

int (mouse_disable_data)() {
  uint8_t status;

  do {
    if(mouse_write_command(MOUSE_WRITE_BYTE_COMMAND) != 0) return 1;
    if(mouse_write_arg(MOUSE_DISABLE_DATA) != 0) return 1;
    if(util_sys_inb(KBC_OUTPUT_BUFFER_REG, &status) != 0) return 1;
  } while(status != MOUSE_ACK);
  return 0; 
}

int (mouse_write_command)(uint8_t command) {
  uint8_t status;

  int tries = 10;

  while( tries > 0 ) {
    if(util_sys_inb(KBC_STATUS_REG, &status) != 0) {
      printf("Error reading kbc status register\n");
    }
    if( (status & KBC_IN_BUFFER_FULL) == 0 ) {
      sys_outb(KBC_INPUT_COMMANDS_REG, command);
      break;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  if(tries < 0) {
    printf("Failed to write to the input commands register\n");
    return 1;
  }

  return 0;
}

int (mouse_write_arg)(uint8_t arg) {
  int tries = 10;
  uint8_t status;

  while( tries > 0 ) {
    if(util_sys_inb(KBC_STATUS_REG, &status) != 0) {
      printf("Error reading kbc status register\n");
    }
    if( (status & KBC_IN_BUFFER_FULL) == 0 ) {
      sys_outb(KBC_INPUT_ARGS_REG, arg);
      break;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }

  if(tries < 0) {
    printf("Failed to write to the input Args register\n");
    return 1;
  }

  return 0;
}

void (mouse_ih)() {

  uint8_t status;
  int tries = 10;
  while(tries > 0) {

    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) {
      break;
    }

    if(status & (KBC_OUT_BUFFER_FULL | BIT(5))) {

      if (util_sys_inb(KBC_OUTPUT_BUFFER_REG, &mouse_data) != 0) { 
        break;
      }

      if((status & (KBC_TIMEOUT_ERROR | KBC_PARITY_ERROR)) == 0) {
        if (mouse_reading_error){
          mouse_counter ++;
        }
        return;
      }
      else {
        printf("Parity/TimeOut error\n");
        break;
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
    tries--;
  }
  mouse_reading_error = true;
  mouse_counter ++;
}

void (assemble_mouse_packet)() {
  if(mouse_counter == 3) mouse_counter = 0;

  if(mouse_counter == 0){
    if(mouse_data & BIT(3)){
      mouse_packet.bytes[mouse_counter] = mouse_data;
      mouse_counter++;
    }
  }else{
    mouse_packet.bytes[mouse_counter] = mouse_data;
    mouse_counter++;
  }
}

void (parse_mouse_packet)() {
  mouse_packet.lb = mouse_packet.bytes[0] & BIT(0);
  mouse_packet.rb = mouse_packet.bytes[0] & BIT(1);
  mouse_packet.mb = mouse_packet.bytes[0] & BIT(2);
  mouse_packet.delta_x = mouse_packet.bytes[1] - ((mouse_packet.bytes[0] << 4) & BIT(8));
  mouse_packet.delta_y = mouse_packet.bytes[2] - ((mouse_packet.bytes[0] << 3) & BIT(8));       
  mouse_packet.x_ov = mouse_packet.bytes[0] & BIT(6);
  mouse_packet.y_ov = mouse_packet.bytes[0] & BIT(7);
}
