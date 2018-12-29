#include "snake.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "ansi_esc.h"
#include "util.h"

void snake_init(snake_snake* snake) {
    assert(SNAKE_ROWS > 2 * SNAKE_INITIAL_OFFSET);
    assert(SNAKE_COLS > 2 * SNAKE_INITIAL_OFFSET);

    unsigned x = ((rand() % (SNAKE_ROWS - 2 * SNAKE_INITIAL_OFFSET))
                  + SNAKE_INITIAL_OFFSET);
    unsigned y = ((rand() % (SNAKE_ROWS - 2 * SNAKE_INITIAL_OFFSET))
                  + SNAKE_INITIAL_OFFSET);
    snake->head.x = x;
    snake->head.y = y;
    snake->tail = snake->head;

    snake->dir = rand() % 4;

    snake->bb.start.x = snake->bb.end.x = x;
    snake->bb.start.y = snake->bb.end.y = y;
    switch (snake->dir) {
    case SNAKE_UP:
        snake->head.y -= (SNAKE_INITIAL_LENGTH - 1);
        snake->bb.start.y -= (SNAKE_INITIAL_LENGTH - 1);
        break;
    case SNAKE_DOWN:
        snake->head.y += (SNAKE_INITIAL_LENGTH - 1);
        snake->bb.end.y += (SNAKE_INITIAL_LENGTH - 1);
        break;
    case SNAKE_LEFT:
        snake->head.x -= (SNAKE_INITIAL_LENGTH - 1);
        snake->bb.start.x -= (SNAKE_INITIAL_LENGTH - 1);
        break;
    case SNAKE_RIGHT:
        snake->head.x += (SNAKE_INITIAL_LENGTH - 1);
        snake->bb.end.x += (SNAKE_INITIAL_LENGTH - 1);
        break;
    }

    memset(&snake->cells, 0, SNAKE_ROWS * SNAKE_COLS);
    for (x = snake->bb.start.x; x <= snake->bb.end.x; x++) {
        for (y = snake->bb.start.y; y <= snake->bb.end.y; y++) {
            snake->cells[x][y] = SNAKE_DIR_TO_CELL(snake->dir);
        }
    }
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
            printf("%c", snake->cells[x][y] ? '0' + (y % 10) : ' ');
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
        "==============================================================\n"
        "BB: (%02u|%02u) to (%02u|%02u)\n",
        snake->head.x, snake->head.y, snake->tail.x, snake->tail.y, dir,
        snake->bb.start.x, snake->bb.start.y, snake->bb.end.x, snake->bb.end.y
    );

    snake_draw_full(snake);
}

bool snake_tick(snake_snake* snake) {
    assert(snake->bb.start.x <= snake->head.x &&
           snake->head.x <= snake->bb.end.x);
    assert(snake->bb.start.y <= snake->head.y &&
           snake->head.y <= snake->bb.end.y);
    assert(snake->bb.start.x <= snake->tail.x &&
           snake->tail.x <= snake->bb.end.x);
    assert(snake->bb.start.y <= snake->tail.y &&
           snake->tail.y <= snake->bb.end.y);

    bool ret = snake_move(snake);

    fflush(stdout);

    return ret;
}

bool snake_move(snake_snake* snake) {
    /* Move head forward */
    snake->head = snake_add(snake->head, snake->dir, 1);
    if (snake->head.x < 0 || snake->head.x > SNAKE_COLS ||
        snake->head.y < 0 || snake->head.y > SNAKE_ROWS) {
        return false;
    }

    snake_fill(snake, snake->head, SNAKE_DIR_TO_CELL(snake->dir));

    /* Update BB */
    snake->bb.start.x = min(snake->bb.start.x, snake->head.x);
    snake->bb.end.x = max(snake->bb.end.x, snake->head.x);
    snake->bb.start.y = min(snake->bb.start.y, snake->head.y);
    snake->bb.end.y = max(snake->bb.end.y, snake->head.y);

    /* Move tail forward */
    snake_coord old_tail = snake->tail;
    snake->tail = snake_add(
        snake->tail,
        SNAKE_CELL_TO_DIR(snake->cells[snake->tail.x][snake->tail.y]),
        1
    );
    snake_clear(snake, old_tail);

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
    printf("X");
}

void snake_clear(snake_snake* snake, snake_coord coord) {
    snake->cells[coord.x][coord.y] = SNAKE_EMPTY;
    snake_goto(coord);
    printf(" ");
}
