#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "board.h"

/* This is declared in flippers.c */
extern int last_used_flipper;

void __board_update_scores(Board* board, int rank, int file);
void __board_update_scores_d(Board* board, int rank, int file, int r_inc,
                                    int f_inc);
void __board_flip_disks_d(Board* board, int disk, int rank, int file, int r_inc,
                                 int f_inc);

/**
 * Initialize board "subsystem".
 */
Board*
oth_board_init(int *argc, char **argv)
{
        int i, rank, file;
        Board* board = NULL;
        Square* squares = NULL;

        rank = file = -1;

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
        if ((rank == -1 && file != -1) || (rank != -1 && file == -1))
        {                       /* XOR */
                rank = file = MAX(rank, file);
        }
        else
        {
                rank = (rank != -1) ? rank : BOARD_SIZE_DEF;
                file = (file != -1) ? file : BOARD_SIZE_DEF;
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
        while (--i >= 0)
        {
                squares[i].name = i;
                squares[i].disk = EMPTY;
        }

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
oth_board_reset(Board* board)
{
        register int rank, file;
        Square *square = NULL;
        int best_score_d = 0, best_score_l = 0;

        /* Reset "bests" */
        board->best_dark = board->best_light = NULL;

        for (rank = 0; rank < board->ranks; ++rank)
        {
                for (file = 0; file < board->files; ++file)
                {
                        square = board(board, rank, file);
                        square->flipping = false;
                        square->score.light = 0;
                        square->score.dark = 0;

                        /* Update scores and "bests" */
                        if (square->disk == EMPTY)
                        {
                                __board_update_scores(board, rank, file);

                                if (square->score.dark > best_score_d)
                                {
                                        board->best_dark = square;
                                        best_score_d = square->score.dark;
                                }
                                if (square->score.light > best_score_l)
                                {
                                        board->best_light = square;
                                        best_score_l = square->score.light;
                                }
                        }
                }
        }
}

/**
 * Helper function for above.
 */
void
__board_update_scores(Board* board, int rank, int file)
{
        __board_update_scores_d(board, rank, file, 1, 0);      /* N  */
        __board_update_scores_d(board, rank, file, 1, 1);      /* NE */
        __board_update_scores_d(board, rank, file, 0, 1);      /* E  */
        __board_update_scores_d(board, rank, file, -1, 1);     /* SE */
        __board_update_scores_d(board, rank, file, -1, 0);     /* S  */
        __board_update_scores_d(board, rank, file, -1, -1);    /* SW */
        __board_update_scores_d(board, rank, file, 0, -1);     /* W  */
        __board_update_scores_d(board, rank, file, 1, -1);     /* NW */
}

/**
 * Helper function for above. Counts the score in one direction.
 */
void
__board_update_scores_d(Board* board, int rank, int file, int r_inc, int f_inc)
{
        Square *square = board(board, rank, file);
        int r = rank + r_inc;
        int f = file + f_inc;
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
                square->score.dark += score_dark;
        if (!no_lights)
                square->score.light += score_light;
}

/**
 * Sets disks for flipping around given square.
 */
void
oth_board_flip_disks(Board* board, Square* square)
{
        int disk = square->disk;
        int rank = rank(board, square->name);
        int file = file(board, square->name);

        last_used_flipper = 0;

        if (disk == EMPTY)
                return;

        __board_flip_disks_d(board, disk, rank, file, 1, 0);      /* N  */
        __board_flip_disks_d(board, disk, rank, file, 1, 1);      /* NE */
        __board_flip_disks_d(board, disk, rank, file, 0, 1);      /* E  */
        __board_flip_disks_d(board, disk, rank, file, -1, 1);     /* SE */
        __board_flip_disks_d(board, disk, rank, file, -1, 0);     /* S  */
        __board_flip_disks_d(board, disk, rank, file, -1, -1);    /* SW */
        __board_flip_disks_d(board, disk, rank, file, 0, -1);     /* W  */
        __board_flip_disks_d(board, disk, rank, file, 1, -1);     /* NW */
}

/**
 * Helper function for above. Does it in one direction.
 */
void
__board_flip_disks_d(Board* board, int disk, int rank, int file, int r_inc, int f_inc)
{
        int flipper;
        register int r = rank + r_inc;
        register int f = file + f_inc;
        Bool found = false;
        Square *square = NULL;

        /* Find same color in direction (r_inc, f_inc) */
        while (r >= 0 && r < board->ranks && f >= 0 && f < board->files)
        {
                square = board(board, r, f);
                if (square->disk == disk)
                {
                        found = true;
                        break;  /* Got it! */
                }
                if (square->disk == EMPTY)
                        return; /* Found empty before same */

                r += r_inc;
                f += f_inc;
        }
        if (!found)
                return;

        /* Flip disks */
        flipper = 0;
        r = rank + r_inc;
        f = file + f_inc;
        square = board(board, r, f);
        while (square->disk != disk)
        {
                square->disk = disk;
                square->flipping = true;
                square->flipper = &flippers[flipper];

                last_used_flipper = MAX(flipper, last_used_flipper);

                flipper++;
                r += r_inc;
                f += f_inc;
                square = board(board, r, f);
        }
}

Square*
oth_board_square(Board* board, int name)
{
        return &(board->squares[name]);
}
