#ifndef _OTHELLO_TRANSFORM_H_
#define _OTHELLO_TRANSFORM_H_

/* This controls the flipping animation. */
typedef struct {
        struct {
                float x, y, z;
        } translation;
        struct {
                float a, x, y, z;
        } rotation;
        /* materials */
        float ambient[4];
        float diffuse[4];
        float specular[4];
} Flipper;

/* Array of those to support wave-animation */
Flipper *flippers;

#endif                         /* _OTHELLO_TRANSFORM_H_ */
