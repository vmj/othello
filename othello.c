/* 
   Othello by Mikko Varri (mikko.varri@iglu.net).

   November, December 2003 for CS2150 Computer Graphics.
   
   Introduction
   ============
   
   Objective of the game is to conquer the board with your color. Players place
   disks on the board in turn such that they trap at least on of the opponents'
   disks between their own disks. Those trapped disks are turned upside down so
   that they change color. If the player can't place a disk on the board, shift
   is transfered to the opponent. If neither player can play, game is over.
   Player with more disks is the winner.
   
   Command line options
   ====================
   
   -2               Human versus human mode.
   -p NUM           Specify how precise the flipping animation
                    should be. By default, it's calculated as a
                    function of board size.
   -r NUM           Specify the board size (or just how many ranks there should
                    be, if also -f is given).
   -f NUM           Specify the board size (or just how many files there should
                    be, if also -r is given).
   -h               Print a help message with more info on the limits for above
                    options.

   Interaction
   ===========

   Disks are place on the board with mouse. Keyboard is also in use, but isn't
   very usable. Just click the square you want to place the disk. If nothing
   happens, disk cannot be placed into that square.

   You can move the camera around the board to some extent using the arrow
   keys.

   ESC quits the game.

 */
/* ============================================================= [common.h] =*/
#ifndef _OTHELLO_COMMON_H_
#define _OTHELLO_COMMON_H_

#ifdef WIN32
#define inline
#endif

#ifdef __c99
#  include <stdbool.h>
#  define Bool   _Bool
#else
#  define Bool   char
#  define false  (char) 0
#  define true   (char) 1
#endif

#define MIN(a,b)      ( (a) < (b) ? (a) : (b) )
#define MAX(a,b)      ( (a) > (b) ? (a) : (b) )

#define CLAMP(a,b,c)  ( (a) < (b) ? MIN (b,c) : (a) )

#define PI            (3.141592654)            /* (acos (-1.0))    */
#define RAD(a)	      ((a) * 0.01745329252)    /* ((a) * PI / 180) */

#endif /* _OTHELLO_COMMON_H_ */
/* ============================================================== [comp.h] = */
#ifndef _OTHELLO_COMP_H_
#define _OTHELLO_COMP_H_

void oth_comp (void);

#endif /* _OTHELLO_COMP_H_ */
/* ============================================================= [input.h] = */
#ifndef _OTHELLO_INPUT_H_
#define _OTHELLO_INPUT_H_

void oth_keyboard (unsigned char key, int x, int y);
void oth_special_keys (int key, int x, int y);
void oth_mouse (int button, int state, int x, int y);

#endif /* _OTHELLO_INPUT_H_ */
/* ============================================================ [camera.h] = */
#ifndef _OTHELLO_CAMERA_H_
#define _OTHELLO_CAMERA_H_

/* Structure to control viewing parameters. Angles are relative to center of
 * the board and controlled via arrow keys.
 */
struct Camera {
  struct { double fov, close, distant; } frustum;
  struct { float v, h; } angle;
  struct { float x, y, z; } radius;
  struct { float x, y, z; } eye;
  struct { float x, y, z; } at;
  struct { float x, y, z; } up;
} cam;

/* Limits for camera angles and how much they increment/decrement. Horizontal
 * increments are calculated as a function of vertical angle.
 */
#define V_ANGLE_MIN              0
#define V_ANGLE_MAX              89

#define H_ANGLE_MIN              90
#define H_ANGLE_MAX              270

#define V_ANGLE_INCREMENT        1
#define H_ANGLE_INCREMENT_MIN    1          /* when looking from V_ANGLE_MIN */
#define H_ANGLE_INCREMENT_MAX   10          /* when looking from V_ANGLE_MAX */

Bool oth_camera_init (int *argc, char **argv);
void oth_camera_free ();
void oth_camera_reset ();

inline
void oth_camera_update ();

#endif
/* =========================================================== [display.h] = */
#ifndef _OTHELLO_DISPLAY_H_
#define _OTHELLO_DISPLAY_H_
#include <GL/glut.h>
//#include "common.h"

