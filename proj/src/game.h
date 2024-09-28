#ifndef __GAME_H_
#define __GAME_H_

#define D 0x20
#define A 0x1E
#define W 0x11
#define S 0x1F
#define Q 0x10
#define UP 0x48
#define DOWN 0x50
#define ENTER 0x1C
#define RIGHT 0x4D
#define LEFT 0x4B

#define FRAME_RATE 60

typedef enum {
  down, //0
  up, //1
  left, //2
  right //3
  } direction;

typedef struct {
  int16_t xPos, yPos;
  int16_t xBPos, yBPos;
  uint16_t pos_counter;
  xpm_image_t imgs[4]; //one for each direction
  uint8_t *maps[4];
  direction dir;
  uint8_t speed;
  bool moving;
  bool powerup;
} pacman_object;

typedef struct {
  int16_t xPos, yPos;
  int16_t xBPos, yBPos;
  uint16_t pos_counter;
  xpm_image_t img;
  direction dir;
  bool free;
  int jail_timer;
  uint8_t *map;
} ghost_object;

typedef struct {
  int16_t xPos, yPos;
  xpm_image_t img; //one for each direction
  uint8_t *map;
} mouse_object;

typedef struct {
  xpm_image_t imgs[10]; //one for each direction
  uint8_t *maps[10];
} numbers_object;

typedef enum {
  MENU,
  IN_GAME,
  SCORES
  } gameState;

/**
 * @brief This function gets the devices ready to be used
 * 
 *  Subscribes the necessary interrupts and then set the graphics mode.
 *  Call the function that loads the xmp's to have them ready to use
 *  Then also initializes some necessary variables
 * 
 * @return 0 upon usccess, else 1
 */
int (initialize_values)();


/**
 * @brief Loads xmp's into memory
 * 
 */
void (load_xpm)();

/**
 * @brief Draws the background
 * 
 *  Calls a function that draw xpm using graphics functionalities
 * 
 */
void (draw_background)();

/**
 * @brief Draws a pacman based on its direction
 * 
 *  Calls a function that draw xpm using graphics functionalities
 * 
 */
void (draw_pacman)();

/**
 * @brief Draws a coin
 * 
 *  Calls a function that draw xpm using graphics functionalities
 */
void (draw_coin)();

/**
 * @brief Responsible for receiving the interrupts, calling the drawing functions.
 *                          Main game function
 * Here we have our game loop running until the game is over
 * @return 0 upon success, else 1
 */
int (game_loop)();

/**
 * @brief Here we return to our normal state
 * 
 *  Unsubscribes the interrupts and returns graphics mode to text mode
 * 
 * @return Returns 0 on success and others on failure
 */
int (close_game)();

/**
 * @brief Draws the game menu
 * 
 *  Calls several function that draw xmp's using graphics functionalites
 * 
 * @param sel = If a button as been selected on not
 */
void draw_menu(int sel);

/**
 * @brief draws the high scores menu
 * 
 */
void draw_scores_menu();

/**
 * @brief draws a number starting the given coordinates
 * 
 * @param n Number to draw
 * @param x Horizontal coordinate
 * @param y Vertical Coordinate
 */
void draw_number(int n, uint16_t x, uint16_t y);

/**
 * @brief Draws the mouse icon
 * 
 *  Calls a function that draw xpm using graphics functionalities
 * 
 */
void (draw_mouse)();

/**
 * @brief Controls where a user clicks on the screen
 * 
 *  If a user clicks on the coordenates of the play button the game begins
 * 
 */
void (game_mouse_click)(uint16_t x, uint16_t y);

/**
 * @brief Controls where a user has the mouse on the screen
 * 
 *  If a user hovers over a one of the buttons on the screen it selects them
 * 
 */
void (game_mouse_hover)(uint16_t x, uint16_t y);

/**
 * @brief draws the score during the game
 * 
 */
void (draw_score)();

/**
 * @brief draws the number of lives the user currently has
 * 
 */
void (draw_lives)();

/**
 * @brief Controls the collsions of the pacman and walls
 * 
 * @param dir = A direção para a qual o pacman está virado 
 * @return true if its colliding with a wall, false otherwise
 */
bool (collision)(direction dir);

/**
 * @brief Lets the pacman pickup coin
 * 
 *  Based on the pacman position and a coin position increments the score if the pacman is on top of a coin
 * 
 */
void (pickup_coins)();

/**
 * @brief resets the pacman ad ghosts atributes when they collide in game
 * 
 */
void (reset_pacman_ghost_values)();

/**
 * @brief resets the board content when restarting the game
 * 
 */
void (reset_board)();

#endif
