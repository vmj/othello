#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#include "common.h"
#include "comp.h"
#include "flippers.h"
#include "user.h"
#include "shift.h"
#include "board.h"

/* Whose turn is it? */
static Shift shift;


void __oth_shift_game_over(Board* board);

void __shift_set_comp(void);
void __shift_unset_comp(void);
void __shift_set_user(void);
void __shift_unset_user(void);

/* Function pointers to above functions */
void (*__shift_set_dark) (void);
void (*__shift_unset_dark) (void);
void (*__shift_set_light) (void);
void (*__shift_unset_light) (void);

/**
 * Initialize shift manager.
 */
Bool
oth_shift_init(int *argc, char **argv)
{
        int i;
        Bool h2h = false;

        for (i = 1; i < *argc; ++i)
        {
                switch (argv[i][0])
                {
                case '-':
                        switch (argv[i][1])
                        {
                        case '2':      /* human vs human mode */
                                h2h = true;
                                break;
                        }
                        break;
                }
        }

        if (h2h)
        {
                __shift_set_dark = __shift_set_user;
                __shift_unset_dark = __shift_unset_user;
                __shift_set_light = __shift_set_user;
                __shift_unset_light = __shift_unset_user;
        }
        else
        {
                srand((unsigned)time(NULL));

                if (rand() % 2)
                {
                        __shift_set_dark = __shift_set_user;
                        __shift_unset_dark = __shift_unset_user;
                        __shift_set_light = __shift_set_comp;
                        __shift_unset_light = __shift_unset_comp;
                }
                else
                {
                        __shift_set_dark = __shift_set_comp;
                        __shift_unset_dark = __shift_unset_comp;
                        __shift_set_light = __shift_set_user;
                        __shift_unset_light = __shift_unset_user;
                }
        }

        /* In Othello, unlike in Chess, dark player moves first */
        shift = DARK;
        __shift_set_dark();

        return true;
}

/**
 * Free resources taken by shift manager.
 */
void
oth_shift_free(void)
{
}

/**
 * Returns true if current player is allowed to put her disk in given square.
 */
Bool
oth_shift_valid(Board* board, Square* square)
{
        if (square->disk != EMPTY)
                return false;
        if (shift == DARK && square->score.dark == 0)
                return false;
        else if (shift == LIGHT && square->score.light == 0)
                return false;
        else if (shift == NONE)
                return false;

        return true;
}

/**
 * Put's a disk in the given square for the current player. Also, sets disks
 * for flipping and changes current player and starts animation.
 */
void
oth_shift_update(Board* board, Square* square)
{
        if (!oth_shift_valid(board, square))
                return;

        square->disk = (shift == DARK ? BLACK : WHITE);
        oth_flippers_flip_disks(board, square);

        switch (shift)
        {
        case NONE:
                /* Shouldn't happend */
                break;
        case DARK:
                __shift_unset_dark();
                break;
        case LIGHT:
                __shift_unset_light();
                break;
        }

        /* start animation */
        glutIdleFunc(oth_flippers_update);
}

/**
 * Stops the animation, resets the board and flippers, and changes current
 * player. If next player has no moves, shift is given back to previous
 * player. However, if both players are out of luck, starts game over
 * animation.
 */
void
oth_shift_reset(Board* board)
{
        glutIdleFunc(NULL);

        oth_board_reset(board);

        if (shift == DARK)
        {
                if (board->best_light)
                {
                        shift = LIGHT;
                        oth_flippers_reset();
                        __shift_set_light();
                }
                else if (board->best_dark)
                {
                        oth_flippers_reset();
                        __shift_set_dark();
                }
                else
                {
                        shift = NONE;
                        __oth_shift_game_over(board);
                }
        }
        else if (shift == LIGHT)
        {
                if (board->best_dark)
                {
                        shift = DARK;
                        oth_flippers_reset();
                        __shift_set_dark();
                }
                else if (board->best_light)
                {
                        oth_flippers_reset();
                        __shift_set_light();
                }
                else
                {
                        shift = NONE;
                        __oth_shift_game_over(board);
                }
        }
        else
        {                       /* shift == NONE, Game Over animation done. */
                fprintf(stderr, "Game Over!\n");
        }
}

Shift
oth_shift_current()
{
    return shift;
}

/**
 * Counts points, and set disks for game over animation.
 */
void
__oth_shift_game_over(Board* board)
{
        fprintf(stderr, "Darks: %i, Lights: %i\n", board->blacks, board->whites);
        oth_flippers_game_over(board);
        glutIdleFunc(oth_flippers_update);
}

/**
 * Set up callback for computer.
 */
void
__shift_set_comp(void)
{
        glutIdleFunc(oth_comp);
}

/**
 * Undo above.
 */
void
__shift_unset_comp(void)
{
        glutIdleFunc(NULL);
}

/**
 * Set up callbacks for user.
 */
void
__shift_set_user(void)
{
        glutMouseFunc(oth_mouse);
}

/**
 * Undo above.
 */
void
__shift_unset_user(void)
{
        glutMouseFunc(NULL);
}
