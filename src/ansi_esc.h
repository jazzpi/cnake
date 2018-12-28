#ifndef __ANSI_ESC_H_
#define __ANSI_ESC_H_

#define ANSI_ESC_ESC "\033"
#define ANSI_ESC_CSI ANSI_ESC_ESC "["
#define ANSI_ESC_CSI_ED "%uJ"
#define ANSI_ESC_CSI_ED_TO_END 0
#define ANSI_ESC_CSI_ED_TO_BEG 1
#define ANSI_ESC_CSI_ED_ALL 2
#define ANSI_ESC_CSI_ED_ALL_AND_SCROLLBACK 3
#define ANSI_ESC_CSI_CUP "%u;%uH"

__attribute__ ((format(printf, 1, 2)))
void ansi_esc_print_csi(const char* seq, ...);

/**
 * Clear the screen and move cursor to top left.
 */
void ansi_esc_clear_screen(void);

/**
 * Move the cursor to the zero-based coordinates.
 */
void ansi_esc_move_cursor(unsigned row, unsigned col);

#endif // __ANSI_ESC_H_
