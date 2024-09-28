#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * @brief Subscribes the keyboard interrupt
 * 
 * @param bit Bit number
 * @return Returns 0 on success and others on failure 
 */
int kbd_subscribe(uint8_t *bit);


/**
 * @brief Unsubscribes the keyboard interrupts
 * 
 * @return Returns 0 on success, else 1
 */
int kbd_unsubscribe();

/**
 * @brief Reads the output buffer
 * @return 0 upon success, else 1
 */
int kbc_read_out_buffer();

/**
 * @brief restores the interrupts from the keyboard
 * 
 */
void kbc_restore_interrupts();

/**
 * @brief Keyboard interrupt handler
 * 
 * Reads the status register and the scancode and checks the output buffer
 * 
 */
void (kbc_ih)();

/**
 * @brief Assembles the scancode, dealing with 1 and 2 bytes scancodes
 * 
 */
void (kbc_assemble_scan_code)();
#endif
