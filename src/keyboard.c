#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "global.h"
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
        Camera* camera = current_camera;

        /* Update angles */
        switch (key)
        {
        case GLUT_KEY_UP:
                camera->angle.v += V_ANGLE_INCREMENT;
                break;
        case GLUT_KEY_DOWN:
                camera->angle.v -= V_ANGLE_INCREMENT;
                break;
        case GLUT_KEY_RIGHT:
                camera->angle.h -=
                        ((H_ANGLE_INCREMENT_MAX -
                          H_ANGLE_INCREMENT_MIN) / V_ANGLE_MAX) *
                        camera->angle.v + H_ANGLE_INCREMENT_MIN;
                break;
        case GLUT_KEY_LEFT:
                camera->angle.h +=
                        ((H_ANGLE_INCREMENT_MAX -
                          H_ANGLE_INCREMENT_MIN) / V_ANGLE_MAX) *
                        camera->angle.v + H_ANGLE_INCREMENT_MIN;
                break;
        default:
                return;
        }

        /* Clamp values to allowed ranges (or repeat) */
        if (V_ANGLE_MIN == 0 && V_ANGLE_MAX == 360)
        {
                if (camera->angle.v > 360)
                        camera->angle.v -= 360;
                else if (camera->angle.v < 0)
                        camera->angle.v += 360;
        }
        else
        {
                camera->angle.v =
                        CLAMP(V_ANGLE_MIN, camera->angle.v, V_ANGLE_MAX);
        }

        if (H_ANGLE_MIN == 0 && H_ANGLE_MAX == 360)
        {
                if (camera->angle.h > 360)
                        camera->angle.h -= 360;
                else if (camera->angle.h < 0)
                        camera->angle.h += 360;
        }
        else
        {
                camera->angle.h =
                        CLAMP(H_ANGLE_MIN, camera->angle.h, H_ANGLE_MAX);
        }

        // [TODO] if (camera->angle.v == 90 || camera->angle.v == 270)
        // [TODO]   do something for the 'up' vector

        oth_camera_update(camera);
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
