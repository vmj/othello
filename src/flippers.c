#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#include "global.h"
#include "common.h"
#include "board.h"
#include "display.h"
#include "shift.h"
#include "flippers.h"

/* We need this many flippers */
static int nflippers;

/* Array of those to support wave-animation */
static Flipper *flippers;

/* This is to jump out of the animation early when only few disks are flipping.
 * Index of the last used Flipper in *flippers.
 */
static int last_used_flipper;

/* Animation precision */
static unsigned char precision;

/* This is used to hold step sizes */
static Flipper stepper;

void __oth_flippers_flip_disks(Board* board, Square* square, Disk disk, Bool first, void* user_data);
void __oth_flippers_game_over(Board* board, Square* square, void* user_data);

/**
 * Initialize flippers "subsystem".
 */
Bool
oth_flippers_init(Board* board, int *argc, char **argv)
{
        int i;
        double a, b;
        precision = 0;

        for (i = 1; i < *argc; ++i)
        {
                switch (argv[i][0])
                {
                case '-':
                        switch (argv[i][1])
                        {
                        case 'p':
                                if (++i > *argc)
                                {
                                        fprintf(stderr,
                                                "Argument missing for option 'p'\n");
                                        return false;
                                }
                                precision =
                                        (unsigned
                                         char)CLAMP(PRECISION_MIN,
                                                    atoi(argv[i]),
                                                    PRECISION_MAX);
                        }
                        break;
                }
        }

        /* Define precision as a function of board size (ax+b) */
        if (precision == 0)
        {
                a = ((PRECISION_MIN - PRECISION_MAX)
                     / ((double)BOARD_SIZE_MAX - BOARD_SIZE_MIN));
                b = 180 - BOARD_SIZE_MIN * a;
                precision =
                        (unsigned char)(a * ((board->ranks + board->files) / 2.0) + b);
        }

        /* Initialize flippers */
        nflippers = MAX (board->ranks, board->files) - 2;
        flippers = (Flipper *) calloc(nflippers, sizeof(Flipper));
        if (flippers == NULL)
        {
                fprintf(stderr, "Memory exhaustion.\n");
                return false;
        }

        /* Initialize stepper */
        stepper.translation.x = SQUARESIZE / (float)precision;
        stepper.rotation.a = 180 / (float)precision;
        for (i = 0; i < 3; ++i)
        {
                stepper.ambient[i] = dark_ambient[i];
                stepper.diffuse[i] = (light_diffuse[i] - dark_diffuse[i])
                                   / (float)precision;
                stepper.specular[i] = dark_specular[i];
        }

        oth_flippers_reset();
        return true;
}

/**
 * Free resources taken by flippers.
 */
void
oth_flippers_free()
{
        if (flippers)
                free(flippers);
}

/**
 * Reset flippers to their starting values.
 */
void
oth_flippers_reset()
{
        register int i, j;

        /* reset starting values */
        for (i = 0; i < nflippers; ++i)
        {
                flippers[i].translation.x = 0.0f;
                flippers[i].rotation.a = 0.0f;
                flippers[i].rotation.z = 1.0f;
                for (j = 0; j < 3; ++j)
                {
                        switch (oth_shift_current()) {
                                case INITIAL:
                                        break;
                                case DARK:
                                        /* We're going to flip light to dark */
                                        flippers[i].diffuse[j] = light_diffuse[j];
                                        break;
                                case LIGHT:
                                        /* We're going to flip dark to light */
                                        flippers[i].diffuse[j] = dark_diffuse[j];
                                        break;
                                case NONE:
                                        flippers[0].diffuse[j] = light_diffuse[j];
                                        flippers[1].diffuse[j] = dark_diffuse[j];
                                        break;
                        }
                }
        }
}

/**
 * GLUT idle function to update flipper structures.
 */
