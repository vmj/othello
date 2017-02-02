#ifndef _OTHELLO_SHIFT_H_
#define _OTHELLO_SHIFT_H_
#include "common.h"
#include "board.h"

/* Shift enum.  NONE is game over. */
typedef enum { INITIAL, DARK, LIGHT, NONE } Shift;

Bool oth_shift_init(int *argc, char **argv);
void oth_shift_free(void);
void oth_shift_reset(Board* board);

Bool oth_shift_valid(Board* board, Square* square);
void oth_shift_update(Board* board, Square* square);

Shift oth_shift_current();

#endif                          /* _OTHELLO_SHIFT_H_ */
