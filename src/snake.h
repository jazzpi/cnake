#ifndef __SNAKE_H_
#define __SNAKE_H_

#include <stdbool.h>

/* Allow for the edge to be printed inside standard XTerm dimensions */
#define SNAKE_COLS (80 - 2)
#define SNAKE_ROWS (24 - 2)

/* Initial minimum distance from the edge */
#define SNAKE_INITIAL_OFFSET 5
#define SNAKE_INITIAL_LENGTH 3

typedef struct {
    unsigned x;
    unsigned y;
} snake_coord;

typedef struct {
    snake_coord start;
    snake_coord end;
} snake_bb;

typedef enum {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_LEFT,
    SNAKE_RIGHT
} snake_dir;

/* Save the direction the snake went in each cell. */
typedef enum {
    SNAKE_EMPTY = 0,
    SNAKE_CUP,
    SNAKE_CDOWN,
    SNAKE_CLEFT,
    SNAKE_CRIGHT
} snake_cell;

#define SNAKE_CELL_TO_DIR(cell) ((snake_dir) (((unsigned) cell) - 1))
#define SNAKE_DIR_TO_CELL(dir) ((snake_cell) (((unsigned) dir) + 1))

typedef struct {
    snake_coord head;
    snake_coord tail;
    snake_dir dir;
    snake_bb bb;
    unsigned char cells[SNAKE_COLS][SNAKE_ROWS];
} snake_snake;

/**
 * Initialize the snake at a random position.
 */
void snake_init(snake_snake* snake);

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

#endif // __SNAKE_H_
