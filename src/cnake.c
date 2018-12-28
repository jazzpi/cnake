#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "snake.h"
#include "ansi_esc.h"

int main() {
    srand(time(NULL));

    snake_snake snake;

    snake_init(&snake);
    snake_debug(&snake);
    snake_reset(&snake);

    sleep(1);

    snake_tick(&snake);

    ansi_esc_move_cursor(0, SNAKE_ROWS + 2);

    snake_debug(&snake);
}
