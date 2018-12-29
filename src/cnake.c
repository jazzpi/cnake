#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <string.h>
#include <assert.h>

#include "snake.h"
#include "ansi_esc.h"

#define RANDOMIZE_START false

/* https://stackoverflow.com/a/37872060/2192641 */
void stdin_set(bool enable_nonblocking) {
    struct termios t;

    tcgetattr(1, &t);
    if (enable_nonblocking) {
        t.c_lflag &= ~(ICANON | ECHO);
    } else {
        t.c_lflag |= ICANON | ECHO;
    }
    tcsetattr(1, 0, &t);
}

void reset_terminal_mode(void) {
    stdin_set(false);
}

int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch() {
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

static unsigned running = 1;

void handle_sigint(int dummy __attribute__((unused))) {
    running = 0;
}

snake_dir read_chars(snake_snake* snake) {
    static char buf[3];
    static unsigned index = 0;

    int r;
    while ((r = kbhit())) {
        assert(r > 0);
        buf[index] = getch();
        switch (index) {
        case 0:
            if (buf[index] == '\033') {
                index++;
            }
            break;
        case 1:
            if (buf[index] == '[') {
                index++;
            } else {
                index = 0;
            }
            break;
        case 2:
            switch (buf[index]) {
            case 'A':
                index = 0;
                return SNAKE_UP;
            case 'B':
                index = 0;
                return SNAKE_DOWN;
            case 'C':
                index = 0;
                return SNAKE_RIGHT;
            case 'D':
                index = 0;
                return SNAKE_LEFT;
            }
            index = 0;
            break;
        }
    }

    return snake->dir;
}

int main() {
    srand(time(NULL));
    signal(SIGINT, handle_sigint);
    stdin_set(true);

    snake_snake snake;

    snake_init(&snake, RANDOMIZE_START);
    snake_reset(&snake);

    while (running) {
        usleep(100000);

        snake_set_dir(&snake, read_chars(&snake));

        if (!snake_tick(&snake)) {
            snake_draw_gameover(&snake);
            break;
        }
    }

    ansi_esc_move_cursor(0, SNAKE_ROWS + 2);
    stdin_set(false);
}
