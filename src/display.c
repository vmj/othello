#include <GL/glut.h>
#include "common.h"
#include "board.h"
#include "camera.h"
#include "flippers.h"
#include "shift.h"
#include "display.h"

/* Displaylist IDs */
#define BOARD 1
#define DISK  2

/* Lights */
GLfloat light0_position[4];

GLfloat light0_direction[] = { 0.0, -1.0, 0.0 };
GLfloat light0_ambient[] = { 1.00, 1.00, 1.00, 1.0 };
GLfloat light0_diffuse[] = { 1.00, 1.00, 1.00, 1.0 };
GLfloat light0_specular[] = { 1.00, 1.00, 1.00, 1.0 };

GLfloat light0_cutoff = 180;

GLfloat light1_position[4];

GLfloat light1_direction[] = { 0.0, 1.0, 0.0 };
GLfloat light1_ambient[] = { 0.50, 0.50, 0.50, 1.0 };
GLfloat light1_diffuse[] = { 0.50, 0.50, 0.50, 1.0 };
GLfloat light1_specular[] = { 0.00, 0.00, 0.00, 1.0 };

GLfloat light1_cutoff = 180;

/* Materials (board) */
GLfloat boardD_ambient[] = { 0.10, 0.25, 0.10, 1.0 };
GLfloat boardD_diffuse[] = { 0.00, 0.00, 0.00, 1.0 };
GLfloat boardD_specular[] = { 0.00, 0.00, 0.00, 1.0 };

GLfloat boardD_shininess = 0.0;

GLfloat boardL_ambient[] = { 0.10, 0.30, 0.10, 1.0 };
GLfloat boardL_diffuse[] = { 0.00, 0.00, 0.00, 1.0 };
GLfloat boardL_specular[] = { 0.00, 0.00, 0.00, 1.0 };

GLfloat boardL_shininess = 0.0;

/* Materials (disks) */
GLfloat dark_ambient[] = { 0.10, 0.10, 0.10, 1.0 };
GLfloat dark_diffuse[] = { 0.20, 0.20, 0.20, 1.0 };
GLfloat dark_specular[] = { 1.00, 1.00, 1.00, 1.0 };

GLfloat light_ambient[] = { 0.10, 0.10, 0.10, 1.0 };
GLfloat light_diffuse[] = { 0.80, 0.80, 0.80, 1.0 };
GLfloat light_specular[] = { 1.00, 1.00, 1.00, 1.0 };

GLfloat disk_shininess = 128.0;

/**
 * Initialize display "subsystem"
 */
Bool
oth_display_init(int *argc, char **argv)
{
        float r, f;
        int rank, file;
        GLUquadric *quad;
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_NORMALIZE);

        /* Setup lighting */
        light0_position[0] = RANKS * SQUARESIZE;
        light0_position[1] = cam.radius.y;
        light0_position[2] = FILES * SQUARESIZE;
        light0_position[3] = 0.0;

        glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light0_cutoff);

        light1_position[0] = 0.0;
        light1_position[1] = -cam.radius.y;
        light1_position[2] = 0.0;
        light1_position[3] = 0.0;

        glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_cutoff);

        /* Create displaylist for board */
        glNewList(BOARD, GL_COMPILE);

        /* Light squares */
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, boardL_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, boardL_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, boardL_specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, boardL_shininess);
        glBegin(GL_QUADS);
        for (rank = 0; rank < RANKS; rank += 2)
        {
                r = rank * SQUARESIZE;
                for (file = 0; file < FILES; file += 2)
                {
                        f = file * SQUARESIZE;
                        glVertex3f(r, -DISKHEIGHT, f);
                        glVertex3f(r, -DISKHEIGHT, f + SQUARESIZE);
                        glVertex3f(r + SQUARESIZE, -DISKHEIGHT,
                                   f + SQUARESIZE);
                        glVertex3f(r + SQUARESIZE, -DISKHEIGHT, f);

                        glVertex3f(r + SQUARESIZE, -DISKHEIGHT,
                                   f + SQUARESIZE);
                        glVertex3f(r + SQUARESIZE, -DISKHEIGHT,
                                   f + SQUARESIZE * 2);
                        glVertex3f(r + SQUARESIZE * 2, -DISKHEIGHT,
                                   f + SQUARESIZE * 2);
                        glVertex3f(r + SQUARESIZE * 2, -DISKHEIGHT,
                                   f + SQUARESIZE);
                }
        }
        glEnd();

        /* Dark squares */
        glMaterialfv(GL_FRONT, GL_AMBIENT, boardD_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, boardD_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, boardD_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, boardD_shininess);
        glBegin(GL_QUADS);
        for (rank = 0; rank < RANKS; rank += 2)
        {
                r = rank * SQUARESIZE;
                for (file = 0; file < FILES; file += 2)
                {
                        f = file * SQUARESIZE;
                        glVertex3f(r + SQUARESIZE, -DISKHEIGHT, f);
                        glVertex3f(r + SQUARESIZE, -DISKHEIGHT,
                                   f + SQUARESIZE);
                        glVertex3f(r + SQUARESIZE * 2, -DISKHEIGHT,
                                   f + SQUARESIZE);
                        glVertex3f(r + SQUARESIZE * 2, -DISKHEIGHT, f);

                        glVertex3f(r, -DISKHEIGHT, f + SQUARESIZE);
                        glVertex3f(r, -DISKHEIGHT, f + SQUARESIZE * 2);
                        glVertex3f(r + SQUARESIZE, -DISKHEIGHT,
                                   f + SQUARESIZE * 2);
                        glVertex3f(r + SQUARESIZE, -DISKHEIGHT,
                                   f + SQUARESIZE);
                }
        }
        glEnd();
        /* Sides (with dark material) */
        glBegin(GL_QUAD_STRIP);
        glVertex3f(0.0, -DISKHEIGHT, 0.0);
        glVertex3f(0.0, -DISKHEIGHT - 20, 0.0);
        glVertex3f(0.0, -DISKHEIGHT, FILES * SQUARESIZE);
        glVertex3f(0.0, -DISKHEIGHT - 20, FILES * SQUARESIZE);
        glVertex3f(RANKS * SQUARESIZE, -DISKHEIGHT, FILES * SQUARESIZE);
        glVertex3f(RANKS * SQUARESIZE, -DISKHEIGHT - 20,
                   FILES * SQUARESIZE);
        glVertex3f(RANKS * SQUARESIZE, -DISKHEIGHT, 0.0);
        glVertex3f(RANKS * SQUARESIZE, -DISKHEIGHT - 20, 0.0);
        glVertex3f(0.0, -DISKHEIGHT, 0.0);
        glVertex3f(0.0, -DISKHEIGHT - 20, 0.0);
        glEnd();
        glEndList();

        /* Create displaylist for disk */
        quad = gluNewQuadric();
        gluQuadricOrientation(quad, GLU_OUTSIDE);
        gluQuadricDrawStyle(quad, GLU_FILL);

        glNewList(DISK, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT, dark_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, dark_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, disk_shininess);
        glTranslatef(SQUARESIZE / 2, 0.0, SQUARESIZE / 2);
        glScalef(1.0, DISKSQUASH, 1.0);
        gluSphere(quad, DISKRADIUS, 12, 12);
        glEndList();

        return true;
}

