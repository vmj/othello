#include <GL/glut.h>
#include "global.h"
#include "common.h"
#include "camera.h"
#include "flippers.h"
#include "shift.h"
#include "display.h"

/* Displaylist IDs */
#define BOARD 1
#define DISK  2

/* Lights */
GLfloat light0_position[] = { 0.00, 0.00, 0.00, 1.0 };
GLfloat light0_direction[] = { 0.00, 0.00, 0.00 };

GLfloat light0_ambient[] = { 0.90, 0.90, 0.90, 1.0 };
GLfloat light0_diffuse[] = { 0.90, 0.90, 0.90, 1.0 };
GLfloat light0_specular[] = { 1.00, 1.00, 1.00, 1.0 };

GLfloat light0_cutoff = 50;
GLfloat light0_exponent = 40.0;

GLfloat light1_position[] = { 0.00, 0.00, 0.00, 1.0 };
GLfloat light1_direction[] = { 0.00, 0.00, 0.00 };

GLfloat light1_ambient[] = { 0.90, 0.90, 0.90, 1.0 };
GLfloat light1_diffuse[] = { 0.90, 0.90, 0.90, 1.0 };
GLfloat light1_specular[] = { 1.00, 1.00, 1.00, 1.0 };

GLfloat light1_cutoff = 50;
GLfloat light1_exponent = 40.0;

GLfloat light2_position[] = { 0.00, 0.00, 0.00, 1.0 };
GLfloat light2_direction[] = { 0.00, 0.00, 0.00 };

GLfloat light2_ambient[] = { 0.60, 0.60, 0.60, 1.0 };
GLfloat light2_diffuse[] = { 0.60, 0.60, 0.60, 1.0 };
GLfloat light2_specular[] = { 0.00, 0.00, 0.00, 1.0 };

GLfloat light2_cutoff = 50;
GLfloat light2_exponent = 40.0;

GLfloat light3_position[] = { 0.00, 0.00, 0.00, 1.0 };
GLfloat light3_direction[] = { 0.00, 0.00, 0.00 };

GLfloat light3_ambient[] = { 0.60, 0.60, 0.60, 1.0 };
GLfloat light3_diffuse[] = { 0.60, 0.60, 0.60, 1.0 };
GLfloat light3_specular[] = { 0.00, 0.00, 0.00, 1.0 };

GLfloat light3_cutoff = 50;
GLfloat light3_exponent = 40.0;

/* Fog */
GLfloat fog_density = 0.2;
GLfloat fog_color[] = { 0.00, 0.00, 0.00, 0.00 };

/* Materials (board) */
GLfloat boardD_ambient[] = { 0.10, 0.35, 0.10, 1.0 };
GLfloat boardD_diffuse[] = { 0.00, 0.00, 0.00, 1.0 };
GLfloat boardD_specular[] = { 0.00, 0.00, 0.00, 1.0 };

GLfloat boardD_shininess = 0.0;

