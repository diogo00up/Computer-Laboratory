#include <lcom/lcf.h>

#include <stdint.h>

int cnt = 0;

/**
 * @brief Gets the 8 least sigficant bytes and stores them in the second parameter
 * 
 */

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  *lsb = (uint8_t) val;

  return 0;
}

/**
 * @brief @brief Gets the 8 most sigficant bytes and stores them in the second parameter
 * 
 */

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  *msb = (uint8_t) (val >> 8);

  return 0;
}

/**
 * @brief This funtion is used to call function sys_inb with an 8 byte parameter
 * 
 */

int (util_sys_inb)(int port, uint8_t *value) {
  
  uint32_t val = 0;

  if(sys_inb(port, &val) != 0) return 1;

#ifdef LAB3
  cnt++;
#endif

  *value = (uint8_t) val;
  return 0;
}
