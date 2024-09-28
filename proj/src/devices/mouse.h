#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief subscribe mouse interrupts
 * @param bit_no bit number
 * @return 0 upon success, else 1
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief unsubscribe mouse interrupts
 * 
 */
int (mouse_unsubscribe_int)();

/**
 * @brief enable mouse data reporting
 * @return 0 upon success, else 1
 */
int (mouse_enable_data)();

/**
 * @brief disable mouse data reporting
 * @return 0 upon success, else 1
 */
int (mouse_disable_data)();

/**
 * @brief writes commands to the kbc proper port
 * @param command Command to write
 * @return 0 upon success, else 1
 */
int (mouse_write_command)(uint8_t command);

/**
 * @brief writes arguments to the kbc proper port
 * @param arg Argument to write
 * @return 0 upon success, else 1
 */
int (mouse_write_arg)(uint8_t arg);

/**
 * @brief Deals with mouse interrupts
 * 
 */
void (mouse_ih)();

/**
 * @brief Builds the mouse packet
 * 
 */
void (assemble_mouse_packet)();

/**
 * @brief Parses the data from the mouse packet bytes
 * 
 */
void (parse_mouse_packet)();
