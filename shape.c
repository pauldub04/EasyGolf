#include "shape.h"

Shape* ShapeCreateRectangle(float x, float y, float width, float height) {
    Shape* shape = (Shape*) malloc(sizeof(Shape));
    shape->rectangle = (Rectangle) {x, y, width, height};
    shape->type = RECTANGLE;
    return shape;
}

Shape* ShapeCreateCircle(float x, float y, float radius) {
    Shape* shape = (Shape*) malloc(sizeof(Shape));
    shape->circle = (Circle) {x, y, radius};
    shape->type = CIRCLE;
    return shape;
}

void ShapeFree(Shape* shape) {
    if (shape == NULL) {
        return;
    }
    free(shape);
}

void ShapeDraw(Shape* shape, Color color, Ball* ball) {
    switch (shape->type) {
    case RECTANGLE:
        ShapeDrawRectangle(shape->rectangle, color, ball);
    break;
    case CIRCLE:
        ShapeDrawCircle(shape->circle, color);
    break;
    default:
        __builtin_unreachable();
    break;
    }
};

void ShapeDrawRectangle(Rectangle rectangle, Color color, Ball* ball) {
    DrawRectangleRec(rectangle, color);
    if (!DEBUG) {
        return;
    }
    Vector2 tl = {rectangle.x, rectangle.y - ball->radius};
    Vector2 tr = Vector2Add(tl, (Vector2) {rectangle.width, 0});
    DrawLineEx(tl, tr, 1, BLACK);

    Vector2 rt = {rectangle.x + rectangle.width + ball->radius, rectangle.y};
    Vector2 rb = Vector2Add(rt, (Vector2) {0, rectangle.height});
    DrawLineEx(rt, rb, 1, BLACK);

    Vector2 bl = {rectangle.x, rectangle.y + rectangle.height + ball->radius};
    Vector2 br = Vector2Add(bl, (Vector2) {rectangle.width, 0});
    DrawLineEx(bl, br, 1, BLACK);

    Vector2 lt = {rectangle.x - ball->radius, rectangle.y};
    Vector2 lb = Vector2Add(lt, (Vector2) {0, rectangle.height});
    DrawLineEx(lt, lb, 1, BLACK);

    
    DrawCircleLinesV((Vector2) {rectangle.x, rectangle.y}, ball->radius, BLACK);  
    DrawCircleLinesV((Vector2) {rectangle.x+rectangle.width, rectangle.y}, ball->radius, BLACK);
    DrawCircleLinesV((Vector2) {rectangle.x, rectangle.y+rectangle.height}, ball->radius, BLACK);
    DrawCircleLinesV((Vector2) {rectangle.x+rectangle.width, rectangle.y+rectangle.height}, ball->radius, BLACK);

    // Rectangle t = {shape->rectangle.x, shape->rectangle.y-ball->radius, shape->rectangle.width, ball->radius};
    // Rectangle r = {shape->rectangle.x+shape->rectangle.width, shape->rectangle.y, ball->radius, shape->rectangle.height};
    // Rectangle b = {shape->rectangle.x, shape->rectangle.y+shape->rectangle.height, shape->rectangle.width, ball->radius};
    // Rectangle l = {shape->rectangle.x-ball->radius, shape->rectangle.y, ball->radius, shape->rectangle.height};
    // DrawRectangleRec(t, YELLOW);
    // DrawRectangleRec(r, YELLOW);
    // DrawRectangleRec(b, YELLOW);
    // DrawRectangleRec(l, YELLOW);
}

void ShapeDrawCircle(Circle circle, Color color) {
    DrawCircleV((Vector2) {circle.x, circle.y}, circle.radius, color);
}

Vector2 ShapeGetPosition(Shape* shape) {
    switch (shape->type) {
    case RECTANGLE:
        return ShapeGetPositionRectangle(shape->rectangle);
    break;
    case CIRCLE:
        return ShapeGetPositionCircle(shape->circle);
    break;
    default:
        __builtin_unreachable();
    break;
    }
}

Vector2 ShapeGetPositionRectangle(Rectangle rectangle) {
    return (Vector2) {rectangle.x, rectangle.y};
}

Vector2 ShapeGetPositionCircle(Circle circle) {
    return (Vector2) {circle.x, circle.y};
}