/**
 * Free any resources taken by display
 */
void
oth_display_free()
{
        glDeleteLists(BOARD, 1);
        glDeleteLists(DISK, 1);
}

/**
 * Reset display.
 */
void
oth_display_reset()
{
}

/**
 * GLUT callback to redraw scene.
 */
void
oth_display(void)
{
        Flipper *p;
        int rank, file, x1, x2, z1, z2, i;
        GLint mode[1];

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();

        gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z,
                  cam.at.x, cam.at.y, cam.at.z,
                  cam.up.x, cam.up.y, cam.up.z);

        glGetIntegerv(GL_RENDER_MODE, mode);

        if (mode[0] == GL_RENDER)
        {
                glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
                glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

                glCallList(BOARD);

                for (rank = 0; rank < RANKS; ++rank)
                {
                        for (file = 0; file < FILES; ++file)
                        {

                                i = index(rank, file);
                                if (board[i].disk == EMPTY)
                                        continue;

                                glPushMatrix();

                                /* Move to next square */
                                glTranslatef(rank * SQUARESIZE, 0.0,
                                             file * SQUARESIZE);

                                /* Draw disk (flipping) */
                                if (board[i].flipping == true)
                                {
                                        p = board[i].flipper;
                                        glTranslatef(p->translation.x,
                                                     p->translation.y,
                                                     p->translation.z);
                                        glRotatef(p->rotation.a,
                                                  p->rotation.x,
                                                  p->rotation.y,
                                                  p->rotation.z);
                                        glMaterialfv(GL_FRONT, GL_DIFFUSE,
                                                     p->diffuse);
                                }
                                /* Draw disk (stationary) */
                                else
                                {
                                        if (board[i].disk == BLACK)
                                        {
                                                glMaterialfv(GL_FRONT,
                                                             GL_DIFFUSE,
                                                             dark_diffuse);
                                        }
                                        else
                                        {       /* WHITE */
                                                glMaterialfv(GL_FRONT,
                                                             GL_DIFFUSE,
                                                             light_diffuse);
                                        }
                                }

                                glCallList(DISK);

                                glPopMatrix();
                        }
                }

        }
        else
        {                       /* mode[0] == GL_SELECTION */

                /* Draw selection squares */
                for (rank = 0; rank < RANKS; ++rank)
                {
                        x1 = rank * SQUARESIZE;
                        x2 = x1 + SQUARESIZE;

                        for (file = 0; file < FILES; ++file)
                        {

                                i = index(rank, file);

                                if (board[i].disk != EMPTY)
                                        continue;
                                if (shift == LIGHT && score[i].light == 0)
                                        continue;
                                if (shift == DARK && score[i].dark == 0)
                                        continue;

                                z1 = file * SQUARESIZE;
                                z2 = z1 + SQUARESIZE;

                                glPushName(i);
                                glBegin(GL_POLYGON);
                                glVertex3f(x1, -DISKHEIGHT / 2, z1);
                                glVertex3f(x1, -DISKHEIGHT / 2, z2);
                                glVertex3f(x2, -DISKHEIGHT / 2, z2);
                                glVertex3f(x2, -DISKHEIGHT / 2, z1);
                                glEnd();
                                glPopName();
                        }
                }

        }

        glPopMatrix();
        glutSwapBuffers();
}                               /* oth_display */

/**
 * GLUT callback to recalculate viewport.
 */
void
oth_reshape(int width, int height)
{
        if (width == 0 || height == 0)
                return;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(cam.frustum.fov, 1.0,
                       cam.frustum.close, cam.frustum.distant);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (width <= height)
                glViewport(0, (height - width) / 2, width, width);
        else
                glViewport((width - height) / 2, 0, height, height);
}