void
oth_flippers_update(void)
{
        Flipper *p;
        int flipper, i;
        /* j controls the wave effect. */
        static int j = -1;

        /* Initialize j (first pass) */
        if (j == -1)
        {
                j = nflippers - 1;
        }

        /* Start flipping next disk? */
        if (flippers[nflippers - j - 1].rotation.a >= (180 / nflippers))
        {
                j = (j > 0) ? j - 1 : 0;
        }

        /* Update flippers */
        if (oth_shift_current() != NONE)
        {
                for (flipper = 0; flipper < nflippers - j; ++flipper)
                {
                        p = &flippers[flipper];
                        switch (oth_shift_current())
                        {
                        case DARK:
                                p->translation.x =
                                        MIN(p->translation.x +
                                            stepper.translation.x,
                                            SQUARESIZE);
                                p->rotation.a =
                                        MIN(p->rotation.a +
                                            stepper.rotation.a, 180);
                                for (i = 0; i < 3; ++i)
                                {
                                        p->diffuse[i] =
                                                MAX(p->diffuse[i] -
                                                    stepper.diffuse[i],
                                                    dark_diffuse[i]);
                                }
                                break;
                        case LIGHT:
                                p->translation.x =
                                        MIN(p->translation.x +
                                            stepper.translation.x,
                                            SQUARESIZE);
                                p->rotation.a =
                                        MIN(p->rotation.a +
                                            stepper.rotation.a, 180);
                                for (i = 0; i < 3; ++i)
                                {
                                        p->diffuse[i] =
                                                MIN(p->diffuse[i] +
                                                    stepper.diffuse[i],
                                                    light_diffuse[i]);
                                }
                                break;
                        }
                }
        }
        else
        {                       /* Game Over animation */
                last_used_flipper = 1;
                /* Flip darks */
                flippers[0].translation.x =
                        MIN(flippers[0].translation.x +
                            stepper.translation.x, SQUARESIZE);
                flippers[0].rotation.a =
                        MIN(flippers[0].rotation.a + stepper.rotation.a,
                            180);
                for (i = 0; i < 3; ++i)
                {
                        flippers[0].diffuse[i] =
                                MAX(flippers[0].diffuse[i] -
                                    stepper.diffuse[i], dark_diffuse[i]);
                }
                /* Flip lights */
                flippers[1].translation.x =
                        MIN(flippers[1].translation.x +
                            stepper.translation.x, SQUARESIZE);
                flippers[1].rotation.a =
                        MIN(flippers[1].rotation.a + stepper.rotation.a,
                            180);
                for (i = 0; i < 3; ++i)
                {
                        flippers[1].diffuse[i] =
                                MIN(flippers[1].diffuse[i] +
                                    stepper.diffuse[i], light_diffuse[i]);
                }
        }

        /* Resign from idle function (last pass) */
        if (flippers[last_used_flipper].rotation.a >= 180)
        {
                j = -1;
                oth_shift_reset(current_board);
        }

        glutPostRedisplay();
}

void
oth_flippers_flip_disks(Board* board, Square* square)
{
    int flipper = 0;
    last_used_flipper = 0;

    oth_board_flip_disks(board, square, __oth_flippers_flip_disks, &flipper);
}

void
__oth_flippers_flip_disks(Board* board, Square* square, Disk disk, Bool first, void* user_data)
{
    int *flipper = (int*) user_data;

    if (first)
        *flipper = 0;

    square->disk = disk;
    square->flipping = true;
    square->flipper = &flippers[*flipper];

    last_used_flipper = MAX((*flipper), last_used_flipper);

    *flipper = (*flipper) + 1;
}

void
oth_flippers_game_over(Board* board) {
        Score score;

        score.blacks = board->score.blacks;
        score.whites = board->score.whites;

        oth_flippers_reset();
        oth_board_for_each_square(board, __oth_flippers_game_over, &score);
}

void
__oth_flippers_game_over(Board* board, Square* square, void* user_data) {
        Score *score = user_data;

        switch (square->disk) {
                case BLACK:
                        if (score->blacks == 0) {
                                /* turn it to light */
                                square->disk = WHITE;
                                square->flipping = true;
                                square->flipper = &flippers[1];
                                score->whites--;
                        } else {
                                /* just count it */
                                score->blacks--;
                        }
                break;
                case WHITE:
                        if (score->blacks > 0) {
                                /* turn it to dark */
                                square->disk = BLACK;
                                square->flipping = true;
                                square->flipper = &flippers[0];
                                score->blacks--;
                        } else {
                                /* just count it */
                                score->whites--;
                        }
                break;
        }
}
