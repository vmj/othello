#include <stdio.h>
#include <GL/glut.h>
#include "display.h"
#include "input.h"
#include "board.h"
#include "flippers.h"
#include "camera.h"
#include "shift.h"

void __oth_init (int *argc, char **argv);
void __oth_help (int exitcode);

int
main (int argc, char **argv)
{
  /* Use GLUT to create a window for drawing */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize(400,400);
  glutInitWindowPosition(100,150);
  glutCreateWindow("Othello");
  
  /* Define the callback functions */
  glutDisplayFunc (oth_display);
  glutReshapeFunc (oth_reshape);
  glutSpecialFunc (oth_special_keys);

  /* Initialize globals */
  __oth_init(&argc, argv);
  
  /* Enter loop */
  glutMainLoop();
  return 0;
}

void
__oth_init (int *argc, char **argv)
{
  int i;

  for (i = 1; i < *argc; ++i) {
    switch (argv[i][0]) {
    case '-':
      switch (argv[i][1]) {
      case 'h': /* help */
	__oth_help (0);
	break;
      }
      break;
    }
  }
  
  /* Alloc and init board and score*/
  if ( ! oth_board_init (argc, argv) ) {
    __oth_help (-1);
  }

  /* Alloc and init flippers */
  if ( ! oth_flippers_init (argc, argv) ) {
    oth_board_free ();
    __oth_help (-1);
  }

  /* Init cam */
  if ( ! oth_camera_init (argc, argv) ) {
    oth_board_free ();
    oth_flippers_free ();
    __oth_help (-1);
  }
  
  /* Init display */
  if ( ! oth_display_init (argc, argv) ) {
    oth_board_free ();
    oth_flippers_free ();
    oth_camera_free ();
    oth_shift_free ();
    __oth_help (-1);
  }

  /* Init shift */
  if ( ! oth_shift_init (argc, argv) ) {
    oth_board_free ();
    oth_flippers_free ();
    oth_camera_free ();
    __oth_help (-1);
  }

  /* OpenGL initialization */
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_SMOOTH);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_CULL_FACE);
  glCullFace(GL_BACK);
}

/** void oth_help (int exitcode)
 *
 * This function prints a short reminder how the program is supposed to be
 * launched. Output goes to standard error. This function does NOT return,
 * but calls exit() system call with exitcode.
 */
void
__oth_help (int exitcode)
{
  fprintf (stderr, "othello [-h] [-p NUM] [-r NUM] [-f NUM]\n");
  fprintf (stderr, "\n");
  fprintf (stderr, "    -h\n");
  fprintf (stderr, "        Print this help message and exit.\n");
  fprintf (stderr, "    -2\n");
  fprintf (stderr, "        Human versus human mode.\n");
  fprintf (stderr, "    -p NUM\n");
  fprintf (stderr, "        Specify how precise the flipping animation\n");
  fprintf (stderr, "        should be. By default, it's calculated as a\n");
  fprintf (stderr, "        function of board size.\n");
  fprintf (stderr, "        Value between (inclusive) %i and %i.\n",
	   PRECISION_MIN, PRECISION_MAX);
  fprintf (stderr, "    -r NUM\n");
  fprintf (stderr, "        Specify the board size (or just how many ranks\n");
  fprintf (stderr, "        there should be, if also -f is given).\n");
  fprintf (stderr, "        Value between (inclusive) %i and %i.\n",
	   BOARD_SIZE_MIN, BOARD_SIZE_MAX);
  fprintf (stderr, "    -f NUM\n");
  fprintf (stderr, "        Specify the board size (or just how many files\n");
  fprintf (stderr, "        there should be, if also -r is given).\n");
  fprintf (stderr, "        Value between (inclusive) %i and %i.\n",
	   BOARD_SIZE_MIN, BOARD_SIZE_MAX);
  exit (exitcode);
}
