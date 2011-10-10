#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "common.h"
#include "board.h"              // RANKS, FILES, BOARD_SIZE_*
#include "display.h"            // materials (light_* and dark_*)
#include "shift.h"              // shift
#include "flippers.h"

/* We need this many flippers */
#define FLIPPERS        (MAX (RANKS, FILES) - 2)

/* This is to jump out of the animation early when only few disks are flipping.
 * Index of the last used Flipper in *flippers. Maintained by 
 * oth_board_flip_disks and used by oth_flippers_update. 
 */
int last_used_flipper;

/* Animation precision */
unsigned char precision;

/* This is used to hold step sizes */
Flipper stepper;

/**
 * Initialize flippers "subsystem".
 */
Bool
oth_flippers_init(int *argc, char **argv)
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
                        (unsigned char)(a * ((RANKS + FILES) / 2.0) + b);
        }

        /* Initialize flippers */
        flippers = (Flipper *) calloc(FLIPPERS, sizeof(Flipper));
        if (flippers == NULL)
        {
                fprintf(stderr, "Memory exhaustion.\n");
                return false;
        }

        /* Initialize stepper */
        stepper.translation.x = (float)SQUARESIZE / precision;
        stepper.rotation.a = (float)180 / precision;
        for (i = 0; i < 3; ++i)
        {
                stepper.ambient[i] = dark_ambient[i];
                stepper.diffuse[i] =
                        ((float)light_diffuse[i] -
                         dark_diffuse[i]) / precision;
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
        int f = FLIPPERS;

        /* reset starting values */
        for (i = 0; i < f; ++i)
        {
                flippers[i].translation.x = 0.0f;
                flippers[i].rotation.a = 0.0f;
                flippers[i].rotation.z = 1.0f;
                for (j = 0; j < 3; ++j)
                {
                        if (shift == DARK)
                        {
                                /* We're going to flip light to dark */
                                flippers[i].diffuse[j] = light_diffuse[j];
                        }
                        else if (shift == LIGHT)
                        {
                                /* We're going to flip dark to light */
                                flippers[i].diffuse[j] = dark_diffuse[j];
                        }
                        else
                        {       /* NONE (Game Over) */
                                flippers[0].diffuse[j] = light_diffuse[j];
                                flippers[1].diffuse[j] = dark_diffuse[j];
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
                j = FLIPPERS - 1;
        }

        /* Start flipping next disk? */
        if (flippers[FLIPPERS - j - 1].rotation.a >= (180 / FLIPPERS))
        {
                j = (j > 0) ? j - 1 : 0;
        }

        /* Update flippers */
        if (shift != NONE)
        {
                for (flipper = 0; flipper < FLIPPERS - j; ++flipper)
                {
                        p = &flippers[flipper];
                        switch (shift)
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
                oth_shift_reset();
        }

        glutPostRedisplay();
}
