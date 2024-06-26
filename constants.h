#ifndef CONSTANTS_H
#define CONSTANTS_H

#define printVector2(vector_name, vec)                                  \
    if (1)                                                              \
        printf(vector_name " = {%f, %f}, len=%f\n", vec.x, vec.y, Vector2Length(vec));

// global
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FPS 60

#define DEBUG 1

// game
#define MAX_LAUNCH_VELOCITY 50

#endif
