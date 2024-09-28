#include <lcom/lcf.h>
#include "devices/graphicscard.h"
#include "devices/keyboard.h"
#include "devices/mouse.h"
#include "game.h"
#include "img/pacman.xpm"
#include "img/background.xpm"
#include "devices/i8042.h"
#include "img/mouse.xpm"
#include "img/ghosts.xpm"
#include "img/menu.xpm"
#include "img/number.xpm"

uint8_t bit_no_timer;
uint8_t bit_no_keyboard;
uint8_t bit_no_mouse;

pacman_object pacman;
ghost_object pink_ghost, red_ghost, orange_ghost, blue_ghost;
mouse_object mouse;
numbers_object numbers; 

extern int counter;

extern uint8_t scancode;
extern uint16_t hres;
extern uint16_t vres;
extern uint8_t mouse_counter;
extern struct packet mouse_packet;

xpm_image_t background_img, menu_play_img, menu_score_img, menu_arrow_img, menu_quit_img, menu_background_img, vuln_ghost_img, scorex_img, lives_img, menu_back_img, menu_slash_img;

uint8_t *background_img_map, *menu_play_img_map, *menu_score_img_map, *menu_arrow_img_map,*menu_quit_img_map, *menu_background_img_map, *vuln_ghost_img_map, *scorex_img_map, *lives_img_map, *menu_back_img_map, *menu_slash_img_map;

//0 - wall 1 - coin 2 - powerup 3 - empty
int game_board[29][26] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //0 
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, //1 
    {2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2}, //2 
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, //3 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //4 
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1}, //5 
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1}, //6
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}, //7
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, //8
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, //9
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //10
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //11
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //12
    {3, 3, 3, 3, 3, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 1, 3, 3, 3, 3, 3}, //13
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //14
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //15
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //16
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //17
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //18
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //19
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, //20
    {2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2}, //21
    {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1}, //22
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, //23
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, //24
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}, //25
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //26
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //27
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}  //28
  };

int game_board_original[29][26] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //0 
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, //1 
    {2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2}, //2 
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, //3 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //4 
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1}, //5 
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1}, //6
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}, //7
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, //8
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, //9
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //10
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //11
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //12
    {3, 3, 3, 3, 3, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 1, 3, 3, 3, 3, 3}, //13
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //14
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //15
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //16
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //17
    {0, 0, 0, 0, 0, 1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0}, //18
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //19
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, //20
    {2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2}, //21
    {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1}, //22
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, //23
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, //24
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}, //25
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //26
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //27
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}  //28
  };

uint8_t framerate = 60;

int selected = 0;
int lives_pacman = 3;
gameState game_state = MENU;
direction dir_buffer;
int score = 0;
int temp_counter = 0;

int high_scores[6] = {0, 0, 0, 0, 0, 0};

int (initialize_values)() {
  timer_subscribe_int(&bit_no_timer);
  kbd_subscribe(&bit_no_keyboard);
  mouse_subscribe_int(&bit_no_mouse);

  timer_set_frequency(0, FRAME_RATE);
  vg_init(0x115); 

  load_xpm();

  pacman.xPos = 102 + 299;
  pacman.yPos = 25 + 418;
  pacman.xBPos = 13;
  pacman.yBPos = 22;
  pacman.pos_counter = 0;
  pacman.speed = 1;
  pacman.moving = false;
  pacman.dir = left;
  pacman.powerup = false;

  pink_ghost.xPos = 102 + 299 - 11;
  pink_ghost.yPos = 25 + 228 + 16;
  pink_ghost.xBPos = 13;
  pink_ghost.yBPos = 10;
  pink_ghost.pos_counter = 0;
  pink_ghost.free = false;
  pink_ghost.jail_timer = 0;

  red_ghost.xPos = 102 + 299;
  red_ghost.yPos = 25 + 190;
  red_ghost.xBPos = 13;
  red_ghost.yBPos = 10;
  red_ghost.pos_counter = 0;
  red_ghost.free = true;
  red_ghost.jail_timer = 0;

  orange_ghost.xPos = 102 + 299 - 52;
  orange_ghost.yPos = 25 + 228 + 16;
  orange_ghost.xBPos = 13;
  orange_ghost.yBPos = 10;
  orange_ghost.pos_counter = 0;
  orange_ghost.free = false;
  orange_ghost.jail_timer = 0;

  blue_ghost.xPos = 102 + 299 + 28;
  blue_ghost.yPos = 25 + 228 + 16;
  blue_ghost.xBPos = 13;
  blue_ghost.yBPos = 10;
  blue_ghost.pos_counter = 0;
  blue_ghost.free = false;
  blue_ghost.jail_timer = 0;

  mouse.xPos = hres / 2 - mouse.img.width / 2;
  mouse.yPos = vres / 2 - mouse.img.height / 2;

  return 0;
}