/* Disks are squashed spheres, here's their info. */
#define DISKSQUASH 0.33
#define DISKRADIUS (SQUARESIZE * 0.45)
#define DISKHEIGHT (DISKRADIUS * DISKSQUASH)

/* Disk materials. Also needed in flippers.c, thus here. */
GLfloat dark_ambient[4];
GLfloat dark_diffuse[4];
GLfloat dark_specular[4];
GLfloat light_ambient[4];
GLfloat light_diffuse[4];
GLfloat light_specular[4];  

Bool oth_display_init (int *argc, char **argv);
void oth_display_free ();
void oth_display_reset ();

void oth_display (void);
void oth_reshape (int width, int height);

#endif /* _OTHELLO_DISPLAY_H_ */
/*============================================================= [flipper.h] =*/
#ifndef _OTHELLO_FLIPPERS_H_
#define _OTHELLO_FLIPPERS_H_
//#include "common.h"

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
/*=============================================================== [board.h] =*/
#ifndef _OTHELLO_BOARD_H_
#define _OTHELLO_BOARD_H_
//#include "common.h"
//#include "flippers.h" // Flipper

/* Disks are all the same, so their color is here separately. Also shift info
 * uses this.
 */
typedef enum { NONE, DARK, LIGHT } Color;

/* One square on the board. */
typedef struct {
  Color    color;
  Bool     flipping;
  Flipper *flipper;
} Square;

/* Serves as a measure of unit, more than anything else. */
#define SQUARESIZE 50

/* Array of Squares */
Square *board;

/* Limits for board size. */
#define BOARD_SIZE_MIN  4
#define BOARD_SIZE_MAX 25
#define BOARD_SIZE_DEF  8

/* Actual board size. User can define them using -r and -f options. */
int RANKS;
int FILES;

/*  */
struct Score {
  int dark;
  int light;
} *score;

/* Indexes of the "best" moves */
int best_dark;
int best_light;

/* Macros to convert between index and (rank, file) information. */
#define rank(index)          ((index) / (FILES))
#define file(index)          ((index) % (FILES))
#define index(rank, file)    ((FILES) * (rank) + (file))

/* Macros to access board and score arrays as if they were two dimensional. */
#define board(rank, file)    (board[ index((rank), (file)) ])
#define score(rank, file)    (score[ index((rank), (file)) ])

Bool oth_board_init (int *argc, char **argv);
void oth_board_free ();
void oth_board_reset ();

void oth_board_flip_disks (int rank, int file);

#endif /* _OTHELLO_BOARD_H_ */
/*============================================================== [shift.h] = */
#ifndef _OTHELLO_SHIFT_H_
#define _OTHELLO_SHIFT_H_
//#include "common.h"
//#include "board.h"   // Color

/* Whose turn? It's NONE if game is over. */
Color shift;

Bool oth_shift_init (int *argc, char **argv);
void oth_shift_free ();
void oth_shift_reset ();

Bool oth_shift_valid (int rank, int file);
void oth_shift_update ();

#endif /* _OTHELLO_SHIFT_H_ */
/* ============================================================== [comp.c] = */
//#include "board.h"
//#include "shift.h"
//#include "comp.h"

/**
 * Very simple function to get computer play Othello.
 */
void
oth_comp (void)
{
  int rank, file;
  
  /* Who Am I ? */
  if (shift == DARK) {
    rank = rank(best_dark);
    file = file(best_dark);
  } else { /* LIGHT */
    rank = rank(best_light);
    file = file(best_light);
  }

  oth_shift_update (rank, file);
}

/* ============================================================= [input.c] = */
#include <stdio.h>
#include <GL/glut.h>
//#include "common.h"
//#include "camera.h"
//#include "board.h"
//#include "shift.h"
//#include "display.h"
//#include "input.h"

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
/* ============================================================ [camera.c] = */
#include <math.h>
//#include "board.h"  // FILES, RANKS, SQUARESIZE
//#include "camera.h"

/**
 * Initialize camera "subsystem"
 */
Bool
oth_camera_init (int *argc, char **argv)
{
  oth_camera_reset ();
  return true;
}

