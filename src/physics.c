#include "physics.h"

Vector2 ApplyVelocity(Vector2 position, Vector2 velocity, float time) {
    return Vector2Add(position, Vector2Scale(velocity, time));
}