void (reset_pacman_ghost_values)() {
  pacman.xPos = 102 + 299;
  pacman.yPos = 25 + 418;
  pacman.xBPos = 13;
  pacman.yBPos = 22;
  pacman.pos_counter = 0;
  pacman.speed = 1;
  pacman.moving = false;
  pacman.dir = left;
  pacman.powerup = false;

  pink_ghost.xPos = 102 + 299 - 11;
  pink_ghost.yPos = 25 + 228 + 16;
  pink_ghost.xBPos = 13;
  pink_ghost.yBPos = 10;
  pink_ghost.pos_counter = 0;
  pink_ghost.free = false;
  pink_ghost.jail_timer = 0;

  red_ghost.xPos = 102 + 299;
  red_ghost.yPos = 25 + 190;
  red_ghost.xBPos = 13;
  red_ghost.yBPos = 10;
  red_ghost.pos_counter = 0;
  red_ghost.free = true;
  red_ghost.jail_timer = 0;

  orange_ghost.xPos = 102 + 299 - 52;
  orange_ghost.yPos = 25 + 228 + 16;
  orange_ghost.xBPos = 13;
  orange_ghost.yBPos = 10;
  orange_ghost.pos_counter = 0;
  orange_ghost.free = false;
  orange_ghost.jail_timer = 0;

  blue_ghost.xPos = 102 + 299 + 28;
  blue_ghost.yPos = 25 + 228 + 16;
  blue_ghost.xBPos = 13;
  blue_ghost.yBPos = 10;
  blue_ghost.pos_counter = 0;
  blue_ghost.free = false;
  blue_ghost.jail_timer = 0;

  counter = 0;
  temp_counter = 0;
}

void (reset_board)() {
  memcpy(game_board, game_board_original, sizeof(int) * 29*26);
}

void (load_xpm)(){
  menu_play_img_map = xpm_load(menu_play, XPM_8_8_8_8, &menu_play_img);
  menu_arrow_img_map = xpm_load(menu_arrow, XPM_8_8_8_8, &menu_arrow_img);
  menu_quit_img_map = xpm_load(menu_quit, XPM_8_8_8_8, &menu_quit_img);
  menu_score_img_map = xpm_load(menu_score,XPM_8_8_8_8, &menu_score_img);
  menu_background_img_map = xpm_load(menu_background, XPM_8_8_8_8, &menu_background_img);

  pacman.maps[0] = xpm_load(pacman_classic_Down, XPM_8_8_8_8, &pacman.imgs[0]);
  pacman.maps[1] = xpm_load(pacman_classic_Up, XPM_8_8_8_8, &pacman.imgs[1]);
  pacman.maps[2] = xpm_load(pacman_classic_Left, XPM_8_8_8_8, &pacman.imgs[2]);
  pacman.maps[3] = xpm_load(pacman_classic_Right, XPM_8_8_8_8, &pacman.imgs[3]);
  pink_ghost.map = xpm_load(pink_ghost_xpm, XPM_8_8_8_8, &pink_ghost.img);
  red_ghost.map = xpm_load(red_ghost_xpm, XPM_8_8_8_8, &red_ghost.img);
  orange_ghost.map = xpm_load(orange_ghost_xpm, XPM_8_8_8_8, &orange_ghost.img);
  blue_ghost.map = xpm_load(blue_ghost_xpm, XPM_8_8_8_8, &blue_ghost.img);
  vuln_ghost_img_map = xpm_load(vulnerable_ghost_xpm, XPM_8_8_8_8, &vuln_ghost_img);
  background_img_map = xpm_load(background, XPM_8_8_8_8, &background_img);

  mouse.map = xpm_load(mouse1, XPM_8_8_8_8, &mouse.img);

  numbers.maps[0] = xpm_load(number00, XPM_8_8_8_8, &numbers.imgs[0]);
  numbers.maps[1] = xpm_load(number11, XPM_8_8_8_8, &numbers.imgs[1]);
  numbers.maps[2] = xpm_load(number22, XPM_8_8_8_8, &numbers.imgs[2]);
  numbers.maps[3] = xpm_load(number33, XPM_8_8_8_8, &numbers.imgs[3]);
  numbers.maps[4] = xpm_load(number44, XPM_8_8_8_8, &numbers.imgs[4]);
  numbers.maps[5] = xpm_load(number55, XPM_8_8_8_8, &numbers.imgs[5]);
  numbers.maps[6] = xpm_load(number66, XPM_8_8_8_8, &numbers.imgs[6]);
  numbers.maps[7] = xpm_load(number77, XPM_8_8_8_8, &numbers.imgs[7]);
  numbers.maps[8] = xpm_load(number88, XPM_8_8_8_8, &numbers.imgs[8]);
  numbers.maps[9] = xpm_load(number99, XPM_8_8_8_8, &numbers.imgs[9]);

  scorex_img_map = xpm_load(scorex, XPM_8_8_8_8, &scorex_img);
  lives_img_map = xpm_load(lives, XPM_8_8_8_8, &lives_img);
  menu_back_img_map = xpm_load(menu_back, XPM_8_8_8_8, &menu_back_img);
  menu_slash_img_map = xpm_load(slash, XPM_8_8_8_8, &menu_slash_img);

}