/**
 * Free any resources taken by camera.
 */
void
oth_camera_free ()
{
}

/**
 * Reset camera to it's initial place with initial viewing frustum.
 */
void
oth_camera_reset ()
{
  /* Most of these just feel suitable */
  cam.angle.v = V_ANGLE_MIN + (V_ANGLE_MAX - V_ANGLE_MIN) / 2 ;
  cam.angle.h = H_ANGLE_MIN + (H_ANGLE_MAX - H_ANGLE_MIN) / 2 ;
  cam.radius.x = SQUARESIZE * RANKS              + (SQUARESIZE * 2);
  cam.radius.y = SQUARESIZE * MAX (RANKS, FILES) + (SQUARESIZE * 2);
  cam.radius.z = SQUARESIZE * FILES              + (SQUARESIZE * 2);
  cam.frustum.fov = 70.0;
  cam.frustum.close = 1.0;
  cam.frustum.distant = MAX(MAX(cam.radius.x, cam.radius.z), cam.radius.y) * 2;
  cam.at.x = RANKS * SQUARESIZE / 2;
  cam.at.y = 0.0;
  cam.at.z = FILES * SQUARESIZE / 2;
  cam.up.x = 0;
  cam.up.y = 1;
  cam.up.z = 0;

  oth_camera_update ();
}

/**
 * Calculates eye-coordinates of the camera using the angle information.
 */
inline
void
oth_camera_update ()
{
  double v_angle = RAD(cam.angle.v);
  double h_angle = RAD(cam.angle.h);

  cam.eye.x = cam.radius.x * cos (v_angle) * cos (h_angle) + cam.at.x;
  cam.eye.y = cam.radius.y * sin (v_angle)                 + cam.at.y;
  cam.eye.z = cam.radius.z * cos (v_angle) * sin (h_angle) + cam.at.z;
}
/* =========================================================== [display.c] = */
#include <GL/glut.h>
//#include "common.h"
//#include "board.h"
//#include "camera.h"
//#include "flippers.h"
//#include "shift.h"
//#include "display.h"

/* Displaylist IDs */
#define BOARD 1
#define DISK  2

/* Lights */
GLfloat light0_position[4];

GLfloat light0_direction[]= {0.0, -1.0, 0.0};
GLfloat light0_ambient[]  = {1.00, 1.00, 1.00, 1.0};
GLfloat light0_diffuse[]  = {1.00, 1.00, 1.00, 1.0};
GLfloat light0_specular[] = {1.00, 1.00, 1.00, 1.0};
GLfloat light0_cutoff     = 180;

GLfloat light1_position[4];

GLfloat light1_direction[]= {0.0, 1.0, 0.0};
GLfloat light1_ambient[]  = {0.50, 0.50, 0.50, 1.0};
GLfloat light1_diffuse[]  = {0.50, 0.50, 0.50, 1.0};
GLfloat light1_specular[] = {0.00, 0.00, 0.00, 1.0};
GLfloat light1_cutoff     = 180;

/* Materials (board) */
GLfloat boardD_ambient[]   = {0.10, 0.25, 0.10, 1.0};
GLfloat boardD_diffuse[]   = {0.20, 0.85, 0.20, 1.0};
GLfloat boardD_specular[]  = {0.20, 0.45, 0.20, 1.0};
GLfloat boardD_shininess   = 0.0;

GLfloat boardL_ambient[]   = {0.10, 0.30, 0.10, 1.0};
GLfloat boardL_diffuse[]   = {0.20, 0.90, 0.20, 1.0};
GLfloat boardL_specular[]  = {0.20, 0.50, 0.20, 1.0};
GLfloat boardL_shininess   = 0.0;

/* Materials (disks) */
GLfloat dark_ambient[]    = {0.10, 0.10, 0.10, 1.0};
GLfloat dark_diffuse[]    = {0.20, 0.20, 0.20, 1.0};
GLfloat dark_specular[]   = {1.00, 1.00, 1.00, 1.0};

GLfloat light_ambient[]   = {0.10, 0.10, 0.10, 1.0};
GLfloat light_diffuse[]   = {0.80, 0.80, 0.80, 1.0};
GLfloat light_specular[]  = {1.00, 1.00, 1.00, 1.0};  

