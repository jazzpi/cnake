#include "ansi_esc.h"

#include <stdarg.h>
#include <stdio.h>

void ansi_esc_print_csi(const char* seq, ...) {
    va_list ap;
    va_start(ap, seq);

    printf(ANSI_ESC_CSI);
    vprintf(seq, ap);
}

void ansi_esc_clear_screen(void) {
    ansi_esc_print_csi(ANSI_ESC_CSI_ED, ANSI_ESC_CSI_ED_ALL);
    ansi_esc_print_csi(ANSI_ESC_CSI_CUP, 1, 1);
}

void ansi_esc_move_cursor(unsigned col, unsigned row) {
    ansi_esc_print_csi(ANSI_ESC_CSI_CUP, row + 1, col + 1);
}
