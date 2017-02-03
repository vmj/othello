#ifndef _OTHELLO_CAMERA_H_
#define _OTHELLO_CAMERA_H_

#include "common.h"
#include "board.h"

typedef struct Camera Camera;

Camera*   oth_camera_init    (Board* board,
                              int *argc,
                              char **argv);
void      oth_camera_free    (Camera* camera);
void      oth_camera_reset   (Camera* camera,
                              Board* board);

void      oth_camera_update  (Camera* camera);

void      oth_camera_move_up    (Camera* camera);
void      oth_camera_move_down  (Camera* camera);
void      oth_camera_move_left  (Camera* camera);
void      oth_camera_move_right (Camera* camera);

double    oth_camera_frustum_fov     (Camera* camera);
double    oth_camera_frustum_close   (Camera* camera);
double    oth_camera_frustum_distant (Camera* camera);
float     oth_camera_angle_v (Camera* camera);
float     oth_camera_angle_h (Camera* camera);
float     oth_camera_radius_x (Camera* camera);
float     oth_camera_radius_y (Camera* camera);
float     oth_camera_radius_z (Camera* camera);
float     oth_camera_eye_x (Camera* camera);
float     oth_camera_eye_y (Camera* camera);
float     oth_camera_eye_z (Camera* camera);
float     oth_camera_at_x (Camera* camera);
float     oth_camera_at_y (Camera* camera);
float     oth_camera_at_z (Camera* camera);
float     oth_camera_up_x (Camera* camera);
float     oth_camera_up_y (Camera* camera);
float     oth_camera_up_z (Camera* camera);


#endif
