#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEY_ESC 0x81  //break code for escape key



#define KBC_STATUS_REG 0x64
#define KBC_INPUT_COMMANDS_REG 0x64
#define KBC_INPUT_ARGS_REG 0x60
#define KBC_OUTPUT_BUFFER_REG 0x60



#define KBC_PARITY_ERROR BIT(7)
#define KBC_TIMEOUT_ERROR BIT(6)
#define KBC_MOUSE_BIT BIT(5)
#define KBC_OUT_BUFFER_FULL BIT(0)
#define KBC_IN_BUFFER_FULL BIT(1)



#define KBC_READ_COMMAND 0x20
#define KBC_WRITE_COMMAND 0x60



#define KBC_2BYTES_SCANCODE_PREFIX 0xE0
#define KBC_MAKECODE_IDENTIFIER 0x80

#define DELAY_US    20000

#endif
