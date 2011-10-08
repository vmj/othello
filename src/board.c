#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "board.h"

/* This is declared in flippers.c */
extern int last_used_flipper;

inline void __board_update_scores (int rank, int file);
inline void __board_update_scores_d (int rank, int file, int r_inc, int f_inc);
inline void __board_flip_disks_d (Color c, 
				  int rank, int file, int r_inc, int f_inc);

/**
 * Initialize board "subsystem".
 */
Bool
oth_board_init (int *argc, char **argv)
{
  int i, rank, file;

  rank = file = -1;
  RANKS = FILES = BOARD_SIZE_DEF;

  for (i = 1; i < *argc; ++i) {
    switch (argv[i][0]) {
    case '-':
      switch (argv[i][1]) {
      case 'r': /* ranks */
        if (++i > *argc) {
          fprintf (stderr, "Argument missing for option 'r'\n");
          return false;
        }
        rank = CLAMP (BOARD_SIZE_MIN, atoi (argv[i]), BOARD_SIZE_MAX);
	break;
      case 'f': /* files */
        if (++i > *argc) {
          fprintf (stderr, "Argument missing for option 'f'\n");
          return false;
        }
        file = CLAMP (BOARD_SIZE_MIN, atoi (argv[i]), BOARD_SIZE_MAX);
	break;
      }
      break;
    }
  }
  
  /* Set RANKS and FILES */
  if ((rank == -1 && file != -1) || (rank != -1 && file == -1)) { /* XOR */
    RANKS = FILES = MAX (rank, file);
  } else {
    RANKS = (rank != -1) ? rank : BOARD_SIZE_DEF;
    FILES = (file != -1) ? file : BOARD_SIZE_DEF;
  }
  
  /* Initialize board */
  i = RANKS * FILES;
  board = (Square *) calloc (i, sizeof(Square));
  if (board == NULL)
    return false;
  while (--i >= 0) {
    board[i].color = NONE;
  }

  /* Initialize score */
  i = RANKS * FILES;
  score = (struct Score *) calloc (i, sizeof(struct Score));
  if (score == NULL)
    return false;

  /* Set starting positions */
  board( RANKS / 2    , FILES / 2     ).color = LIGHT;
  board( RANKS / 2    , FILES / 2 - 1 ).color = DARK;
  board( RANKS / 2 - 1, FILES / 2     ).color = DARK;
  board( RANKS / 2 - 1, FILES / 2 - 1 ).color = LIGHT;

  /* Initialize scores and flipper pointers */
  oth_board_reset ();
  return true;
}

/**
 * Free resources taken by board.
 */
void
oth_board_free ()
{
  if (board)
    free (board);
  if (score)
    free (score);
}

/**
 *
 */
void
oth_board_reset ()
{
  register int i, rank, file;
  int best_score_d = 0, best_score_l = 0;

  /* Reset "bests" */
  best_dark = best_light = -1;

  for (rank = 0; rank < RANKS; ++rank) {
    for (file = 0; file < FILES; ++file) {
      i = index(rank, file);
      board[i].flipping = false;
      score[i].light = 0;
      score[i].dark  = 0;

      /* Update scores and "bests" */
      if (board[i].color == NONE) {
	__board_update_scores (rank, file);
	
	if (score[i].dark > best_score_d) {
	  best_dark = i;
	  best_score_d = score[i].dark;
	}
	if (score[i].light > best_score_l) {
	  best_light = i;
	  best_score_l = score[i].light;
	}
      }
    }
  }
}

/**
 * Helper function for above.
 */
inline 
void
__board_update_scores (int rank, int file)
{
  __board_update_scores_d (rank, file,  1,  0); /* N  */
  __board_update_scores_d (rank, file,  1,  1); /* NE */
  __board_update_scores_d (rank, file,  0,  1); /* E  */
  __board_update_scores_d (rank, file, -1,  1); /* SE */  
  __board_update_scores_d (rank, file, -1,  0); /* S  */
  __board_update_scores_d (rank, file, -1, -1); /* SW */
  __board_update_scores_d (rank, file,  0, -1); /* W  */
  __board_update_scores_d (rank, file,  1, -1); /* NW */
}

/**
 * Helper function for above. Counts the score in one direction.
 */
inline 
void
__board_update_scores_d (int rank, int file, int r_inc, int f_inc)
{
  int i = index(rank, file); 
  int r = rank + r_inc;
  int f = file + f_inc;
  int score_dark = 0, score_light = 0;
  Bool no_darks = true, no_lights = true;
  
  while (r >= 0 && r < RANKS && f >= 0 && f < FILES && (no_darks || no_lights))
    {
      switch (board(r, f).color)
	{
	case NONE:
	  goto DONE; /* break away */
	  break;
	case DARK:
	  no_darks = false;
	  if (no_lights) ++score_light;
	  r += r_inc;
	  f += f_inc;      
	  break;
	case LIGHT:
	  no_lights = false;
	  if (no_darks) ++score_dark;
	  r += r_inc;
	  f += f_inc;      
	  break;
	}
    }

 DONE:
  if (! no_darks)  score[i].dark  += score_dark;
  if (! no_lights) score[i].light += score_light;
}

/**
 * Sets disks for flipping around given square.
 */
void
oth_board_flip_disks (int rank, int file)
{
  Color c = board(rank, file).color;

  last_used_flipper = 0;

  if (c == NONE)
    return;

  __board_flip_disks_d (c, rank, file,  1,  0); /* N  */
  __board_flip_disks_d (c, rank, file,  1,  1); /* NE */
  __board_flip_disks_d (c, rank, file,  0,  1); /* E  */
  __board_flip_disks_d (c, rank, file, -1,  1); /* SE */
  __board_flip_disks_d (c, rank, file, -1,  0); /* S  */
  __board_flip_disks_d (c, rank, file, -1, -1); /* SW */
  __board_flip_disks_d (c, rank, file,  0, -1); /* W  */
  __board_flip_disks_d (c, rank, file,  1, -1); /* NW */
}

/**
 * Helper function for above. Does it in one direction.
 */
inline
void
__board_flip_disks_d (Color c, int rank, int file, int r_inc, int f_inc)
{
  int flipper;
  register int i;
  register int r = rank + r_inc;
  register int f = file + f_inc;
  Bool found = false;

  /* Find same color in direction (r_inc, f_inc) */
  while ( r >= 0 && r < RANKS && f >= 0 && f < FILES ) 
    {
      i = index(r, f);
      if (board[i].color == c) {
	found = 1;
	break; /* Got it! */
      }
      if (board[i].color == NONE)
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
  i = index(r, f);
  while (board[i].color != c) {
    board[i].color = c;
    board[i].flipping = true;
    board[i].flipper = &flippers[flipper];

    last_used_flipper = MAX (flipper, last_used_flipper);

    flipper++;
    r += r_inc;
    f += f_inc;
    i = index(r, f);
  }
}
