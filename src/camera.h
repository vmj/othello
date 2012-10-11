#ifndef _OTHELLO_CAMERA_H_
#define _OTHELLO_CAMERA_H_

#include "common.h"
#include "board.h"

/* Structure to control viewing parameters. Angles are relative to center of
 * the board and controlled via arrow keys.
 */
typedef struct {
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
} Camera;

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

Camera*   oth_camera_init    (Board* board,
                              int *argc,
                              char **argv);
void      oth_camera_free    (Camera* camera);
void      oth_camera_reset   (Camera* camera,
                              Board* board);

void      oth_camera_update  (Camera* camera);

#endif
