#include "global.h"
#include "board.h"
#include "shift.h"
#include "comp.h"

/**
 * Very simple function to get computer play Othello.
 */
void
oth_comp(void)
{
        Board* board = current_board;
        Square* square = oth_shift_current() == DARK ? board->best_dark : board->best_light;

        oth_shift_update(board, square);
}
