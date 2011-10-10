#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "common.h"
#include "camera.h"
//#include "board.h"
//#include "shift.h"
//#include "display.h"
#include "keyboard.h"

/**
 * GLUT callback to handle arrow keys.
 */
void
oth_special_keys(int key, int x, int y)
{
        /* Update angles */
        switch (key)
        {
        case GLUT_KEY_UP:
                cam.angle.v += V_ANGLE_INCREMENT;
                break;
        case GLUT_KEY_DOWN:
                cam.angle.v -= V_ANGLE_INCREMENT;
                break;
        case GLUT_KEY_RIGHT:
                cam.angle.h -=
                        ((H_ANGLE_INCREMENT_MAX -
                          H_ANGLE_INCREMENT_MIN) / V_ANGLE_MAX) *
                        cam.angle.v + H_ANGLE_INCREMENT_MIN;
                break;
        case GLUT_KEY_LEFT:
                cam.angle.h +=
                        ((H_ANGLE_INCREMENT_MAX -
                          H_ANGLE_INCREMENT_MIN) / V_ANGLE_MAX) *
                        cam.angle.v + H_ANGLE_INCREMENT_MIN;
                break;
        default:
                return;
        }

        /* Clamp values to allowed ranges (or repeat) */
        if (V_ANGLE_MIN == 0 && V_ANGLE_MAX == 360)
        {
                if (cam.angle.v > 360)
                        cam.angle.v -= 360;
                else if (cam.angle.v < 0)
                        cam.angle.v += 360;
        }
        else
        {
                cam.angle.v =
                        CLAMP(V_ANGLE_MIN, cam.angle.v, V_ANGLE_MAX);
        }

        if (H_ANGLE_MIN == 0 && H_ANGLE_MAX == 360)
        {
                if (cam.angle.h > 360)
                        cam.angle.h -= 360;
                else if (cam.angle.h < 0)
                        cam.angle.h += 360;
        }
        else
        {
                cam.angle.h =
                        CLAMP(H_ANGLE_MIN, cam.angle.h, H_ANGLE_MAX);
        }

        // [TODO] if (cam.angle.v == 90 || cam.angle.v == 270)
        // [TODO]   do something for the 'up' vector

        oth_camera_update();
        glutPostRedisplay();
}

/**
 * GLUT callback to handle keyboard.
 */
void
oth_keyboard(unsigned char key, int x, int y)
{
        if (key == 27 /* ESC */ )
                exit(0);
}
