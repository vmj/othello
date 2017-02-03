#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "board.h"

/* Macros to convert between index and (rank, file) information. */
#define rank(board, index)          ((index) / ((board)->files))
#define file(board, index)          ((index) % ((board)->files))
#define index(board, rank, file)    (((board)->files) * (rank) + (file))

/* Macro to access board as if it were two dimensional. */
#define board(board, rank, file)    (&((board)->squares[ index((board), (rank), (file)) ]))


void __oth_board_reset(Board* board, Square* square, void* user_data);

void __board_update_scores(Board* board, Square* square);
void __board_update_scores_d(Board* board, Square* square, int r_inc, int f_inc);
void __board_flip_disks_d(Board* board, Square* orig_square, int r_inc, int f_inc, FlipDiskFunc flip_disk, void* user_data);

/**
 * Initialize board "subsystem".
 */
Board*
oth_board_init(int *argc, char **argv)
{
        unsigned int i, rank, file, unset_size;
        Board* board = NULL;
        Square* squares = NULL;

        unset_size = BOARD_SIZE_MIN-1;
        rank = file = unset_size;

        for (i = 1; i < *argc; ++i)
        {
                switch (argv[i][0])
                {
                case '-':
                        switch (argv[i][1])
                        {
                        case 'r':      /* ranks */
                                if (++i > *argc)
                                {
                                        fprintf(stderr,
                                                "Argument missing for option 'r'\n");
                                        return NULL;
                                }
                                rank = CLAMP(BOARD_SIZE_MIN,
                                             atoi(argv[i]),
                                             BOARD_SIZE_MAX);
                                rank -= rank % 2;
                                break;
                        case 'f':      /* files */
                                if (++i > *argc)
                                {
                                        fprintf(stderr,
                                                "Argument missing for option 'f'\n");
                                        return NULL;
                                }
                                file = CLAMP(BOARD_SIZE_MIN,
                                             atoi(argv[i]),
                                             BOARD_SIZE_MAX);
                                file -= file % 2;
                                break;
                        }
                        break;
                }
        }

        /* Set RANKS and FILES */
        if ((rank == unset_size && file != unset_size) || (rank != unset_size && file == unset_size))
        {                       /* XOR */
                rank = file = MAX(rank, file);
        }
        else
        {
                rank = (rank != unset_size) ? rank : BOARD_SIZE_DEF;
                file = (file != unset_size) ? file : BOARD_SIZE_DEF;
        }

        /* Initialize board */
        board = (Board *) calloc(1, sizeof(Board));
        if (board == NULL)
                return NULL;
        board->ranks = rank;
        board->files = file;
        i = rank * file;
        board->squares = squares = (Square *) calloc(i, sizeof(Square));
        if (squares == NULL)
        {
                free(board);
                board = NULL;
                return NULL;
        }
        do {
                i--;
                squares[i].name = i;
                squares[i].rank = rank(board, i);
                squares[i].file = file(board, i);
                squares[i].disk = EMPTY;
        } while (i > 0);

        /* Set starting positions */
        board(board, rank / 2, file / 2)->disk = WHITE;
        board(board, rank / 2, file / 2 - 1)->disk = BLACK;
        board(board, rank / 2 - 1, file / 2)->disk = BLACK;
        board(board, rank / 2 - 1, file / 2 - 1)->disk = WHITE;

        /* Initialize scores and flipper pointers */
        oth_board_reset(board);
        return board;
}

/**
 * Free resources taken by board.
 */
void
oth_board_free(Board* board)
{
        if (board)
        {
                if (board->squares)
                        free(board->squares);
                free(board);
        }
}

/**
 *
 */
void
oth_board_reset(Board* board) {
    /* Reset "bests" */
    board->best_dark = board->best_light = oth_board_square(board, 0);

    board->score.blacks = 0;
    board->score.whites = 0;

    oth_board_for_each_square(board, __oth_board_reset, NULL);
}

void
__oth_board_reset(Board* board, Square* square, void* user_data) {
    square->flipping = false;
    square->score.whites = 0;
    square->score.blacks = 0;

    /* Update scores and "bests" */
    switch (square->disk) {
        case EMPTY:
            __board_update_scores(board, square);

            if (square->score.blacks > board->best_dark->score.blacks)
                board->best_dark = square;
            if (square->score.whites > board->best_light->score.whites)
                board->best_light = square;
            break;
        case BLACK:
            board->score.blacks++;
            break;
        case WHITE:
            board->score.whites++;
            break;
    }
}

