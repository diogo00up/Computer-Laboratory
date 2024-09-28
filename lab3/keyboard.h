#ifndef KEYBOARD_H
#define KEYBOARD_H

int kbd_subscribe(uint8_t *bit);

int kbd_unsubscribe();

int kbc_read_out_buffer();

void kbc_restore_interrupts();

void (kbc_ih)();

void (kbc_assemble_scan_code)();
#endif
