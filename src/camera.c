#include <math.h>
#include <stdlib.h>
#include "common.h"
#include "camera.h"

/**
 * Initialize camera "subsystem"
 */
Camera*
oth_camera_init(Board* board, int *argc, char **argv)
{
        Camera* camera = (Camera*) calloc(1, sizeof(Camera));
        if (!camera)
                return NULL;
        oth_camera_reset(camera, board);
        return camera;
}

/**
 * Free any resources taken by camera.
 */
void
oth_camera_free(Camera* camera)
{
        if (camera)
                free(camera);
}

/**
 * Reset camera to it's initial place with initial viewing frustum.
 */
void
oth_camera_reset(Camera* camera, Board* board)
{
        /* Most of these just feel suitable */
        camera->angle.v = V_ANGLE_MIN + (V_ANGLE_MAX - V_ANGLE_MIN) / 2;
        camera->angle.h = H_ANGLE_MIN + (H_ANGLE_MAX - H_ANGLE_MIN) / 2;
        camera->radius.x = SQUARESIZE * board->ranks + (SQUARESIZE * 2);
        camera->radius.y = SQUARESIZE * MAX(board->ranks, board->files) + (SQUARESIZE * 2);
        camera->radius.z = SQUARESIZE * board->files + (SQUARESIZE * 2);
        camera->frustum.fov = 70.0;
        camera->frustum.close = 1.0;
        camera->frustum.distant =
                MAX(MAX(camera->radius.x, camera->radius.z), camera->radius.y) * 2;
        camera->at.x = board->ranks * SQUARESIZE / (double)2;
        camera->at.y = 0.0;
        camera->at.z = board->files * SQUARESIZE / (double)2;
        camera->up.x = 0;
        camera->up.y = 1;
        camera->up.z = 0;

        oth_camera_update(camera);
}

/**
 * Calculates eye-coordinates of the camera using the angle information.
 */
void
oth_camera_update(Camera* camera)
{
        double v_angle = RAD(camera->angle.v);
        double h_angle = RAD(camera->angle.h);

        camera->eye.x = camera->radius.x * cos(v_angle) * cos(h_angle) + camera->at.x;
        camera->eye.y = camera->radius.y * sin(v_angle) + camera->at.y;
        camera->eye.z = camera->radius.z * cos(v_angle) * sin(h_angle) + camera->at.z;
}
