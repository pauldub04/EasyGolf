#ifndef BALL_H
#define BALL_H

#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "physics.h"
#include "math.h"
#include "constants.h"

typedef struct Ball Ball;
#include "shape.h"

typedef struct Ball {
    int radius;
    Vector2 position;
    Vector2 velocity;
    Color color;
} Ball;


bool BallIsMoving(Ball* ball);

// ballRadius is excplicitly for collision with rect
Ball BallResolveCollisionCircle(Ball* ball, Vector2 oldPosition, int ballRadius, Circle circle, float deltaTime);
Ball BallResolveCollisionHorizontalSegment(Ball *ball, Vector2 oldPosition, Vector2 left, Vector2 right);
Ball BallResolveCollisionVerticalSegment(Ball *ball, Vector2 oldPosition, Vector2 top, Vector2 bottom);
Ball BallResolveCollisionRectangle(Ball* ball, Vector2 oldPosition, Rectangle rectangle, float deltaTime);

void BallMove(Ball* ball, Shape* shapes[], int shapesLen, float deltaTime);
void BallDraw(Ball* ball);

#endif
