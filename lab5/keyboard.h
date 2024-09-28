#ifndef KEYBOARD_HH
#define KEYBOARD_HH

int (kbd_subscribe_i)();

int (kbd_unsubscribe_i)();

int (kbc_read)();

void (kbc_ih)();

#endif