void (draw_lives)() {
  vg_draw_xpm(5, 15, lives_img, lives_img_map);
  vg_draw_xpm(25, 15 + scorex_img.height + 10, numbers.imgs[lives_pacman], numbers.maps[lives_pacman]);

}

void (draw_score)() {
  
  vg_draw_xpm(715, 15, scorex_img, scorex_img_map);
  
  if(score < 10) {
    vg_draw_xpm(738, 15 + scorex_img.height + 10, numbers.imgs[score], numbers.maps[score]);
  } else if( score < 100) {
    int dec = score / 10;
    int unit =  score % 10;
    vg_draw_xpm(738, 15 + scorex_img.height + 10 , numbers.imgs[dec], numbers.maps[dec]);
    vg_draw_xpm(738, 15 + scorex_img.height + 45, numbers.imgs[unit], numbers.maps[unit]);
  } else {
    int hund = score / 100;
    int dec = (score % 100) / 10;
    int unit =  (score % 100) % 10;
    vg_draw_xpm(738, 15 + scorex_img.height + 10 , numbers.imgs[hund], numbers.maps[hund]);
    vg_draw_xpm(738, 15 + scorex_img.height + 45, numbers.imgs[dec], numbers.maps[dec]);
    vg_draw_xpm(738, 15 + scorex_img.height + 80, numbers.imgs[unit], numbers.maps[unit]);
  }
}

void (draw_background)() {
  vg_draw_xpm(91, 15, background_img, background_img_map);
}

void (draw_coin)() {
  for(int i = 0; i < 29; i++){
    for(int j = 0; j < 26; j++){
      int x = 102 + 23 * j - j/2;
      int y = 25 + 19 * i - i/1.8;
      if (game_board[i][j] == 1) {
        x += 15;
        y += 17;
        vg_draw_rectangle(x, y, 3, 3, 0xffff00);
      } else if (game_board[i][j] == 2){
        x += 13;
        y += 15;
        vg_draw_rectangle(x, y, 7, 7, 0xffff00);
      }
    }
  }
}

void (pickup_coins)() {
  if(pacman.powerup && (counter - temp_counter)/60.0 >= 30){
    pacman.powerup = false;
  }

  if (game_board[pacman.yBPos][pacman.xBPos] == 1 || game_board[pacman.yBPos][pacman.xBPos] == 2){
    if (game_board[pacman.yBPos][pacman.xBPos] == 2){
      pacman.powerup = true;
      temp_counter = counter;
    }
    game_board[pacman.yBPos][pacman.xBPos] = 3;
    score++;
  }
}

