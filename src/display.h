#ifndef _OTHELLO_DISPLAY_H_
#define _OTHELLO_DISPLAY_H_
#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#include "common.h"
#include "board.h"

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

Bool   oth_display_init    (Board* board,
                            Camera* camera,
                            int *argc,
                            char **argv);
void   oth_display_free    (void);
void   oth_display_reset   (void);

void   oth_display         (void);
void   oth_reshape         (int width,
                            int height);

#endif                          /* _OTHELLO_DISPLAY_H_ */
