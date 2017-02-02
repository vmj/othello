#ifndef _OTHELLO_FLIPPERS_H_
#define _OTHELLO_FLIPPERS_H_
#include "common.h"
#include "transform.h"
#include "board.h"

/* Controls how many step the animation has (for one disk) */
#define PRECISION_MIN     1
#define PRECISION_MAX   180

Bool oth_flippers_init(Board* board, int *argc, char **argv);
void oth_flippers_free(void);
void oth_flippers_reset();

void oth_flippers_update(void);

void oth_flippers_flip_disks(Board* board, Square* square);
void oth_flippers_game_over(Board* board);

#endif                          /* _OTHELLO_FLIPPERS_H_ */
