#include <stdio.h>
#include <GL/glut.h>
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

        if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
                return;

        glGetIntegerv(GL_VIEWPORT, viewport);

        /* Set up viewing for selection mode */
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPickMatrix(x, viewport[3] - (y - viewport[1] * 2), 2, 2,
                      viewport);
        gluPerspective(cam.frustum.fov, 1.0, cam.frustum.close,
                       cam.frustum.distant);
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
        r = rank(name_stack[3]);
        f = file(name_stack[3]);
        if (!oth_shift_valid(r, f))
        {
                r = f = -1;     /* invalid choice */
                return;
        }

        /* Process valid choice */
        oth_shift_update(r, f);
        glutPostRedisplay();
}
