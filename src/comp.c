#include "board.h"
#include "shift.h"
#include "comp.h"

/**
 * Very simple function to get computer play Othello.
 */
void
oth_comp (void)
{
  int rank, file;
  
  /* Who Am I ? */
  if (shift == DARK) {
    rank = rank(best_dark);
    file = file(best_dark);
  } else { /* LIGHT */
    rank = rank(best_light);
    file = file(best_light);
  }

  oth_shift_update (rank, file);
}