GLfloat disk_shininess    = 128.0;

/**
 * Initialize display "subsystem"
 */
Bool
oth_display_init (int *argc, char **argv)
{
  float r, f;
  int rank, file;
  GLUquadric *quad;
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_LIGHT1);
  glEnable (GL_NORMALIZE);
  glEnable (GL_POLYGON_SMOOTH);

  /* Setup lighting */
  light0_position[0] = RANKS * SQUARESIZE;
  light0_position[1] = cam.radius.y;
  light0_position[2] = FILES * SQUARESIZE;
  light0_position[3] = 0.0;

  glLightfv (GL_LIGHT0, GL_AMBIENT,        light0_ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE,        light0_diffuse);
  glLightfv (GL_LIGHT0, GL_SPECULAR,       light0_specular);
  glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
  glLightf  (GL_LIGHT0, GL_SPOT_CUTOFF,    light0_cutoff);

  light1_position[0] = 0.0;
  light1_position[1] = - cam.radius.y;
  light1_position[2] = 0.0;
  light1_position[3] = 0.0;

  glLightfv (GL_LIGHT1, GL_AMBIENT,        light1_ambient);
  glLightfv (GL_LIGHT1, GL_DIFFUSE,        light1_diffuse);
  glLightfv (GL_LIGHT1, GL_SPECULAR,       light1_specular);
  glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
  glLightf  (GL_LIGHT1, GL_SPOT_CUTOFF,    light1_cutoff);

  /* Create displaylist for board */
  glNewList (BOARD, GL_COMPILE);

  /* Light squares */
  glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,   boardL_ambient);
  glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,   boardL_diffuse);
  glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  boardL_specular);
  glMaterialf  (GL_FRONT_AND_BACK, GL_SHININESS, boardL_shininess);
  glBegin (GL_QUADS);
    for (rank = 0; rank < RANKS ; rank += 2) {
      r = rank * SQUARESIZE;
      for (file = 0; file < FILES; file += 2) {
	f = file * SQUARESIZE;
	glVertex3f (r,              -DISKHEIGHT, f);
	glVertex3f (r,              -DISKHEIGHT, f + SQUARESIZE);
	glVertex3f (r + SQUARESIZE, -DISKHEIGHT, f + SQUARESIZE);
	glVertex3f (r + SQUARESIZE, -DISKHEIGHT, f);
	
	glVertex3f (r + SQUARESIZE,     -DISKHEIGHT, f + SQUARESIZE);
	glVertex3f (r + SQUARESIZE,     -DISKHEIGHT, f + SQUARESIZE * 2);
	glVertex3f (r + SQUARESIZE * 2, -DISKHEIGHT, f + SQUARESIZE * 2);
	glVertex3f (r + SQUARESIZE * 2, -DISKHEIGHT, f + SQUARESIZE);
      }
    }
  glEnd ();

  /* Dark squares */
  glMaterialfv (GL_FRONT, GL_AMBIENT,   boardD_ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE,   boardD_diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR,  boardD_specular);
  glMaterialf  (GL_FRONT, GL_SHININESS, boardD_shininess);
  glBegin (GL_QUADS);
  for (rank = 0; rank < RANKS ; rank += 2) {
    r = rank * SQUARESIZE;
    for (file = 0; file < FILES; file += 2) {
      f = file * SQUARESIZE;
      glVertex3f (r + SQUARESIZE,     -DISKHEIGHT, f);
      glVertex3f (r + SQUARESIZE,     -DISKHEIGHT, f + SQUARESIZE);
      glVertex3f (r + SQUARESIZE * 2, -DISKHEIGHT, f + SQUARESIZE);
      glVertex3f (r + SQUARESIZE * 2, -DISKHEIGHT, f);
      
      glVertex3f (r,              -DISKHEIGHT, f + SQUARESIZE);
      glVertex3f (r,              -DISKHEIGHT, f + SQUARESIZE * 2);
      glVertex3f (r + SQUARESIZE, -DISKHEIGHT, f + SQUARESIZE * 2);
      glVertex3f (r + SQUARESIZE, -DISKHEIGHT, f + SQUARESIZE);
    }
  }
  glEnd ();
  /* Sides (with dark material) */
  glBegin (GL_QUAD_STRIP);
  glVertex3f (0.0,                -DISKHEIGHT,      0.0);
  glVertex3f (0.0,                -DISKHEIGHT - 20, 0.0);
  glVertex3f (0.0,                -DISKHEIGHT,      FILES * SQUARESIZE);
  glVertex3f (0.0,                -DISKHEIGHT - 20, FILES * SQUARESIZE);
  glVertex3f (RANKS * SQUARESIZE, -DISKHEIGHT,      FILES * SQUARESIZE);
  glVertex3f (RANKS * SQUARESIZE, -DISKHEIGHT - 20, FILES * SQUARESIZE);
  glVertex3f (RANKS * SQUARESIZE, -DISKHEIGHT,      0.0);
  glVertex3f (RANKS * SQUARESIZE, -DISKHEIGHT - 20, 0.0);
  glVertex3f (0.0,                -DISKHEIGHT,      0.0);
  glVertex3f (0.0,                -DISKHEIGHT - 20, 0.0);
  glEnd ();
  glEndList ();
  
  /* Create displaylist for disk */
  quad = gluNewQuadric ();
  gluQuadricOrientation (quad, GLU_OUTSIDE);
  gluQuadricDrawStyle (quad, GLU_FILL);
  gluQuadricNormals (quad, GLU_SMOOTH);
  
  glNewList (DISK, GL_COMPILE);
  glMaterialfv (GL_FRONT, GL_AMBIENT, dark_ambient);
  glMaterialfv (GL_FRONT, GL_SPECULAR, dark_specular);
  glMaterialf (GL_FRONT, GL_SHININESS, disk_shininess);
  glTranslatef (SQUARESIZE/2, 0.0, SQUARESIZE/2);
  glScalef (1.0, DISKSQUASH, 1.0);
  gluSphere (quad, DISKRADIUS, 12, 12);
  glEndList ();

  return true;
}

