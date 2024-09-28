#include "adresses.h"
//#include "graphicscard.h"
#include <lcom/lcf.h>
#include <stdbool.h>
// IMPORTANT: you must include the following line in all your C files
#include "i8042.h"
#include "keyboard.h"
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include <graphicscard.h>
#include <timer.c>
#include <sprite.h>




// Any header files included below this line should have been created by you




int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
extern char *buffer;
extern char *video_mem; /* frame-buffer VM address */
extern vbe_mode_info_t vmi_p;
extern uint8_t scancode;
extern int counter;

int(video_test_init)(uint16_t mode, uint8_t delay) {

  vg_init(mode);
  sleep(delay);
  vg_exit();

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  vg_init(mode);
  vg_draw_rectangle(	x, y, width,height,color);
  from_buffer();
  sleep(5);
  vg_exit();
  return 0;

}


int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  vg_init(mode);
  uint32_t R_first;
  uint32_t G_first;
  uint32_t B_first;
  switch (mode) {
    case 0x105:
      break;
    case 0x110:      
      R_first = (first & 0x00007C00) >> vmi_p.RedFieldPosition;
      G_first = (first & 0x000003E0) >> vmi_p.GreenFieldPosition;
      B_first = (first & 0x0000001F) >> vmi_p.BlueFieldPosition;

    case 0x115:
      R_first = (first & 0x00FF0000) >> vmi_p.RedFieldPosition;
      G_first = (first & 0x0000FF00) >> vmi_p.GreenFieldPosition;
      B_first = (first & 0x000000FF) >> vmi_p.BlueFieldPosition;

      break;
    case 0x11A:
      R_first = (first & 0x0000F800) >> vmi_p.RedFieldPosition;
      G_first = (first & 0x000007E0) >> vmi_p.GreenFieldPosition;
      B_first = (first & 0x0000001F) >> vmi_p.BlueFieldPosition;
      break;
    case 0x14C:
      R_first = (first & 0x00FF0000) >> vmi_p.RedFieldPosition;
      G_first = (first & 0x0000FF00) >> vmi_p.GreenFieldPosition;
      B_first = (first & 0x000000FF) >> vmi_p.BlueFieldPosition;
      break;
  }

  int rectangle_x_size = vmi_p.XResolution / no_rectangles;
  int rectangle_y_size = vmi_p.YResolution / no_rectangles;
  int R;
  int G;
  int B;
  uint32_t color;
  

  for (int i = 0; i < no_rectangles; i++) {
    for (int i2 = 0; i2 < no_rectangles; i2++) {
      if (mode == 0x105) {
        color = (first + (i * no_rectangles + i2) * step) % (1 << vmi_p.BitsPerPixel);
      }
      else {
        R = (R_first + i2 * step) % (1 << vmi_p.RedMaskSize);
        G = (G_first + i * step) % (1 << vmi_p.GreenMaskSize);
        B = (B_first + (i2 + i) * step) % (1 << vmi_p.BlueMaskSize);
        color = 0 | (R << vmi_p.RedFieldPosition) | (G << vmi_p.GreenFieldPosition) | (B << vmi_p.BlueFieldPosition);
      }
    
     // vg_draw_rectangle(i * rectangle_x_size, i2 * rectangle_y_size, rectangle_x_size, rectangle_y_size, color);

      vg_draw_rectangle(i * rectangle_x_size,  i2 * rectangle_y_size,  rectangle_x_size,rectangle_y_size,color);
      from_buffer();

      
    }
  }

  kbd_subscribe_i();
  int irq_set = BIT(KEYBOARD_IRQ);

  int ipc_status;
  int r;
  message msg;
  while (scancode != KEY_ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
          }
          break;
        default:
          break;
      }
    }
  }

  kbd_unsubscribe_i();

  vg_exit();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////





int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {


  vg_init(0x105);
  xpm_image_t img;
  uint8_t *map = xpm_load(spaceInvaders, XPM_INDEXED, &img);
  vg_draw_sprite(map, img, x, y);
  from_buffer(); 
  


  kbd_subscribe_i();
  int irq_set = BIT(KEYBOARD_IRQ);

  int ipc_status;
  int r;
  message msg;
  while (scancode != KEY_ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
          }
          break;
        default:
          break;
      }
    }
  }

  kbd_unsubscribe_i();
  vg_exit();
  return 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {


  timer_set_frequency(0,fr_rate);
  
  uint32_t irq_set_2 = BIT(TIMER0_IRQ);

  if(timer_subscribe_int(TIMER0_IRQ) != 0) {
    printf("timer_subscribe error\n");
  }




  vg_init(0x105);
  xpm_image_t img;
  uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);
  vg_draw_rectangle(xi, yi, img.width, img.height, 0);
  from_buffer();

  vg_draw_sprite(map, img, xi, yi);
  from_buffer();

  kbd_subscribe_i();
  int irq_set = BIT(KEYBOARD_IRQ);

  int ipc_status;
  int r;
  message msg;
  int conta=0;

  while (scancode != KEY_ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }


    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();  
          }

    if (msg.m_notify.interrupts & irq_set_2) { /* subscribed interrupt */
        timer_int_handler();
        conta++;
      
        if(speed>0){
          vg_draw_rectangle(xi, yi, img.width, img.height, 0);
          from_buffer();
          if(scancode==D){
              xi=xi+1;
          }
          if(scancode==A){
              xi=xi-1;
          }
          if(scancode==W){
              yi=yi-1;
          }
          if(scancode==S){
              yi=yi+1;
          }

          map = xpm_load(xpm, XPM_INDEXED, &img);
          vg_draw_sprite(map, img, xi, yi);
          from_buffer();
        }

        if(speed<0){
          if(conta == -speed){
            conta=0;
            vg_draw_rectangle(xi, yi, img.width, img.height, 0);
            from_buffer();
            if(scancode==D){
                xi=xi+1;
            }
            if(scancode==A){
                xi=xi-1;
            }
            if(scancode==W){
                yi=yi-1;
            }
            if(scancode==S){
                yi=yi+1;
            }

            map = xpm_load(xpm, XPM_INDEXED, &img);
            vg_draw_sprite(map, img, xi, yi);
            from_buffer();
          }
        }  
    }
          break;
        default:
          break;
      }
    } 
  }
  

  kbd_unsubscribe_i();
  vg_exit();
  return 0;


  
}                 



int(video_test_controller)() {
  printf("%s(): under construction\n", __func__);

  return 1;
  
}










