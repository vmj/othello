#include <stdio.h>
#include <GL/glut.h>
#include "common.h"
#include "camera.h"
#include "board.h"
#include "shift.h"
#include "display.h"
#include "input.h"

/* Users choice via keyboard. These are reset if user uses mouse. */
int rank = -1;
int file = -1;

/**
 * GLUT callback to handle arrow keys.
 */
void
oth_special_keys (int key, int x, int y)
{
  /* Update angles */
  switch (key) {
  case GLUT_KEY_UP:
    cam.angle.v += V_ANGLE_INCREMENT;
    break;
  case GLUT_KEY_DOWN:
    cam.angle.v -= V_ANGLE_INCREMENT;
    break;
  case GLUT_KEY_RIGHT:
    cam.angle.h -= ((H_ANGLE_INCREMENT_MAX - H_ANGLE_INCREMENT_MIN) 
		    / V_ANGLE_MAX) * cam.angle.v + H_ANGLE_INCREMENT_MIN;
    break;
  case GLUT_KEY_LEFT:
    cam.angle.h += ((H_ANGLE_INCREMENT_MAX - H_ANGLE_INCREMENT_MIN) 
		    / V_ANGLE_MAX) * cam.angle.v + H_ANGLE_INCREMENT_MIN;
    break;
  default:
    return;
  }
  
  /* Clamp values to allowed ranges (or repeat) */
  if (V_ANGLE_MIN == 0 && V_ANGLE_MAX == 360) {
    if (cam.angle.v > 360)    cam.angle.v -= 360;
    else if (cam.angle.v < 0) cam.angle.v += 360;
  } else {
    cam.angle.v = CLAMP (V_ANGLE_MIN, cam.angle.v, V_ANGLE_MAX);
  }
  
  if (H_ANGLE_MIN == 0 && H_ANGLE_MAX == 360) {
    if (cam.angle.h > 360)    cam.angle.h -= 360;
    else if (cam.angle.h < 0) cam.angle.h += 360;
  } else {
    cam.angle.h = CLAMP (H_ANGLE_MIN, cam.angle.h, H_ANGLE_MAX);
  }
  
  // [TODO] if (cam.angle.v == 90 || cam.angle.v == 270)
  // [TODO]   do something for the 'up' vector
  
  oth_camera_update ();
  glutPostRedisplay ();
}

/**
 * GLUT callback to handle keyboard.
 */
void
oth_keyboard (unsigned char key, int x, int y) /* Does not work !!! */
{
  int i;
  /* This function has a problem: if user chooses file first (a-z), she can
   * only choose ranks from 1-9. Too hard a problem to solve now. Just use
   * mouse for now :)
   */
  if (key == 27 /* ESC */) exit (0);

  /* ranks 0 - 25 (one digit at a time) */
  if ( key >= '0' && key <= '9' ) {
    i = key - 47 - 2; // ASCII of '0' is 48, -2 becouse '1' should yeld 0
    if (rank == -1) rank = i;
    else rank = 10 * rank + i;

    if (rank < 0 || rank >= RANKS ) {
      fprintf (stderr, "Keyboard: throwing away rank (%i)\n", rank);
      rank = -1;
      return;
    }
    fprintf (stderr, "Keyboard: rank %i (key %c)\n", rank, key);
    if (file == -1)
      return; /* wait for more input */
  }

  /* files a - z and A - Z (0 - 25) */
  else if (key >= 'a' && key <= 'z') {
    file = key - 96 - 1; // ASCII of 'a' is 97
    if (file < 0 || file >= FILES) {
      fprintf (stderr, "Keyboard: throwing away file (%i)\n", file);
      file = -1;
      return;
    }
    fprintf (stderr, "Keyboard: file %i (key %c)\n", file, key);
    if (rank == -1)
      return; /* wait for more input */
  } else if (key >= 'A' && key <= 'Z') {
    file = key - 64 - 1; // ASCII of 'A' is... yes, 65.
    if (file < 0 || file >= FILES) {
      fprintf (stderr, "Keyboard: throwing away file (%i)\n", file);
      file = -1;
      return;
    }
    fprintf (stderr, "Keyboard: file %i (key %c)\n", file, key);
    if (rank == -1)
      return; /* wait for more input */

  } else {
    return;
  }

  if ( ! oth_shift_valid (rank, file)) {
    fprintf (stderr, "Keyboard: reseting\n", file, key);
    rank = file = -1; /* invalid choice */
    return;
  }

  oth_shift_update (rank, file);
  glutPostRedisplay ();
}

/**
 * GLUT callback to handle mouse.
 */
void
oth_mouse (int button, int state, int x, int y)
{
  int r, f;
  GLint hits;
  GLuint name_stack[100]; // [FIXME] unnecesserily big ?
  GLint viewport[4];
  
  if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
    return;
  
  glGetIntegerv (GL_VIEWPORT, viewport);

  /* Set up viewing for selection mode */
  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();
  gluPickMatrix (x, viewport[3] - y, 2, 2, viewport);
  gluPerspective ( cam.frustum.fov, 1.0, 
		   cam.frustum.close, cam.frustum.distant);
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity ();
  
  /* Initialize name stack and enter selection mode */
  glInitNames ();
  glSelectBuffer (100, name_stack); // [FIXME]
  glRenderMode (GL_SELECT);
  
  /* Force redraw */
  oth_display ();
  
  /* Restore viewing conditions */
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();
  glFlush ();
  
  /* Validate choice */
  hits = glRenderMode (GL_RENDER);
  if (hits != 1)
    return;
  r = rank(name_stack[3]);
  f = file(name_stack[3]);
  if ( ! oth_shift_valid (r, f)) {
    r = f = -1; /* invalid choice */
    return;
  }

  /* Process valid choice (reset keyboard variables) */
  rank = -1;
  file = -1;
  oth_shift_update (r, f);
  glutPostRedisplay ();
}