/**
 * Free any resources taken by display
 */
void
oth_display_free ()
{
  glDeleteLists (BOARD, 1);
  glDeleteLists (DISK, 1);
}

/**
 * Reset display.
 */
void
oth_display_reset ()
{
}

/**
 * GLUT callback to redraw scene.
 */
void
oth_display (void)
{
  Flipper *p;
  int rank, file, x1, x2, z1, z2, i;
  GLint mode[1];

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix ();

  gluLookAt (cam.eye.x, cam.eye.y, cam.eye.z,
	     cam.at.x,  cam.at.y,  cam.at.z,
	     cam.up.x,  cam.up.y,  cam.up.z);
  
  glGetIntegerv (GL_RENDER_MODE, mode);

  if (mode[0] == GL_RENDER) {
    glLightfv (GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv (GL_LIGHT1, GL_POSITION, light1_position);

    glCallList (BOARD);

    for (rank = 0; rank < RANKS; ++rank) {
      for (file = 0; file < FILES; ++file) {

	i = index(rank, file);
	if (board[i].color == NONE)
	  continue;
	
	glPushMatrix ();

	/* Move to next square */
	glTranslatef (rank * SQUARESIZE, 0.0, file * SQUARESIZE);

	/* Draw disk (flipping) */
	if (board[i].flipping == true) {
	  p = board[i].flipper;
	  glTranslatef (p->translation.x, p->translation.y, p->translation.z);
	  glRotatef (p->rotation.a, p->rotation.x,p->rotation.y,p->rotation.z);
	  glMaterialfv (GL_FRONT, GL_DIFFUSE,  p->diffuse);
	}
	/* Draw disk (stationary) */
	else {
	  if (board[i].color == DARK) {
	    glMaterialfv (GL_FRONT, GL_DIFFUSE, dark_diffuse);
	  } else {           /* LIGHT */
	    glMaterialfv (GL_FRONT, GL_DIFFUSE, light_diffuse);
	  }
	}

	glCallList (DISK);

	glPopMatrix ();
      }
    }

  } else { /* mode[0] == GL_SELECTION */

    /* Draw selection squares */
    for (rank = 0; rank < RANKS; ++rank) {
      x1 = rank * SQUARESIZE;
      x2 = x1 + SQUARESIZE;
      
      for (file = 0; file < FILES; ++file) {

	i = index(rank, file);

	if (board[i].color != NONE)
	  continue;
	if (shift == LIGHT && score[i].light == 0)
	  continue;
	if (shift == DARK  && score[i].dark  == 0)
	  continue;

	z1 = file * SQUARESIZE;
	z2 = z1 + SQUARESIZE;
	
	glPushName (i);
	glBegin (GL_POLYGON);
	glVertex3f (x1, -DISKHEIGHT/2, z1);
	glVertex3f (x1, -DISKHEIGHT/2, z2);
	glVertex3f (x2, -DISKHEIGHT/2, z2);
	glVertex3f (x2, -DISKHEIGHT/2, z1);
	glEnd ();
	glPopName ();
      }
    }

  }

  glPopMatrix ();
  glutSwapBuffers();
} /* oth_display */


/**
 * GLUT callback to recalculate viewport.
 */
void
oth_reshape (int width, int height)
{
  if (width == 0 || height == 0)
    return;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective ( cam.frustum.fov, 1.0, 
		   cam.frustum.close, cam.frustum.distant);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (width <= height)
    glViewport (0, (height - width) / 2, width, width);
  else
    glViewport ((width - height) / 2, 0, height, height);
}
/* ========================================================== [flippers.c] = */
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
//#include "common.h"
//#include "board.h"    // RANKS, FILES, BOARD_SIZE_*
//#include "display.h"  // materials (light_* and dark_*)
//#include "shift.h"    // shift
//#include "flippers.h"

/* We need this many flippers */
#define FLIPPERS        (MAX (RANKS, FILES) - 2)
 
/* This is to jump out of the animation early when only few disks are flipping.
 * Index of the last used Flipper in *flippers. Maintained by 
 * oth_board_flip_disks and used by oth_flippers_update. 
 */
int last_used_flipper;

/* Controls how many step the animation has (for one disk) */
unsigned char precision;
#define PRECISION_MIN    10
#define PRECISION_MAX   180

/* This is used to hold step sizes */
Flipper stepper;

/**
 * Initialize flippers "subsystem".
 */
Bool 
oth_flippers_init (int *argc, char **argv)
{
  int i;
  double a, b;
  precision = 0;

  for (i = 1; i < *argc; ++i) {
    switch (argv[i][0]) {
    case '-':
      switch (argv[i][1]) {
      case 'p':
        if (++i > *argc) {
          fprintf (stderr, "Argument missing for option 'p'\n");
          return false;
        }
	precision = (unsigned char) 
	  CLAMP (PRECISION_MIN, atoi (argv[i]), PRECISION_MAX);
      }
      break;
    }
  }

  /* Define precision as a function of board size (ax+b) */
  if (precision == 0) {
    a = ((PRECISION_MIN - PRECISION_MAX) 
	 / ((double) BOARD_SIZE_MAX - BOARD_SIZE_MIN));
    b = 180 - BOARD_SIZE_MIN * a;
    precision = (unsigned char) (a * ((RANKS + FILES) / 2.0) + b);
  }

  /* Initialize flippers */
  flippers = (Flipper *) calloc (FLIPPERS, sizeof(Flipper));
  if (flippers == NULL) {
    fprintf (stderr, "Memory exhaustion.\n");
    return false;
  }

  /* Initialize stepper */
  stepper.translation.x = (float) SQUARESIZE / precision;
  stepper.rotation.a = (float) 180 / precision;
  for (i = 0; i < 3; ++i) {
    stepper.ambient[i]  = dark_ambient[i];
    stepper.diffuse[i]  = 
      ((float) light_diffuse[i]  - dark_diffuse[i] ) / precision;
    stepper.specular[i] = dark_specular[i];
  } 

  oth_flippers_reset ();
  return true;
}

/**
 * Free resources taken by flippers.
 */
void
oth_flippers_free ()
{
  if (flippers)
    free (flippers);
}

/**
 * Reset flippers to their starting values.
 */
void
oth_flippers_reset ()
{
  register int i, j;
  int f = FLIPPERS;

  /* reset starting values */
  for (i = 0; i < f; ++i) {
    flippers[i].translation.x = 0.0f;
    flippers[i].rotation.a = 0.0f;
    flippers[i].rotation.z = 1.0f;
    for (j = 0; j < 3; ++j) {
      if (shift == DARK) {
	flippers[i].diffuse[j] = dark_diffuse[j];
      } else if (shift == LIGHT) {
	flippers[i].diffuse[j] = light_diffuse[j];
      } else { /* NONE (Game Over) */
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
oth_flippers_update (void)
{
  Flipper *p;
  int flipper, i;
  /* j controls the wave effect. */
  static int j = -1;

  /* Initialize j (first pass) */
  if (j == -1) {
    j = FLIPPERS - 1;
  }

  /* Start flipping next disk? */
  if (flippers[FLIPPERS - j - 1].rotation.a >= (180 / FLIPPERS)) {
    j = (j > 0) ? j - 1 : 0;
  }

  /* Update flippers */
  if (shift != NONE) {
    for (flipper = 0; flipper < FLIPPERS - j ; ++flipper) {
      p = &flippers[flipper];
      switch (shift)
	{
	case DARK:
	  p->translation.x = 
	    MIN (p->translation.x + stepper.translation.x, SQUARESIZE);
	  p->rotation.a = MIN (p->rotation.a + stepper.rotation.a, 180);
	  for (i = 0; i < 3; ++i) {
	    p->diffuse[i]  = 
	      MAX (p->diffuse[i] - stepper.diffuse[i], dark_diffuse[i]);
	  }
	  break;
	case LIGHT:
	  p->translation.x = 
	    MIN (p->translation.x + stepper.translation.x, SQUARESIZE);
	  p->rotation.a = MIN (p->rotation.a + stepper.rotation.a, 180);
	  for (i = 0; i < 3; ++i) {
	    p->diffuse[i]  = 
	      MIN (p->diffuse[i] + stepper.diffuse[i], light_diffuse[i]);
	  }
	  break;
	}
    }
  } else { /* Game Over animation */
    last_used_flipper = 1;
    /* Flip darks */
    flippers[0].translation.x =
      MIN (flippers[0].translation.x + stepper.translation.x, 
	   SQUARESIZE);
    flippers[0].rotation.a =
      MIN (flippers[0].rotation.a + stepper.rotation.a, 
	   180);
    for (i = 0; i < 3; ++i) {
      flippers[0].diffuse[i] = 
	MAX (flippers[0].diffuse[i] - stepper.diffuse[i], dark_diffuse[i]);
    }
    /* Flip lights */
    flippers[1].translation.x =
      MIN (flippers[1].translation.x + stepper.translation.x,
	   SQUARESIZE);
    flippers[1].rotation.a =
      MIN (flippers[1].rotation.a + stepper.rotation.a, 
	   180);
    for (i = 0; i < 3; ++i) {
      flippers[1].diffuse[i] = 
	MIN (flippers[1].diffuse[i] + stepper.diffuse[i], light_diffuse[i]);
    }
  }
  
  /* Resign from idle function (last pass) */
  if (flippers[last_used_flipper].rotation.a >= 180) {
    j = -1;
    oth_shift_reset ();
  }

  glutPostRedisplay ();
}
/* ============================================================= [board.c] = */
#include <stdio.h>
#include <stdlib.h>
//#include "common.h"
//#include "board.h"

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
/* ============================================================= [shift.c] = */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
//#include "common.h"
//#include "comp.h"
//#include "board.h"
//#include "flippers.h"
//#include "input.h"
//#include "shift.h"

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
/* ============================================================== [main.c] = */
#include <stdio.h>
#include <GL/glut.h>
//#include "display.h"
//#include "input.h"
//#include "board.h"
//#include "flippers.h"
//#include "camera.h"
//#include "shift.h"

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
