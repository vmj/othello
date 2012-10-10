#include <math.h>
#include "camera.h"

/**
 * Initialize camera "subsystem"
 */
Bool
oth_camera_init(Board* board, int *argc, char **argv)
{
        oth_camera_reset(board);
        return true;
}

/**
 * Free any resources taken by camera.
 */
void
oth_camera_free()
{
}

/**
 * Reset camera to it's initial place with initial viewing frustum.
 */
void
oth_camera_reset(Board* board)
{
        /* Most of these just feel suitable */
        cam.angle.v = V_ANGLE_MIN + (V_ANGLE_MAX - V_ANGLE_MIN) / 2;
        cam.angle.h = H_ANGLE_MIN + (H_ANGLE_MAX - H_ANGLE_MIN) / 2;
        cam.radius.x = SQUARESIZE * board->ranks + (SQUARESIZE * 2);
        cam.radius.y = SQUARESIZE * MAX(board->ranks, board->files) + (SQUARESIZE * 2);
        cam.radius.z = SQUARESIZE * board->files + (SQUARESIZE * 2);
        cam.frustum.fov = 70.0;
        cam.frustum.close = 1.0;
        cam.frustum.distant =
                MAX(MAX(cam.radius.x, cam.radius.z), cam.radius.y) * 2;
        cam.at.x = board->ranks * SQUARESIZE / (double)2;
        cam.at.y = 0.0;
        cam.at.z = board->files * SQUARESIZE / (double)2;
        cam.up.x = 0;
        cam.up.y = 1;
        cam.up.z = 0;

        oth_camera_update();
}

/**
 * Calculates eye-coordinates of the camera using the angle information.
 */
inline void
oth_camera_update()
{
        double v_angle = RAD(cam.angle.v);
        double h_angle = RAD(cam.angle.h);

        cam.eye.x = cam.radius.x * cos(v_angle) * cos(h_angle) + cam.at.x;
        cam.eye.y = cam.radius.y * sin(v_angle) + cam.at.y;
        cam.eye.z = cam.radius.z * cos(v_angle) * sin(h_angle) + cam.at.z;
}
