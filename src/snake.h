#ifndef __SNAKE_H_
#define __SNAKE_H_

#include <stdbool.h>

/* Allow for the edge to be printed inside standard XTerm dimensions */
#define SNAKE_COLS (80 - 2)
#define SNAKE_ROWS (24 - 2)

#define SNAKE_CHAR '#'
#define TARGET_CHAR 'X'
#define SNAKE_SCORE_FMT "Score: %04u"
#define SNAKE_SCORE_WIDTH 11
#define SNAKE_GAME_OVER_WIDTH 22
#define SNAKE_GAME_OVER_HEIGHT 5

/* Initial minimum distance from the edge */
#define SNAKE_INITIAL_OFFSET 5
#define SNAKE_INITIAL_LENGTH 6

typedef struct {
    unsigned x;
    unsigned y;
} snake_coord;

typedef enum {
    SNAKE_UP = 0,
    SNAKE_DOWN = 2,
    SNAKE_LEFT = 1,
    SNAKE_RIGHT = 3
} snake_dir;

/* Save the direction the snake went in each cell. */
typedef enum {
    SNAKE_EMPTY = 0,
    SNAKE_CUP = 1,
    SNAKE_CDOWN = 3,
    SNAKE_CLEFT = 2,
    SNAKE_CRIGHT = 4
} snake_cell;

#define SNAKE_CELL_TO_DIR(cell) ((snake_dir) (((unsigned) cell) - 1))
#define SNAKE_DIR_TO_CELL(dir) ((snake_cell) (((unsigned) dir) + 1))

typedef struct {
    unsigned length;
    snake_coord head;
    snake_coord tail;
    snake_coord target;
    snake_dir dir;
    unsigned char cells[SNAKE_COLS][SNAKE_ROWS];
} snake_snake;

/**
 * Initialize the snake.
 *
 * @param snake The snake to initialize
 * @param randomize Whether to randomize the starting position
 */
void snake_init(snake_snake* snake, bool randomize);

/**
 * Draw the entire screen.
 */
void snake_draw_full(snake_snake* snake);

/**
 * Redraw the entire screen.
 */
void snake_reset(snake_snake* snake);

/**
 * Output debug information about the snake.
 */
void snake_debug(snake_snake* snake);

/**
 * Perform one game tick and update the screen.
 *
 * @return true if the game is still running and false if it's game over.
 */
bool snake_tick(snake_snake* snake);

/**
 * Move the snake by one cell in its respective direction.
 *
 * @return true if the game is still running and false if it's game over.
 */
bool snake_move(snake_snake* snake);

/**
 * Calculate a cell from a start, direction, and distance.
 */
snake_coord snake_add(snake_coord start, snake_dir dir, unsigned dist);

/**
 * Move the cursor to the given coordinate.
 */
void snake_goto(snake_coord coord);

/**
 * Fill a cell on screen and in the cells storage.
 */
void snake_fill(snake_snake* snake, snake_coord coord, snake_cell type);

/**
 * Clear a cell on screen and in the cells storage.
 */
void snake_clear(snake_snake* snake, snake_coord coord);

/**
 * (Attempt to) set the direction of the snake.
 */
bool snake_set_dir(snake_snake* snake, snake_dir dir);

/**
 * Generate a target.
 */
void snake_generate_target(snake_snake* snake);

/**
 * Draw the target to the screen.
 */
void snake_draw_target(snake_snake* snake);

/**
 * Draw the score to the screen.
 */
void snake_draw_score(snake_snake* snake);

/**
 * Draw the game over message to the screen.
 */
void snake_draw_gameover(snake_snake* snake);

#endif // __SNAKE_H_
