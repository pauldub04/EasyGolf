#ifndef BALL_H
#define BALL_H

#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "physics.h"
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
void BallResolveCollisionCircle(Ball* ball, int ballRadius, Circle circle);
void BallResolveCollisionRect(Ball* ball, Vector2 oldPosition, Rectangle* rect);

float sign(float x);
float Vector2CrossProduct(Vector2 a, Vector2 b);
bool onSeg(Vector2 p, Vector2 a, Vector2 b);
bool checkSides(Vector2 a, Vector2 b, Vector2 c, Vector2 d);
bool isIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d);

void BallMove(Ball* ball, Shape* shapes[], int shapesLen, float deltaTime);
void BallDraw(Ball* ball);

#endif
