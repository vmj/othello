#ifndef _OTHELLO_SHIFT_H_
#define _OTHELLO_SHIFT_H_
#include "common.h"

/* Shift enum.  NONE is game over. */
typedef enum { INITIAL, DARK, LIGHT, NONE } Shift;

/* Whose turn is it? */
Shift shift;

Bool oth_shift_init(int *argc, char **argv);
void oth_shift_free();
void oth_shift_reset();

Bool oth_shift_valid(int rank, int file);
void oth_shift_update();

#endif                          /* _OTHELLO_SHIFT_H_ */
