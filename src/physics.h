#ifndef PHYSICS_H
#define PHYSICS_H

#include <raymath.h>

static const float gravity = 0.5f;
static const float bounce = 0.7f;
static const float friction = 0.05f;

Vector2 ApplyVelocity(Vector2 position, Vector2 velocity, float time);

#endif
