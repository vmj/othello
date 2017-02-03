#include <math.h>
#include <stdlib.h>
#include "common.h"
#include "camera.h"

/* Structure to control viewing parameters. Angles are relative to center of
 * the board and controlled via arrow keys.
 */
struct Camera {
    struct {
        double fov, close, distant;
    } frustum;
    struct {
        float v, h;
    } angle;
    struct {
        float x, y, z;
    } radius;
    struct {
        float x, y, z;
    } eye;
    struct {
        float x, y, z;
    } at;
    struct {
        float x, y, z;
    } up;
};

/* Limits for camera angles and how much they increment/decrement. Horizontal
 * increments are calculated as a function of vertical angle.
 */
#define V_ANGLE_MIN              0
#define V_ANGLE_MAX              89

#define H_ANGLE_MIN              0
#define H_ANGLE_MAX              360

#define V_ANGLE_INCREMENT        1
#define H_ANGLE_INCREMENT_MIN    1      /* when looking from V_ANGLE_MIN */
#define H_ANGLE_INCREMENT_MAX   10      /* when looking from V_ANGLE_MAX */

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

/**
 * Moves camera up.
 */
void
oth_camera_move_up(Camera* camera)
{
        camera->angle.v += V_ANGLE_INCREMENT;

        /* Clamp values to allowed ranges (or repeat) */
        if (V_ANGLE_MIN == 0 && V_ANGLE_MAX == 360)
        {
                if (camera->angle.v > 360)
                        camera->angle.v -= 360;
        }
        else
        {
                camera->angle.v =
                        CLAMP(V_ANGLE_MIN, camera->angle.v, V_ANGLE_MAX);
        }

        /* [TODO] if (camera->angle.v == 90 || camera->angle.v == 270) */
        /* [TODO]   do something for the 'up' vector */

        oth_camera_update(camera);
}

/**
 * Moves camera down.
 */
void
oth_camera_move_down(Camera* camera)
{
        camera->angle.v -= V_ANGLE_INCREMENT;

        /* Clamp values to allowed ranges (or repeat) */
        if (V_ANGLE_MIN == 0 && V_ANGLE_MAX == 360)
        {
                if (camera->angle.v < 0)
                        camera->angle.v += 360;
        }
        else
        {
                camera->angle.v =
                        CLAMP(V_ANGLE_MIN, camera->angle.v, V_ANGLE_MAX);
        }

        /* [TODO] if (camera->angle.v == 90 || camera->angle.v == 270) */
        /* [TODO]   do something for the 'up' vector */

        oth_camera_update(camera);
}

/**
 * Moves camera left.
 */
void
oth_camera_move_left(Camera* camera)
{
        camera->angle.h +=
                ((H_ANGLE_INCREMENT_MAX -
                  H_ANGLE_INCREMENT_MIN) / V_ANGLE_MAX) *
                camera->angle.v + H_ANGLE_INCREMENT_MIN;

        if (H_ANGLE_MIN == 0 && H_ANGLE_MAX == 360)
        {
                if (camera->angle.h > 360)
                        camera->angle.h -= 360;
        }
        else
        {
                camera->angle.h =
                        CLAMP(H_ANGLE_MIN, camera->angle.h, H_ANGLE_MAX);
        }

        oth_camera_update(camera);
}

/**
 * Moves camera right.
 */
void
oth_camera_move_right(Camera* camera)
{
        camera->angle.h -=
                ((H_ANGLE_INCREMENT_MAX -
                  H_ANGLE_INCREMENT_MIN) / V_ANGLE_MAX) *
                camera->angle.v + H_ANGLE_INCREMENT_MIN;


        if (H_ANGLE_MIN == 0 && H_ANGLE_MAX == 360)
        {
                if (camera->angle.h < 0)
                        camera->angle.h += 360;
        }
        else
        {
                camera->angle.h =
                        CLAMP(H_ANGLE_MIN, camera->angle.h, H_ANGLE_MAX);
        }

        oth_camera_update(camera);
}

double
oth_camera_frustum_fov(Camera* camera)
{
        return camera->frustum.fov;
}

double
oth_camera_frustum_close(Camera* camera)
{
        return camera->frustum.close;
}

double
oth_camera_frustum_distant(Camera* camera)
{
        return camera->frustum.distant;
}

float
oth_camera_angle_v(Camera* camera)
{
        return camera->angle.v;
}

float
oth_camera_angle_h(Camera* camera)
{
        return camera->angle.h;
}

float
oth_camera_radius_x(Camera* camera)
{
        return camera->radius.x;
}

float
oth_camera_radius_y(Camera* camera)
{
        return camera->radius.y;
}

float
oth_camera_radius_z(Camera* camera)
{
        return camera->radius.z;
}

float
oth_camera_eye_x(Camera* camera)
{
        return camera->eye.x;
}

float
oth_camera_eye_y(Camera* camera)
{
        return camera->eye.y;
}

float
oth_camera_eye_z(Camera* camera)
{
        return camera->eye.z;
}

float
oth_camera_at_x(Camera* camera)
{
        return camera->at.x;
}

float
oth_camera_at_y(Camera* camera)
{
        return camera->at.y;
}

float
oth_camera_at_z(Camera* camera)
{
        return camera->at.z;
}

float
oth_camera_up_x(Camera* camera)
{
        return camera->up.x;
}

float
oth_camera_up_y(Camera* camera)
{
        return camera->up.y;
}

float
oth_camera_up_z(Camera* camera)
{
        return camera->up.z;
}
