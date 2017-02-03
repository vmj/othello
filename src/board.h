#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_
#include "common.h"
#include "transform.h"

typedef enum { EMPTY, BLACK, WHITE } Disk;

typedef struct { unsigned int blacks; unsigned int whites; } Score;

/* One square on the board. */
typedef struct {
        /* immutable fields */
        unsigned int name;
        unsigned int rank;
        unsigned int file;

        /* read-only, except for board (update scores) */
        Score score;

        /*
         * read-write, with limited state changes.
         *
         * Initially EMPTY.
         *
         * EMPTY -> NON-EMPTY, no flipping.
         *
         * flipping false -> true, disk to opponent, flipper null -> non-null.
         *
         * flipping true -> false, disk stays and flipper clears.
         */
        Disk disk;
        Bool flipping;
        Flipper *flipper;
} Square;

/**
 * Square size in OpenGL world units.
 *
 * Square size is a convenient unit when placing lights and camera around
 * the board.  A value of one is a no-op, so it could be optimized out
 * (likely, the compiler does that).  But, since we've gone through the
 * trouble of having all the calculations done in terms of square size,
 * let's just keep this around.
 */
#define SQUARESIZE ((unsigned)1)

typedef struct {
        /*
         * Array of Squares
         */
        Square *squares;
        /*
         * Number of squares in that array.
         */
        unsigned int nsquares;
        /*
         * Pointer to an empty square that has highest score for the dark.
         */
        Square *best_dark;
        /*
         * Pointer to an empty square that has highest score for the light.
         */
        Square *best_light;
        /*
         * Actual board size. User can define them using -r and -f options.
         */
        int ranks;
        int files;
        /*
         * Current number of black or white disks on the board.
         *
         * I.e. the game score.
         */
        Score score;
} Board;

/**
 * A callback function to flip one disk.
 *
 * @param   board       Board on which the disk is flipped.
 * @param   square      Square on which the disk is flipped.
 * @param   disk        Color of the disk after flipping.
 * @param   first       true if this is the first disk in one direction.
 * @param   user_data   Callback state.
 */
typedef void (*FlipDiskFunc)(Board* board, Square* square, Disk disk, Bool first, void* user_data);

typedef void (*SquareVisitor)(Board* board, Square* square, void* user_data);

/* Limits for board size. */
#define BOARD_SIZE_MIN  ((unsigned)4)
#define BOARD_SIZE_MAX  ((unsigned)98)
#define BOARD_SIZE_DEF  ((unsigned)8)

Board*   oth_board_init         (int *argc, char **argv);
void     oth_board_free         (Board* board);
void     oth_board_reset        (Board* board);

void     oth_board_flip_disks   (Board* board, Square* square, FlipDiskFunc flip_disk, void* user_data);

void     oth_board_for_each_square   (Board* board, SquareVisitor visitor, void* user_data);

Square*  oth_board_square       (Board* board, unsigned int name);

#endif                          /* _OTHELLO_BOARD_H_ */
