#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_
#include "common.h"
#include "flippers.h"           // Flipper

/* One square on the board. */
typedef struct {
        enum { EMPTY, BLACK, WHITE } disk;
        Bool flipping;
        Flipper *flipper;
} Square;

/* Serves as a measure of unit, more than anything else. */
#define SQUARESIZE 50

/* Array of Squares */
Square *board;

/* Limits for board size. */
#define BOARD_SIZE_MIN  4
#define BOARD_SIZE_MAX 98
#define BOARD_SIZE_DEF  8

/* Actual board size. User can define them using -r and -f options. */
int RANKS;
int FILES;

/*  */
struct Score {
        int dark;
        int light;
} *score;

/* Indexes of the "best" moves */
int best_dark;
int best_light;

/* Macros to convert between index and (rank, file) information. */
#define rank(index)          ((index) / (FILES))
#define file(index)          ((index) % (FILES))
#define index(rank, file)    ((FILES) * (rank) + (file))

/* Macros to access board and score arrays as if they were two dimensional. */
#define board(rank, file)    (board[ index((rank), (file)) ])
#define score(rank, file)    (score[ index((rank), (file)) ])

Bool oth_board_init(int *argc, char **argv);
void oth_board_free();
void oth_board_reset();

void oth_board_flip_disks(int rank, int file);

#endif                          /* _OTHELLO_BOARD_H_ */