void (draw_mouse)() {
  vg_draw_xpm(mouse.xPos, mouse.yPos, mouse.img, mouse.map);
}

void (game_mouse_click)(uint16_t x, uint16_t y) {
  if(game_state == SCORES) {
    if(x > (hres/2)-350 && x < (hres/2)-350 + 200 && y < vres/2 + 125 + 90 && y > vres/2 + 125) {
      game_state = MENU;
    }
    return;
  }

  if(x > (hres/2)-350 && x < (hres/2)-350 + 200 && y < vres/2 + 100 + 90 && y > vres/2 + 100) {
    game_state = IN_GAME;
    counter = 0;
    score = 0;
    lives_pacman = 3;
    reset_board();
    draw_background();
    draw_coin();
    draw_pacman();
    draw_score();
    draw_lives();
    move_buffer();
    return;
  }
  if(x > (hres/2)-100 && x < (hres/2)-100 + 200 && y < vres/2 + 100 + 90 && y > vres/2 + 100) {
    game_state = SCORES;
    for (int i = 0; i < 6; i++) {
      for (int j = i + 1; j < 6; j++) {
          if (high_scores[i] < high_scores[j]) {
              int aux = high_scores[i];
              high_scores[i] = high_scores[j];
              high_scores[j] = aux;
          }
      }
    }
    return;
  }
  if(x > (hres/2)+150 && x < (hres/2)+150 + 200 && y < vres/2 + 100 + 90 && y > vres/2 + 100) {
    scancode=KEY_ESC;
    return;
  }
}

void (game_mouse_hover)(uint16_t x, uint16_t y) {
  //check play
  if(x > (hres/2)-350 && x < (hres/2)-350 + 200 && y < vres/2 + 100 + 90 && y > vres/2 + 100) {
    selected = 0;
    return;
  }
  if(x > (hres/2)-100 && x < (hres/2)-100 + 200 && y < vres/2 + 100 + 90 && y > vres/2 + 100) {
    selected = 1;
    return;
  }
  if(x > (hres/2)+150 && x < (hres/2)+150 + 200 && y < vres/2 + 100 + 90 && y > vres/2 + 100) {
    selected = 2;
    return;
  }
}

bool (collision)(direction dir) {
  //printf("x:%d y:%d\n", pacman.xBPos, pacman.yBPos);
  if(pacman.pos_counter == 0){
    switch (dir) {
      case right:
        if (pacman.xBPos + 1 == 26 || game_board[pacman.yBPos][pacman.xBPos + 1] == 0) return true;
        break;

      case left:
        if (pacman.xBPos - 1 == -1 || game_board[pacman.yBPos][pacman.xBPos - 1] == 0) return true;
        break;
      
      case up:
        if (pacman.yBPos - 1 == -1 || game_board[pacman.yBPos - 1][pacman.xBPos] == 0) return true;
        break;

      case down:
        if (pacman.yBPos + 1 == 29 || game_board[pacman.yBPos + 1][pacman.xBPos] == 0) return true;  
        break;
    }
  }
  return false;
}

bool (collision_ghosts)(ghost_object* ghost, direction dir) {
  if (ghost->pos_counter == 0){
    switch (dir) {
      case right:
        if (ghost->xBPos + 1 == 26 || game_board[ghost->yBPos][ghost->xBPos + 1] == 0) return true;
        break;

      case left:
        if (ghost->xBPos - 1 == -1 || game_board[ghost->yBPos][ghost->xBPos - 1] == 0) return true;
        break;
      
      case up:
        if (ghost->yBPos - 1 == -1 || game_board[ghost->yBPos - 1][ghost->xBPos] == 0) return true;
        break;

      case down:
        if (ghost->yBPos + 1 == 29 || game_board[ghost->yBPos + 1][ghost->xBPos] == 0) return true;  
        break;
    }
  }
  return false;
}

