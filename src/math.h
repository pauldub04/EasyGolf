#ifndef MATH_H
#define MATH_H

#include "raymath.h"
#include "stdbool.h"

float FloatSign(float x);
float Vector2CrossProductLength(Vector2 a, Vector2 b);
bool CheckSegmentsIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d);

// returns number of real roots, roots should be roots[2]
int QuadraticEquation(float a, float b, float c, float* roots);

#endif