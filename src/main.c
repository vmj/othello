#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#include "global.h"
#include "display.h"
#include "keyboard.h"
#include "board.h"
#include "flippers.h"
#include "camera.h"
#include "shift.h"
#include "version.h"

void __oth_init(int *argc, char **argv);
void __oth_help(int exitcode);
void __oth_version(void);

int
main(int argc, char **argv)
{
        /* Use GLUT to create a window for drawing */
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
        glutCreateWindow("Othello");

        /* Define the callback functions */
        glutDisplayFunc(oth_display);
        glutReshapeFunc(oth_reshape);
        glutSpecialFunc(oth_special_keys);
        glutKeyboardFunc(oth_keyboard);

        /* Initialize globals */
        __oth_init(&argc, argv);

        /* Enter loop */
        glutMainLoop();
        return 0;
}

void
__oth_init(int *argc, char **argv)
{
        int i;
        Board *board;
        Camera *camera;

        for (i = 1; i < *argc; ++i)
        {
                switch (argv[i][0])
                {
                case '-':
                        switch (argv[i][1])
                        {
                        case 'h':      /* help */
                                __oth_help(0);
                                break;
                        case 'v':      /* version */
                                __oth_version();
                                break;
                        }
                        break;
                }
        }

        /* Alloc and init board */
        current_board = board = oth_board_init(argc, argv);
        if (!board)
        {
                __oth_help(-1);
        }

        /* Alloc and init flippers */
        if (!oth_flippers_init(board, argc, argv))
        {
                oth_board_free(board);
                __oth_help(-1);
        }

        /* Init camera */
        current_camera = camera = oth_camera_init(board, argc, argv);
        if (!camera)
        {
                oth_board_free(board);
                oth_flippers_free();
                __oth_help(-1);
        }

        /* Init display */
        if (!oth_display_init(board, camera, argc, argv))
        {
                oth_board_free(board);
                oth_flippers_free();
                oth_camera_free(camera);
                __oth_help(-1);
        }

        /* Init shift */
        if (!oth_shift_init(argc, argv))
        {
                oth_board_free(board);
                oth_flippers_free();
                oth_camera_free(camera);
                oth_display_free();
                __oth_help(-1);
        }

        /* OpenGL initialization */
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
}

/** void oth_help (int exitcode)
 *
 * This function prints a short reminder how the program is supposed to be
 * launched. Output goes to standard error. This function does NOT return,
 * but calls exit() system call with exitcode.
 */
void
__oth_help(int exitcode)
{
        fprintf(stderr, "othello [-h] [-p NUM] [-r NUM] [-f NUM]\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "    -h\n");
        fprintf(stderr, "        Print this help message and exit.\n");
        fprintf(stderr, "    -2\n");
        fprintf(stderr, "        Human versus human mode.\n");
        fprintf(stderr, "    -p NUM\n");
        fprintf(stderr,
                "        Specify how precise the flipping animation\n");
        fprintf(stderr,
                "        should be. By default, it's calculated as a\n");
        fprintf(stderr, "        function of board size.\n");
        fprintf(stderr, "        Value between (inclusive) %i and %i.\n",
                PRECISION_MIN, PRECISION_MAX);
        fprintf(stderr, "    -r NUM\n");
        fprintf(stderr,
                "        Specify the board size (or just how many ranks\n");
        fprintf(stderr,
                "        there should be, if also -f is given).\n");
        fprintf(stderr,
                "        An even value between (inclusive) %i and %i.\n",
                BOARD_SIZE_MIN, BOARD_SIZE_MAX);
        fprintf(stderr, "    -f NUM\n");
        fprintf(stderr,
                "        Specify the board size (or just how many files\n");
        fprintf(stderr,
                "        there should be, if also -r is given).\n");
        fprintf(stderr,
                "        An even value between (inclusive) %i and %i.\n",
                BOARD_SIZE_MIN, BOARD_SIZE_MAX);
        exit(exitcode);
}

/** void oth_version()
 *
 */
void
__oth_version()
{
        printf("othello %s\n", program_version);
        printf("Copyright (C) 2011 Mikko Värri.\n");
        printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
        printf("This is free software: you are free to change and redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");
        exit(0);
}