void (draw_pacman)() {  
  switch (pacman.dir) {
    case down:
      vg_draw_xpm(pacman.xPos - pacman.xBPos/2, pacman.yPos - pacman.yBPos/1.8, pacman.imgs[0], pacman.maps[0]);
      break;
    case up:
      vg_draw_xpm(pacman.xPos - pacman.xBPos/2, pacman.yPos - pacman.yBPos/1.8, pacman.imgs[1], pacman.maps[1]);
      break;
    case left:
      vg_draw_xpm(pacman.xPos - pacman.xBPos/2, pacman.yPos - pacman.yBPos/1.8, pacman.imgs[2], pacman.maps[2]);
      break;
    case right:
      vg_draw_xpm(pacman.xPos - pacman.xBPos/2, pacman.yPos - pacman.yBPos/1.8, pacman.imgs[3], pacman.maps[3]);
      break;
    default:
      break;
  }
}

void (draw_ghost)(ghost_object* ghost) {
  if(!pacman.powerup)
    vg_draw_xpm(ghost->xPos - ghost->xBPos/2, ghost->yPos - ghost->yBPos/1.8, ghost->img, ghost->map);
  else vg_draw_xpm(ghost->xPos - ghost->xBPos/2, ghost->yPos - ghost->yBPos/1.8, vuln_ghost_img, vuln_ghost_img_map);
}

void (ghost_movement)(ghost_object* ghost) {
  if(ghost->pos_counter == 0 && ghost->yBPos == 13){
    if (ghost->xBPos == 0){
      ghost->xBPos = 25;
      ghost->xPos = 102 + 575;
    } else if (ghost->xBPos == 25){
      ghost->xBPos = 0;
      ghost->xPos = 102;
    }
  }
  if (ghost->pos_counter != 0){
    ghost->pos_counter++;
    switch (ghost->dir) {
      case right:
        ghost->xPos++;
        if (ghost->pos_counter % 23 == 0){
          ghost->pos_counter = 0;
          ghost->xBPos++;
        }
        break;

      case left:
        ghost->xPos--;
        if (ghost->pos_counter % 23 == 0){
          ghost->pos_counter = 0;
          ghost->xBPos--;
        }
        break;

      case up:
        ghost->yPos--;
        if (ghost->pos_counter % 19 == 0){
          ghost->pos_counter = 0;
          ghost->yBPos--;
        }
        break;

      case down:
        ghost->yPos++;
        if (ghost->pos_counter % 19 == 0){
          ghost->pos_counter = 0;
          ghost->yBPos++;
        }
        break;
      default:
        break;
    }
  } else {
    while(true){
      int dir = random() % 4;
      switch (dir) {
        case 0:
          if (ghost->dir == left) break; 
          if (!collision_ghosts(ghost, right)){
            ghost->dir = right;
            ghost->pos_counter++;
            ghost->xPos++;
          }
          break;

        case 1:
          if (ghost->dir == right) break; 
          if (!collision_ghosts(ghost, left)){
            ghost->dir = left;
            ghost->pos_counter++;
            ghost->xPos--;
          }
          break;

        case 2:
          if (ghost->dir == down) break; 
          if (!collision_ghosts(ghost, up)){
            ghost->dir = up;
            ghost->pos_counter++;
            ghost->yPos--;
          }
          break;

        case 3:
          if (ghost->dir == up) break; 
          if (!collision_ghosts(ghost, down)){
            ghost->dir = down;
            ghost->pos_counter++;
            ghost->yPos++;
          }
          break;
      
        default:
          break;
      }
      if(ghost->pos_counter != 0) break;
    }
  }
}

bool (GP_collisions)(ghost_object* ghost) {
  if(ghost->jail_timer == counter){
    ghost->free = true;
    ghost->jail_timer = 0;
  }

  if( (((pacman.xBPos == ghost->xBPos || pacman.xBPos == ghost->xBPos + 1 || pacman.xBPos == ghost->xBPos - 1) && pacman.yBPos == ghost->yBPos) || ((pacman.yBPos == ghost->yBPos || pacman.yBPos == ghost->yBPos + 1 || pacman.yBPos == ghost->yBPos - 1) && pacman.xBPos == ghost->xBPos))  && ghost->free){
    if (!pacman.powerup) {
      return true;
    } else {
      ghost->jail_timer = counter + (15 * 60);
      ghost->free = false;  
      ghost->xPos = 102 + 299;
      ghost->yPos = 25 + 190;
      ghost->xBPos = 13;
      ghost->yBPos = 10;
    }
  }
  return false;
}