GLfloat boardL_ambient[] = { 0.15, 0.45, 0.15, 1.0 };
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
oth_display_init(Board* board, Camera* camera, int *argc, char **argv)
{
        const int h = 100000;
        const int k = CLAMP(0, 50, SQUARESIZE);
        float r, f, i, j;
        int rank, file;
        GLUquadric *quad;
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHT3);
        glEnable(GL_NORMALIZE);

        /* Setup lighting */
        light0_position[0] = board->ranks * SQUARESIZE;
        light0_position[1] = oth_camera_radius_y(camera);
        light0_position[2] = board->files * SQUARESIZE;

        light0_direction[0] = -light0_position[0] / 4;
        light0_direction[1] = -light0_position[1];
        light0_direction[2] = -light0_position[2] / 4;

        glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light0_cutoff);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, light0_exponent);

        light1_position[0] = 0.0;
        light1_position[1] = oth_camera_radius_y(camera);
        light1_position[2] = 0.0;

        light1_direction[0] = (board->ranks * SQUARESIZE) / (double)4;
        light1_direction[1] = -light1_position[1];
        light1_direction[2] = (board->files * SQUARESIZE) / (double)4;

        glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_cutoff);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light1_exponent);

        light2_position[0] = board->ranks * SQUARESIZE;
        light2_position[1] = oth_camera_radius_y(camera);
        light2_position[2] = 0.0;

        light2_direction[0] = -light2_position[0] / 4;
        light2_direction[1] = -light2_position[1];
        light2_direction[2] = light2_position[2] / 4;

        glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light2_cutoff);
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, light2_exponent);

        light3_position[0] = 0.0;
        light3_position[1] = oth_camera_radius_y(camera);
        light3_position[2] = board->files * SQUARESIZE;

        light3_direction[0] = light3_position[0] / 4;
        light3_direction[1] = -light3_position[1];
        light3_direction[2] = -light3_position[2] / 4;

        glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
        glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, light3_cutoff);
        glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, light3_exponent);

        /* Create displaylist for board */
        glNewList(BOARD, GL_COMPILE);

        /* Light squares */
        glMaterialfv(GL_FRONT, GL_AMBIENT, boardL_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, boardL_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, boardL_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, boardL_shininess);
        glBegin(GL_QUADS);
        for (rank = 0; rank < board->ranks; rank += 2)
        {
                r = rank * SQUARESIZE;
                for (file = 0; file < board->files; file += 2)
                {
                        f = file * SQUARESIZE;
                        for (i = 0; i < SQUARESIZE; i += k)
                        {
                                for (j = 0; j < SQUARESIZE; j += k)
                                {
                                        glVertex3f(r + i    , -DISKHEIGHT, f + j    );
                                        glVertex3f(r + i    , -DISKHEIGHT, f + j + k);
                                        glVertex3f(r + i + k, -DISKHEIGHT, f + j + k);
                                        glVertex3f(r + i + k, -DISKHEIGHT, f + j    );

                                        glVertex3f(r + i     + SQUARESIZE, -DISKHEIGHT, f + j     + SQUARESIZE);
                                        glVertex3f(r + i     + SQUARESIZE, -DISKHEIGHT, f + j + k + SQUARESIZE);
                                        glVertex3f(r + i + k + SQUARESIZE, -DISKHEIGHT, f + j + k + SQUARESIZE);
                                        glVertex3f(r + i + k + SQUARESIZE, -DISKHEIGHT, f + j     + SQUARESIZE);
                                }
                        }
                }
        }
        glEnd();

        /* Dark squares */
        glMaterialfv(GL_FRONT, GL_AMBIENT, boardD_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, boardD_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, boardD_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, boardD_shininess);
        glBegin(GL_QUADS);
        for (rank = 0; rank < board->ranks; rank += 2)
        {
                r = rank * SQUARESIZE;
                for (file = 0; file < board->files; file += 2)
                {
                        f = file * SQUARESIZE;
                        for (i = 0; i < SQUARESIZE; i += k)
                        {
                                for (j = 0; j < SQUARESIZE; j += k)
                                {
                                        glVertex3f(r + i     + SQUARESIZE, -DISKHEIGHT, f + j    );
                                        glVertex3f(r + i     + SQUARESIZE, -DISKHEIGHT, f + j + k);
                                        glVertex3f(r + i + k + SQUARESIZE, -DISKHEIGHT, f + j + k);
                                        glVertex3f(r + i + k + SQUARESIZE, -DISKHEIGHT, f + j    );

                                        glVertex3f(r + i    , -DISKHEIGHT, f + j     + SQUARESIZE);
                                        glVertex3f(r + i    , -DISKHEIGHT, f + j + k + SQUARESIZE);
                                        glVertex3f(r + i + k, -DISKHEIGHT, f + j + k + SQUARESIZE);
                                        glVertex3f(r + i + k, -DISKHEIGHT, f + j     + SQUARESIZE);
                                }
                        }
                }
        }
        glEnd();

        /* Sides (with dark material) */
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fog_color);
        glFogf(GL_FOG_START, oth_camera_radius_y(camera));
        glFogf(GL_FOG_END, oth_camera_radius_y(camera) * 2);
        for (i = 0; i < h; i += k)
        {
                glBegin(GL_QUAD_STRIP);
                glVertex3f(0.0               , -DISKHEIGHT - i    , 0.0);
                glVertex3f(0.0               , -DISKHEIGHT - i - k, 0.0);
                glVertex3f(0.0               , -DISKHEIGHT - i    , board->files * SQUARESIZE);
                glVertex3f(0.0               , -DISKHEIGHT - i - k, board->files * SQUARESIZE);
                glVertex3f(board->ranks * SQUARESIZE, -DISKHEIGHT - i    , board->files * SQUARESIZE);
                glVertex3f(board->ranks * SQUARESIZE, -DISKHEIGHT - i - k, board->files * SQUARESIZE);
                glVertex3f(board->ranks * SQUARESIZE, -DISKHEIGHT - i    , 0.0);
                glVertex3f(board->ranks * SQUARESIZE, -DISKHEIGHT - i - k, 0.0);
                glVertex3f(0.0               , -DISKHEIGHT - i    , 0.0);
                glVertex3f(0.0               , -DISKHEIGHT - i - k, 0.0);
                glEnd();
        }
        glDisable(GL_FOG);
        glEndList();

        /* Create displaylist for disk */
        quad = gluNewQuadric();
        gluQuadricOrientation(quad, GLU_OUTSIDE);
        gluQuadricDrawStyle(quad, GLU_FILL);

        glNewList(DISK, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT, dark_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, dark_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, disk_shininess);
        glTranslatef(SQUARESIZE / (double)2, 0.0, SQUARESIZE / (double)2);
        glScalef(1.0, DISKSQUASH, 1.0);
        gluSphere(quad, DISKRADIUS, 24, 24);
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
        Board* board = current_board;
        Camera* camera = current_camera;
        Square* square = NULL;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();

        gluLookAt(oth_camera_eye_x(camera), oth_camera_eye_y(camera), oth_camera_eye_z(camera),
                  oth_camera_at_x(camera), oth_camera_at_y(camera), oth_camera_at_z(camera),
                  oth_camera_up_x(camera), oth_camera_up_y(camera), oth_camera_up_z(camera));

        glGetIntegerv(GL_RENDER_MODE, mode);

        if (mode[0] == GL_RENDER)
        {
                glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
                glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);
                glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
                glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);
                glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
                glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
                glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
                glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_direction);

                glCallList(BOARD);

                for (rank = 0; rank < board->ranks; ++rank)
                {
                        for (file = 0; file < board->files; ++file)
                        {
                                square = board(board, rank, file);

                                if (square->disk == EMPTY)
                                        continue;

                                glPushMatrix();

                                /* Move to next square */
                                glTranslatef(rank * SQUARESIZE, 0.0,
                                             file * SQUARESIZE);

                                /* Draw disk (flipping) */
                                if (square->flipping == true)
                                {
                                        p = square->flipper;
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
                                        if (square->disk == BLACK)
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
                for (rank = 0; rank < board->ranks; ++rank)
                {
                        x1 = rank * SQUARESIZE;
                        x2 = x1 + SQUARESIZE;

                        for (file = 0; file < board->files; ++file)
                        {
                                i = index(board, rank, file);
                                square = &(board->squares[i]);

                                if (square->disk != EMPTY)
                                        continue;
                                if (shift == LIGHT && square->score.light == 0)
                                        continue;
                                if (shift == DARK && square->score.dark == 0)
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
        Camera *camera = current_camera;

        if (width == 0 || height == 0)
                return;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(oth_camera_frustum_fov(camera), 1.0,
                       oth_camera_frustum_close(camera), oth_camera_frustum_distant(camera));

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (width <= height)
                glViewport(0, (height - width) / 2, width, width);
        else
                glViewport((width - height) / 2, 0, height, height);
}
