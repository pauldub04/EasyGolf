#ifndef SHAPE_H
#define SHAPE_H

#include <stdlib.h>
#include "raylib.h"
#include "constants.h"

typedef struct Shape Shape;
typedef struct Circle Circle;

#include "ball.h"

typedef struct Circle {
    float x;
    float y;
    float radius;
} Circle;

typedef enum ShapeType {
    RECTANGLE,
    CIRCLE
} ShapeType;

typedef struct Shape {
    ShapeType type;
    Rectangle rectangle;
    Circle circle;
} Shape;

Shape* ShapeCreateRectangle(float x, float y, float width, float height);
Shape* ShapeCreateCircle(float x, float y, float radius);
void ShapeFree(Shape* shape);

void ShapeDraw(Shape* shape, Color color, Ball* ball);
void ShapeDrawRectangle(Rectangle rectangle, Color color, Ball* ball);
void ShapeDrawCircle(Circle circle, Color color);

Vector2 ShapeGetPosition(Shape* shape);
Vector2 ShapeGetPositionRectangle(Rectangle rectangle);
Vector2 ShapeGetPositionCircle(Circle circle);

#endif