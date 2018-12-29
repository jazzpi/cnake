#include "snake.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "ansi_esc.h"
#include "util.h"

void snake_init(snake_snake* snake, bool randomize) {
    assert(SNAKE_ROWS > 2 * SNAKE_INITIAL_OFFSET);
    assert(SNAKE_COLS > 2 * SNAKE_INITIAL_OFFSET);

    snake->length = SNAKE_INITIAL_LENGTH;

    unsigned x, y;
    if (randomize) {
        x = ((rand() % (SNAKE_COLS - 2 * SNAKE_INITIAL_OFFSET))
             + SNAKE_INITIAL_OFFSET);
        y = ((rand() % (SNAKE_ROWS - 2 * SNAKE_INITIAL_OFFSET))
             + SNAKE_INITIAL_OFFSET);
    } else {
        x = SNAKE_COLS / 2;
        y = SNAKE_ROWS / 2;
    }
    snake->head.x = x;
    snake->head.y = y;
    snake->tail = snake->head;

    snake->dir = randomize ? rand() % 4 : SNAKE_RIGHT;

    switch (snake->dir) {
    case SNAKE_UP:
        snake->head.y -= (snake->length - 1);
        break;
    case SNAKE_DOWN:
        snake->head.y += (snake->length - 1);
        break;
    case SNAKE_LEFT:
        snake->head.x -= (snake->length - 1);
        break;
    case SNAKE_RIGHT:
        snake->head.x += (snake->length - 1);
        break;
    }

    memset(&snake->cells, 0, sizeof(snake->cells));
    for (x = min(snake->head.x, snake->tail.x);
         x <= max(snake->head.x, snake->tail.x);
         x++) {
        for (y = min(snake->head.y, snake->tail.y);
             y <= max(snake->head.y, snake->tail.y);
             y++) {
            snake->cells[x][y] = SNAKE_DIR_TO_CELL(snake->dir);
        }
    }

    snake_generate_target(snake);
}

void snake_draw_full(snake_snake* snake) {
    unsigned x, y;
    for (x = 0; x < SNAKE_COLS + 2; x++) {
        printf("=");
    }
    printf("\n");
    for (y = 0; y < SNAKE_ROWS; y++) {
        printf("|");
        for (x = 0; x < SNAKE_COLS; x++) {
            if (x == snake->target.x && y == snake->target.y) {
                printf("%c", TARGET_CHAR);
            } else {
                printf("%c", snake->cells[x][y] ? SNAKE_CHAR : ' ');
            }
        }
        printf("|\n");
    }
    for (x = 0; x < SNAKE_COLS + 2; x++) {
        printf("=");
    }
    printf("\n");
}

void snake_reset(snake_snake* snake) {
    ansi_esc_clear_screen();
    snake_draw_full(snake);
}

void snake_debug(snake_snake* snake) {
    const char* dir;
    switch (snake->dir) {
    case SNAKE_UP:
        dir = "UP";
        break;
    case SNAKE_DOWN:
        dir = "DOWN";
        break;
    case SNAKE_LEFT:
        dir = "LEFT";
        break;
    case SNAKE_RIGHT:
        dir = "RIGHT";
        break;
    }

    printf(
        "Snake with head at (%02u|%02u), tail at (%02u|%02u), facing %s\n"
        "==============================================================\n",
        snake->head.x, snake->head.y, snake->tail.x, snake->tail.y, dir
    );

    snake_draw_full(snake);
}

bool snake_tick(snake_snake* snake) {
    bool ret = snake_move(snake);

    fflush(stdout);

    return ret;
}

bool snake_move(snake_snake* snake) {
    /* Move head forward */
    snake->head = snake_add(snake->head, snake->dir, 1);
    /* TODO: coords are unsigned, this can't be < 0 (but it overflows, so it
       works?) */
    if (snake->head.x < 0 || snake->head.x >= SNAKE_COLS ||
        snake->head.y < 0 || snake->head.y >= SNAKE_ROWS ||
        snake->cells[snake->head.x][snake->head.y]) {
        /* TODO: Game over? */
        return false;
    }

    snake_fill(snake, snake->head, SNAKE_DIR_TO_CELL(snake->dir));

    if (snake->head.x == snake->target.x && snake->head.y == snake->target.y) {
        snake->length++;
        if (snake->length == SNAKE_COLS * SNAKE_ROWS) {
            /* TODO: Game won? */
            return false;
        }
        snake_generate_target(snake);
        snake_goto(snake->target);
        printf("%c", TARGET_CHAR);
    } else {
        /* Move tail forward */
        snake_coord old_tail = snake->tail;
        snake->tail = snake_add(
            snake->tail,
            SNAKE_CELL_TO_DIR(snake->cells[snake->tail.x][snake->tail.y]),
            1
        );
        snake_clear(snake, old_tail);
    }

    return true;
}

snake_coord snake_add(snake_coord start, snake_dir dir, unsigned dist) {
    switch (dir) {
    case SNAKE_UP:
        start.y -= dist;
        break;
    case SNAKE_DOWN:
        start.y += dist;
        break;
    case SNAKE_LEFT:
        start.x -= dist;
        break;
    case SNAKE_RIGHT:
        start.x += dist;
        break;
    }

    return start;
}

void snake_goto(snake_coord coord) {
    /* Offset by one to account for border */
    ansi_esc_move_cursor(coord.x + 1, coord.y + 1);
}

void snake_fill(snake_snake* snake, snake_coord coord, snake_cell type) {
    snake->cells[coord.x][coord.y] = type;
    snake_goto(coord);
    printf("%c", SNAKE_CHAR);
}

void snake_clear(snake_snake* snake, snake_coord coord) {
    snake->cells[coord.x][coord.y] = SNAKE_EMPTY;
    snake_goto(coord);
    printf(" ");
}

bool snake_set_dir(snake_snake* snake, snake_dir dir) {
    int diff = (((int) snake->dir) - ((int) dir) + 4) % 4;
    if (diff == 1 || diff == 3) {
        snake->dir = dir;
        snake->cells[snake->head.x][snake->head.y] = SNAKE_DIR_TO_CELL(dir);
        return true;
    }
    return false;
}

void snake_generate_target(snake_snake* snake) {
    unsigned possible = SNAKE_ROWS * SNAKE_COLS - snake->length;
    assert(possible > 0);

    /* Put the target in an empty spot */
    unsigned index = rand() % possible;
    unsigned i = 0;
    unsigned x, y;
    for (y = 0; y < SNAKE_ROWS; y++) {
        for (x = 0; x < SNAKE_COLS; x++) {
            if (!snake->cells[x][y]) {
                if (i == index) {
                    snake->target.x = x;
                    snake->target.y = y;
                    return;
                }
                i++;
            }
        }
    }
}