void draw_menu(int sel) {
  vg_draw_xpm(0, 0, menu_background_img, menu_background_img_map);     
  vg_draw_xpm((hres/2)-350, vres/2 + 100, menu_play_img, menu_play_img_map);
  vg_draw_xpm((hres/2)-100, vres/2 + 100, menu_score_img, menu_score_img_map);
  vg_draw_xpm((hres/2)+150, vres/2 + 100, menu_quit_img, menu_quit_img_map);

  if (sel == 0){
    vg_draw_xpm((hres/2)-350 - 50, vres/2 + 100 + 20, menu_arrow_img, menu_arrow_img_map);
  } else if(sel == 1) {
    vg_draw_xpm((hres/2)-100 - 50, vres/2 + 100 + 20, menu_arrow_img, menu_arrow_img_map);
  } else {
    vg_draw_xpm((hres/2)+150 - 50, vres/2 + 100 + 20, menu_arrow_img, menu_arrow_img_map);
  }
}

void draw_scores_menu() {
  vg_draw_xpm(0, 0, menu_background_img, menu_background_img_map);
  vg_draw_xpm((hres/2)-350 , vres/2 + 125, menu_back_img, menu_back_img_map);

  vg_draw_xpm(hres/2 - 100, vres/2 + 50, numbers.imgs[1], numbers.maps[1]);
  vg_draw_xpm(hres/2 - 100 + 30, vres/2 + 50 + 10, menu_slash_img, menu_slash_img_map);

  draw_number(high_scores[0], hres/2 - 100 + 30 + 25, vres/2 + 50);

  vg_draw_xpm(hres/2 - 100, vres/2 + 100, numbers.imgs[2], numbers.maps[2]);
  vg_draw_xpm(hres/2 - 100 + 30, vres/2 + 100 + 10, menu_slash_img, menu_slash_img_map);

  draw_number(high_scores[1], hres/2 - 100 + 30 + 25, vres/2 + 100);

  vg_draw_xpm(hres/2 - 100, vres/2 + 150, numbers.imgs[3], numbers.maps[3]);
  vg_draw_xpm(hres/2 - 100 + 30, vres/2 + 150 + 10, menu_slash_img, menu_slash_img_map);

  draw_number(high_scores[2], hres/2 - 100 + 30 + 25, vres/2 + 150);

  vg_draw_xpm(hres/2 - 100, vres/2 + 200, numbers.imgs[4], numbers.maps[4]);
  vg_draw_xpm(hres/2 - 100 + 30, vres/2 + 200 + 1, menu_slash_img, menu_slash_img_map);

  draw_number(high_scores[3], hres/2 - 100 + 30 + 25, vres/2 + 200);

  vg_draw_xpm(hres/2 - 100, vres/2 + 250, numbers.imgs[5], numbers.maps[5]);
  vg_draw_xpm(hres/2 - 100 + 30, vres/2 + 250 + 10, menu_slash_img, menu_slash_img_map);

  draw_number(high_scores[4], hres/2 - 100 + 30 + 25, vres/2 + 250);
}

void draw_number(int n, uint16_t x, uint16_t y) {
  if(n < 10) {
    vg_draw_xpm(x, y, numbers.imgs[n], numbers.maps[n]);
  } else if( n < 100) {
    int dec = n / 10;
    int unit =  n % 10;
    vg_draw_xpm(x, y, numbers.imgs[dec], numbers.maps[dec]);
    vg_draw_xpm(x + 35, y, numbers.imgs[unit], numbers.maps[unit]);
  } else {
    int hund = n / 100;
    int dec = (n % 100) / 10;
    int unit =  (n % 100) % 10;
    vg_draw_xpm(x, y , numbers.imgs[hund], numbers.maps[hund]);
    vg_draw_xpm(x + 35, y, numbers.imgs[dec], numbers.maps[dec]);
    vg_draw_xpm(x + 70, y, numbers.imgs[unit], numbers.maps[unit]);
  }
}

