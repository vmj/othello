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
        int rank, file;
        Board* board = current_board;

        /* Who Am I ? */
        if (shift == DARK)
        {
                rank = rank(board, best_dark);
                file = file(board, best_dark);
        }
        else
        {                       /* LIGHT */
                rank = rank(board, best_light);
                file = file(board, best_light);
        }

        oth_shift_update(board, rank, file);
}
