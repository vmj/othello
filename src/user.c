#include <stdio.h>
#include <GL/glut.h>
#include "global.h"
#include "camera.h"
#include "shift.h"
#include "display.h"
#include "board.h"
#include "user.h"

/**
 * GLUT callback to handle mouse.
 */
void
oth_mouse(int button, int state, int x, int y)
{
        int r, f;
        GLint hits;
        GLuint name_stack[100]; // [FIXME] unnecesserily big ?
        GLint viewport[4];
        Board* board = current_board;
        Camera* camera = current_camera;

        if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
                return;

        glGetIntegerv(GL_VIEWPORT, viewport);

        /* Set up viewing for selection mode */
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPickMatrix(x, viewport[3] - (y - viewport[1] * 2), 2, 2,
                      viewport);
        gluPerspective(oth_camera_frustum_fov(camera), 1.0, oth_camera_frustum_close(camera),
                       oth_camera_frustum_distant(camera));
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        /* Initialize name stack and enter selection mode */
        glInitNames();
        glSelectBuffer(100, name_stack);        // [FIXME]
        glRenderMode(GL_SELECT);

        /* Force redraw */
        oth_display();

        /* Restore viewing conditions */
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glFlush();

        /* Validate choice */
        hits = glRenderMode(GL_RENDER);
        if (hits != 1)
                return;
        r = rank(board, name_stack[3]);
        f = file(board, name_stack[3]);
        if (!oth_shift_valid(board, r, f))
        {
                r = f = -1;     /* invalid choice */
                return;
        }

        /* Process valid choice */
        oth_shift_update(board, r, f);
        glutPostRedisplay();
}