/**
 * Helper function for above.
 */
void
__board_update_scores(Board* board, Square* square)
{
        __board_update_scores_d(board, square, 1, 0);      /* N  */
        __board_update_scores_d(board, square, 1, 1);      /* NE */
        __board_update_scores_d(board, square, 0, 1);      /* E  */
        __board_update_scores_d(board, square, -1, 1);     /* SE */
        __board_update_scores_d(board, square, -1, 0);     /* S  */
        __board_update_scores_d(board, square, -1, -1);    /* SW */
        __board_update_scores_d(board, square, 0, -1);     /* W  */
        __board_update_scores_d(board, square, 1, -1);     /* NW */
}

/**
 * Helper function for above. Counts the score in one direction.
 */
void
__board_update_scores_d(Board* board, Square* square, int r_inc, int f_inc)
{
        int r = square->rank + r_inc;
        int f = square->file + f_inc;
        int score_dark = 0, score_light = 0;
        Bool no_darks = true, no_lights = true;

        while ( r >= 0 && r < board->ranks
             && f >= 0 && f < board->files
             && (no_darks || no_lights))
        {
                switch (board(board, r, f)->disk)
                {
                case EMPTY:
                        goto DONE;      /* break away */
                        break;
                case BLACK:
                        no_darks = false;
                        if (no_lights)
                                ++score_light;
                        r += r_inc;
                        f += f_inc;
                        break;
                case WHITE:
                        no_lights = false;
                        if (no_darks)
                                ++score_dark;
                        r += r_inc;
                        f += f_inc;
                        break;
                }
        }

 DONE:
        if (!no_darks)
                square->score.blacks += score_dark;
        if (!no_lights)
                square->score.whites += score_light;
}

/**
 * Sets disks for flipping around given square.
 */
void
oth_board_flip_disks(Board* board, Square* square, FlipDiskFunc flip_disk, void* user_data)
{
        if (square->disk == EMPTY)
                return;

        __board_flip_disks_d(board, square, 1, 0, flip_disk, user_data);      /* N  */
        __board_flip_disks_d(board, square, 1, 1, flip_disk, user_data);      /* NE */
        __board_flip_disks_d(board, square, 0, 1, flip_disk, user_data);      /* E  */
        __board_flip_disks_d(board, square, -1, 1, flip_disk, user_data);     /* SE */
        __board_flip_disks_d(board, square, -1, 0, flip_disk, user_data);     /* S  */
        __board_flip_disks_d(board, square, -1, -1, flip_disk, user_data);    /* SW */
        __board_flip_disks_d(board, square, 0, -1, flip_disk, user_data);     /* W  */
        __board_flip_disks_d(board, square, 1, -1, flip_disk, user_data);     /* NW */
}

/**
 * Helper function for above. Does it in one direction.
 */
void
__board_flip_disks_d(Board* board, Square* orig_square, int r_inc, int f_inc, FlipDiskFunc flip_disk, void* user_data)
{
        register int r = orig_square->rank + r_inc;
        register int f = orig_square->file + f_inc;
        Bool found = false;
        Square* other = NULL;

        /* Find same color in direction (r_inc, f_inc) */
        while (r >= 0 && r < board->ranks && f >= 0 && f < board->files)
        {
                other = board(board, r, f);
                if (other->disk == orig_square->disk)
                {
                        found = true;
                        break;  /* Got it! */
                }
                if (other->disk == EMPTY)
                        return; /* Found empty before same */

                r += r_inc;
                f += f_inc;
        }
        if (!found)
                return;

        /* Flip disks */
        r = orig_square->rank + r_inc;
        f = orig_square->file + f_inc;
        other = board(board, r, f);
        Bool first = true;
        while (other->disk != orig_square->disk)
        {
                flip_disk(board, other, orig_square->disk, first, user_data);

                r += r_inc;
                f += f_inc;
                other = board(board, r, f);
                first = false;
        }
}

void
oth_board_for_each_square(Board* board, SquareVisitor visitor, void* user_data)
{
    int rank, file;
    Square *square = NULL;
    for (rank = 0; rank < board->ranks; ++rank) {
        for (file = 0; file < board->files; ++file) {
            square = board(board, rank, file);
            visitor(board, square, user_data);
        }
    }
}


Square*
oth_board_square(Board* board, int name)
{
        return &(board->squares[name]);
}