int (game_loop)() {
  draw_menu(selected);
  draw_mouse();
  draw_score();
  move_buffer();


  int ipc_status;
  message msg;
  int r;

  while (scancode != KEY_ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & bit_no_keyboard) {
            kbc_ih();
            if(game_state == MENU){
              if (scancode==DOWN || scancode==LEFT || scancode==A) {
                selected--;
              }  
              if (scancode==UP || scancode== RIGHT || scancode==D) {
                selected++;
              }
              if (selected==3) {
                selected = 0;
              }
              if (selected==-1) {
                selected = 2;
              }
              if (scancode==ENTER && selected==0) {
                game_state = IN_GAME;
                counter = 0;
                score = 0;
                lives_pacman = 3;
                reset_board();
                draw_background();
                draw_coin();
                draw_pacman();
                draw_score();
                draw_lives();
                move_buffer();
              }
              if (scancode==ENTER && selected==1) {
                  for (int i = 0; i < 6; i++) {
                    for (int j = i + 1; j < 6; j++) {
                        if (high_scores[i] < high_scores[j]) {
                            int aux = high_scores[i];
                            high_scores[i] = high_scores[j];
                            high_scores[j] = aux;
                        }
                    }
                }
                game_state = SCORES;
              }
              if (scancode==ENTER && selected==2) {
                scancode=KEY_ESC;
              }

            } else if(game_state == IN_GAME) {
              switch (scancode){
                case D:
                  pacman.moving = true;
                  if (pacman.dir != right) {
                    dir_buffer = right;
                  }
                  break;
                case A:
                  pacman.moving = true;
                  if (pacman.dir != left) {
                    dir_buffer = left;
                  }
                  break;
                case W:
                  pacman.moving = true;
                  if (pacman.dir != up) {
                    dir_buffer = up;
                  }
                  break;
                case S:
                  pacman.moving = true;
                  if (pacman.dir != down) {
                    dir_buffer = down;
                  }
                  break;
                case Q:
                  reset_pacman_ghost_values();
                  for (int i = 0; i < 5; i++) {
                    for (int j = i + 1; j < 5; j++) {
                        if (high_scores[i] < high_scores[j]) {
                            int aux = high_scores[i];
                            high_scores[i] = high_scores[j];
                            high_scores[j] = aux;
                        }
                    }
                  }
                  high_scores[5] = score;
                  game_state = MENU;
                  break;
                  
                default:
                  break;
              }
            } else if(game_state == SCORES) {
              if (scancode==ENTER) {
                game_state = MENU;
              }
            }
          }

          if (msg.m_notify.interrupts & bit_no_timer) {
              timer_int_handler();
              if(game_state == MENU){
                draw_menu(selected);
                draw_mouse();
                move_buffer();
              } else if(game_state == IN_GAME) {
                pickup_coins();
                if (score == 244) game_state = MENU;
                if (GP_collisions(&pink_ghost)){
                  lives_pacman--;
                  reset_pacman_ghost_values();
                } 
                if (GP_collisions(&red_ghost)) {
                  lives_pacman--;
                  reset_pacman_ghost_values();
                }
                if (GP_collisions(&orange_ghost)) {
                  lives_pacman--;
                  reset_pacman_ghost_values();
                }
                if (GP_collisions(&blue_ghost)) {
                  lives_pacman--;
                  reset_pacman_ghost_values();
                }
                if(lives_pacman == 0) {
                  reset_pacman_ghost_values();
                  for (int i = 0; i < 5; i++) {
                    for (int j = i + 1; j < 5; j++) {
                        if (high_scores[i] < high_scores[j]) {
                            int aux = high_scores[i];
                            high_scores[i] = high_scores[j];
                            high_scores[j] = aux;
                        }
                    }
                  }
                  high_scores[5] = score;
                  game_state = MENU;
                  break;
                }
                if(pacman.pos_counter == 0 && pacman.yBPos == 13){
                  if (pacman.xBPos == 0){
                    pacman.xBPos = 25;
                    pacman.xPos = 102 + 575;
                  } else if (pacman.xBPos == 25){
                    pacman.xBPos = 0;
                    pacman.xPos = 102;
                  }
                }
                if(pacman.pos_counter == 0 && !collision(dir_buffer)) pacman.dir = dir_buffer;

                if(pacman.moving) {
                  if(red_ghost.jail_timer == 0) ghost_movement(&red_ghost);
                  if (counter/60.0 >= 10){
                    if (!pink_ghost.free && pink_ghost.jail_timer == 0){
                      pink_ghost.xPos = 102 + 299;
                      pink_ghost.yPos = 25 + 190;
                      pink_ghost.free = true;
                    }
                    ghost_movement(&pink_ghost);
                  }
                  if (counter/60.0 >= 20){
                    if (!blue_ghost.free && blue_ghost.jail_timer == 0){
                      blue_ghost.xPos = 102 + 299;
                      blue_ghost.yPos = 25 + 190;
                      blue_ghost.free = true;
                    }
                    ghost_movement(&blue_ghost);
                  }
                  if (counter/60.0 >= 30){
                    if (!orange_ghost.free && orange_ghost.jail_timer == 0){
                      orange_ghost.xPos = 102 + 299;
                      orange_ghost.yPos = 25 + 190;
                      orange_ghost.free = true;
                    }
                    ghost_movement(&orange_ghost);
                  }
                  switch (pacman.dir) {
                    case down:
                      if (!collision(pacman.dir)){
                        pacman.yPos += pacman.speed;
                        pacman.pos_counter += pacman.speed;
                        if (pacman.pos_counter % 19 == 0){
                          pacman.pos_counter = 0;
                          pacman.yBPos++;
                        }
                      }
                      break;
                    case up:
                      if (!collision(pacman.dir)){
                        pacman.yPos -= pacman.speed;
                        pacman.pos_counter += pacman.speed;
                        if (pacman.pos_counter % 19 == 0){
                          pacman.pos_counter = 0;
                          pacman.yBPos--;
                        }
                      }
                      break;
                    case left:
                      if (!collision(pacman.dir)){
                        pacman.xPos -= pacman.speed;
                        pacman.pos_counter += pacman.speed;
                        if (pacman.pos_counter % 23 == 0){
                          pacman.pos_counter = 0;
                          pacman.xBPos--;
                        }
                      }
                      break;
                    case right:
                      if (!collision(pacman.dir)){
                        pacman.xPos += pacman.speed;
                        pacman.pos_counter += pacman.speed;
                        if (pacman.pos_counter % 23 == 0){
                          pacman.pos_counter = 0;
                          pacman.xBPos++;
                        }
                      }
                      break;
                    default:
                      break;
                    }
                } 

                draw_background();
                draw_coin();
                if (red_ghost.jail_timer == 0) draw_ghost(&red_ghost);
                if (pink_ghost.jail_timer == 0) draw_ghost(&pink_ghost);
                if (blue_ghost.jail_timer == 0) draw_ghost(&blue_ghost);
                if (orange_ghost.jail_timer == 0) draw_ghost(&orange_ghost);
                draw_pacman();
                draw_score();
                draw_lives();
                draw_mouse();
                move_buffer();
              } else if(game_state == SCORES) {
                draw_scores_menu();
                draw_mouse();
                move_buffer();
              }
          
          }
          
          if (msg.m_notify.interrupts & bit_no_mouse) {
            mouse_ih();
            assemble_mouse_packet();
            if(mouse_counter == 3) {
              parse_mouse_packet();
              mouse.xPos += mouse_packet.delta_x;
              mouse.yPos -= mouse_packet.delta_y;
              if(mouse.xPos > hres - mouse.img.width) {
                mouse.xPos = hres - mouse.img.width;
              } else if(mouse.xPos < 0) {
                mouse.xPos = 0;
              }

              if(mouse.yPos > vres - mouse.img.height) {
                mouse.yPos = vres - mouse.img.height;
              } else if(mouse.yPos < 0) {
                mouse.yPos = 0;
              }
              if(game_state == MENU || game_state == SCORES) {
                game_mouse_hover(mouse.xPos, mouse.yPos);
                if(mouse_packet.lb) {
                  game_mouse_click(mouse.xPos, mouse.yPos);
                }
              }
            }
          }

          break;
        default:
          break;
      }
    } 
  }
  return 0;
}

int close_game() {

  timer_unsubscribe_int();
  kbd_unsubscribe();
  mouse_unsubscribe_int();

  return vg_exit();
}
