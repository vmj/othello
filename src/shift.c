#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include "common.h"
#include "comp.h"
#include "board.h"
#include "flippers.h"
#include "input.h"
#include "shift.h"

void __oth_shift_game_over ();

void __shift_set_comp (void);
void __shift_unset_comp (void);
void __shift_set_user (void);
void __shift_unset_user (void);

/* Function pointers to above functions */
void (*__shift_set_dark)    (void);
void (*__shift_unset_dark)  (void);
void (*__shift_set_light)   (void);
void (*__shift_unset_light) (void);

/**
 * Initialize shift manager.
 */
Bool
oth_shift_init (int *argc, char **argv)
{
  int i;
  Bool h2h = false;

  for (i = 1; i < *argc; ++i) {
    switch (argv[i][0]) {
    case '-':
      switch (argv[i][1]) {
      case '2': /* human vs human mode */
        h2h = true;
	break;
      }
      break;
    }
  }

  if (h2h) {
    __shift_set_dark    = __shift_set_user;
    __shift_unset_dark  = __shift_unset_user;
    __shift_set_light   = __shift_set_user;
    __shift_unset_light = __shift_unset_user;
  } else {
    srand( (unsigned) time (NULL));

    if ( rand()%2 ) {
    __shift_set_dark    = __shift_set_user;
    __shift_unset_dark  = __shift_unset_user;
    __shift_set_light   = __shift_set_comp;
    __shift_unset_light = __shift_unset_comp;
    } else {
    __shift_set_dark    = __shift_set_comp;
    __shift_unset_dark  = __shift_unset_comp;
    __shift_set_light   = __shift_set_user;
    __shift_unset_light = __shift_unset_user;
    }
  }

  /* In Othello, unlike in Chess, dark player moves first */
  shift = DARK;
  __shift_set_dark ();
  
  return true;
}

/**
 * Free resources taken by shift manager.
 */
void
oth_shift_free ()
{
}

/**
 * Returns true if current player is allowed to put her disk in given square.
 */
Bool
oth_shift_valid (int rank, int file)
{
  int i = index(rank, file);

  if (board[i].color != NONE)
    return false;
  if (shift == DARK && score[i].dark == 0)
    return false;
  else if (shift == LIGHT && score[i].light == 0)
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
oth_shift_update (int rank, int file)
{
  if (! oth_shift_valid (rank, file))
    return;

  board(rank, file).color = shift;
  oth_board_flip_disks (rank, file);

  switch (shift) {
  case NONE:
    /* Shouldn't happend */
    break;
  case DARK:
    __shift_unset_dark ();
    break;
  case LIGHT:
    __shift_unset_light ();
    break;
  }

  /* start animation */
  glutIdleFunc (oth_flippers_update);
}

/**
 * Stops the animation, resets the board and flippers, and changes current
 * player. If next player has no moves, shift is given back to previous
 * player. However, if both players are out of luck, starts game over 
 * animation.
 */
void
oth_shift_reset ()
{
  glutIdleFunc (NULL);

  oth_board_reset ();
  oth_flippers_reset ();
  
  if (shift == DARK) {

    if (best_light >= 0) {
      shift = LIGHT;
      __shift_set_light ();
    } else if (best_dark >= 0) {
      __shift_set_dark ();
    } else {
      shift = NONE;
      __oth_shift_game_over ();
    }

  } else if (shift == LIGHT) {

    if (best_dark >= 0) {
      shift = DARK;
      __shift_set_dark ();
    } else if (best_light >= 0) {
      __shift_set_light ();
    } else {
      shift = NONE;
      __oth_shift_game_over ();
    }

  } else { /* shift == NONE, Game Over animation done. */
    glutKeyboardFunc (oth_keyboard); /* Allow user to ESC */
    fprintf (stderr, "Game Over!\n");
  }
}

/**
 * Counts points, and set disks for game over animation.
 */
void
__oth_shift_game_over ()
{
  int darks = 0, lights = 0;
  int rank, file;

  /* Count the result */
  for (rank = 0; rank < RANKS; ++rank) {
    for (file = 0; file < FILES; ++file) {
      switch (board(rank, file).color)
	{
	case NONE: break;
	case DARK: darks++; break;
	case LIGHT: lights++; break;
	}
    }
  }

  fprintf (stderr, "Darks: %i, Lights: %i\n", darks, lights);

  /* Flip'em */
  oth_flippers_reset ();

  for (rank = 0; rank < RANKS; ++rank) {
    for (file = 0; file < FILES; ++file) {
      switch (board(rank, file).color)
	{
	case NONE: 
	  break;
	case DARK:
	  if (darks == 0) {
	    /* turn it to light */
	    board(rank, file).color = LIGHT;
	    board(rank, file).flipping = true;
	    board(rank, file).flipper = &flippers[1];
	    lights--;
	  } else {
	    /* just count it */
	    darks--;
	  }
	  break;
	case LIGHT:
	  if (darks > 0) {
	    /* turn it to dark */
	    board(rank, file).color = DARK;
	    board(rank, file).flipping = true;
	    board(rank, file).flipper = &flippers[0];
	    darks--;
	  } else {
	    /* just count it */
	    lights--;
	  }
	  break;
	}
    }
  }
  
  glutIdleFunc (oth_flippers_update);
}


/**
 * Set up callback for computer.
 */
void
__shift_set_comp (void)
{
  glutIdleFunc (oth_comp);
}

/**
 * Undo above.
 */
void
__shift_unset_comp (void)
{
  glutIdleFunc (NULL);
}

/**
 * Set up callbacks for user.
 */
void
__shift_set_user (void)
{
  glutMouseFunc (oth_mouse);
  glutKeyboardFunc (oth_keyboard);
}

/**
 * Undo above.
 */
void
__shift_unset_user (void)
{
  glutMouseFunc (NULL);
  glutKeyboardFunc (NULL); /* [FIXME] Also disables ESC */
}
