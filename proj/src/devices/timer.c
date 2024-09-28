#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id_timer = 0;
int counter = 0;

/**
 * @brief Sets the timer frequency
 * 
 *  Changing the control word sets the timer frequency
 * @param timer Timer to set frequency
 * @param freq Frequency to set the timer to
 * @return 0 upon success, else 1
 */

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (freq < 19 || freq > TIMER_FREQ) {
    return 1;
  }

  uint8_t st = 0;
  if(timer_get_conf(timer, &st) != 0){
    printf("Error in timer_get_conf");
    return 1;
  }

  uint8_t new_st = st & 0x0F;
  uint8_t ctrlWord = 0;
  switch (timer)
  {
  case 0:
    ctrlWord =  TIMER_SEL0 | TIMER_LSB_MSB | new_st;
    break;
  case 1:
    ctrlWord =  TIMER_SEL1 | TIMER_LSB_MSB | new_st;
    break;
  case 2:
    ctrlWord =  TIMER_SEL2 | TIMER_LSB_MSB | new_st;
    break;
  default:
    return 1;
    break;
  }
  
  uint16_t division = TIMER_FREQ / freq;

  uint8_t lsb = 0;
  uint8_t msb = 0;
  util_get_LSB(division, &lsb);
  util_get_MSB(division, &msb);

/*   printf("control word = %x\n", ctrlWord);
  printf("division = %x\n", division);
  printf("lsb = %x\n", lsb);
  printf("msb = %x\n", msb);
 */

  if(sys_outb(TIMER_CTRL, ctrlWord) != 0) {
    printf("sys_outb failed\n");
    return 1;   
  }

  sys_outb(TIMER_0 + timer, lsb);
  sys_outb(TIMER_0 + timer, msb); 

  return 0;
}

/**
 * @brief Subscribes timer interrupt
 * @return 0 upon success, else 1
 */

int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id_timer = 0;
  *bit_no = BIT(hook_id_timer);

  int policy = IRQ_REENABLE;
  int IRQ_line = TIMER0_IRQ;

  if (sys_irqsetpolicy(IRQ_line, policy, &hook_id_timer) != 0) {
    printf("Error in sys_irqsetpolicy()");
    return 1;
  };

  return 0;
}

/**
 * @brief Unsubscribes timer interrupt
 * @return 0 upon success, else 1
 */
int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_timer) != OK) {
    printf("Error in sys_irqrmpolicy()");
    return 1;
  };

  return 0;
}

/**
 * @brief Increments global variable
 * 
 *  Necessary to increment this variable in other files
 * 
 */
void (timer_int_handler)() {
  counter++;
}

/**
 * @brief Reads the status register
 * @param timer Timer to read the config from
 * @param st Stores the status from the timer
 * @return 0 upon success, else 1
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  uint32_t crtlWord = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_ ;
  
  if(sys_outb(TIMER_CTRL, crtlWord) != 0) {
    printf("sys_outb failed\n");
    return 1; 
  }
  
  if(util_sys_inb(TIMER_0 + timer, st) != 0)  {
    printf("util_sys_inb failed\n");
    return 1;
  }

  return 0;
}

/**
 * @brief Displays timer config
 * @param timer Timer to display the config
 * @param field timer_status_field
 * @return 0 upon success, else 1
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val conf;

  switch (field) {
    case tsf_all:
      conf.byte = st;
      break;

    case tsf_initial:
      conf.in_mode = (( st & 0x30 ) >> 4);
      break;

    case tsf_mode:
      conf.count_mode = ((st & 0x0f ) >> 1);
      if(conf.count_mode == 0x6 || conf.count_mode == 0x7) conf.count_mode &= 0x3;
      break;

    case tsf_base:
      if ((st & 0x01)) {
        conf.bcd = true;
      }
      else {
        conf.bcd = false;
      }
      break;
  }

  if (timer_print_config(timer, field, conf) != 0)
    return 1;

  return 0;
}

