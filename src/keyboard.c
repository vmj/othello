#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#include "global.h"
#include "common.h"
#include "camera.h"
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
                oth_camera_move_up(camera);
                break;
        case GLUT_KEY_DOWN:
                oth_camera_move_down(camera);
                break;
        case GLUT_KEY_RIGHT:
                oth_camera_move_right(camera);
                break;
        case GLUT_KEY_LEFT:
                oth_camera_move_left(camera);
                break;
        default:
                return;
        }

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
