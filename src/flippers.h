#ifndef _OTHELLO_FLIPPERS_H_
#define _OTHELLO_FLIPPERS_H_
#include "common.h"

/* Controls how many step the animation has (for one disk) */
#define PRECISION_MIN    10
#define PRECISION_MAX   180

/* This controls the flipping animation. */
typedef struct {
  struct { float x, y, z; } translation;
  struct { float a, x, y, z; } rotation;
  /* materials */
  float ambient[4];
  float diffuse[4];
  float specular[4];
} Flipper;

/* Array of those to support wave-animation */
Flipper *flippers;

Bool oth_flippers_init (int *argc, char **argv);
void oth_flippers_free ();
void oth_flippers_reset ();

void oth_flippers_update (void);

#endif /* _OTHELLO_FLIPPERS_H_ */
