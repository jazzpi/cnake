#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "snake.h"
#include "ansi_esc.h"

static unsigned running = 1;

void handle_sigint(int dummy __attribute__((unused))) {
    running = 0;
}

int main() {
    srand(time(NULL));
    signal(SIGINT, handle_sigint);

    snake_snake snake;

    snake_init(&snake);
    snake_debug(&snake);
    snake_reset(&snake);

    while (running) {
        sleep(1);
        if (!snake_tick(&snake)) {
            break;
        }
    }

    ansi_esc_move_cursor(0, SNAKE_ROWS + 2);

    snake_debug(&snake);
}
