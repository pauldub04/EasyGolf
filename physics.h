#pragma once

#define printVector2(vector_name, vec)                                  \
    if (1)                                                              \
        printf(vector_name " = {%f, %f}, len=%f\n", vec.x, vec.y, Vector2Length(vec));

static const float gravity = 0.5f;
static const float bounce = 0.7f;
static const float friction = 0.05f;