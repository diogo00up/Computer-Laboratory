#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdbool.h>
#include <stdint.h>

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

int (mouse_enable_data)();

int (mouse_disable_data)();

int (mouse_write_command)(uint8_t command);

int (mouse_write_arg)(uint8_t arg);

void (mouse_ih)();

void (assemble_mouse_packet)();

void (parse_mouse_packet)();
