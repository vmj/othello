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
                rank = rank(board, board->best_dark->name);
                file = file(board, board->best_dark->name);
        }
        else
        {                       /* LIGHT */
                rank = rank(board, board->best_light->name);
                file = file(board, board->best_light->name);
        }

        oth_shift_update(board, rank, file);
}
