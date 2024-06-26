#pragma once
#include <math.h>
#include "raylib.h"
#include <raymath.h>
#include <stdio.h>
#include "physics.h"

typedef struct Ball {
    int radius;
    Vector2 position;
    Vector2 velocity;
    Color color;
} Ball;


bool IsBallMoving(Ball* ball) {
    return Vector2Length(ball->velocity) > 0.00001f;
}

void BallResolveCollisionCircle(Ball* ball, Vector2 position, float radius) {
    Vector2 delta = Vector2Subtract(ball->position, position);
    float oldLength = Vector2Length(delta);

    Vector2 mtd = Vector2Scale(delta, (ball->radius + radius - oldLength) / oldLength);

    ball->position = Vector2Add(ball->position, mtd);
    ball->velocity = Vector2Reflect(ball->velocity, Vector2Normalize(mtd));
    ball->velocity = Vector2Scale(ball->velocity, bounce);
}

float sign(float x) {
    if (FloatEquals(x, 0.0f)) {
        return 0;
    } else if (x > 0) {
        return 1;
    } else {
        return -1;
    }
}

float Vector2CrossProduct(Vector2 a, Vector2 b) {
    return a.x*b.y - a.y*b.x;
}

bool onSeg(Vector2 p, Vector2 a, Vector2 b) {
    return FloatEquals(Vector2CrossProduct(Vector2Subtract(a, b), Vector2Subtract(p, b)), 0.0f) &&
            Vector2DotProduct(Vector2Subtract(p, a), Vector2Subtract(p, b)) <= 0;
}
 
bool checkSides(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    return sign(Vector2CrossProduct(Vector2Subtract(a, b), Vector2Subtract(d, b))) *
            sign(Vector2CrossProduct(Vector2Subtract(a, b), Vector2Subtract(c, b))) < 0 &&
            sign(Vector2CrossProduct(Vector2Subtract(c, d), Vector2Subtract(a, d))) *
            sign(Vector2CrossProduct(Vector2Subtract(c, d), Vector2Subtract(b, d))) < 0;
}
 
bool isIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    return checkSides(a,b,c,d) || onSeg(a,c,d) || onSeg(b,c,d) ||
            onSeg(c,a,b) || onSeg(d,a,b);
}


void BallResolveCollisionRect(Ball* ball, Rectangle* rect) {
    if (ball->position.x + ball->radius >= rect->x && ball->position.x - ball->radius < rect->x) { // left side
        ball->position.x = rect->x - ball->radius;
        ball->velocity.x *= -bounce;
    } else if (ball->position.x - ball->radius <= rect->x + rect->width && ball->position.x + ball->radius > rect->x + rect->width) { // right side
        ball->position.x = rect->x + rect->width + ball->radius;
        ball->velocity.x *= -bounce;
    } else if (ball->position.y + ball->radius >= rect->y && ball->position.y - ball->radius < rect->y) { // top side
        ball->position.y = rect->y - ball->radius;
        ball->velocity.y *= -bounce;
    } else if (ball->position.y - ball->radius <= rect->y + rect->height && ball->position.y + ball->radius > rect->y + rect->height) { // bottom side
        ball->position.y = rect->y + rect->height + ball->radius;
        ball->velocity.y *= -bounce;
    }
}

void BallMove(Ball* ball, Ball circles[], Rectangle rectangles[], float deltaTime) {
    float old_velocity_len = Vector2Length(ball->velocity);
    if (!IsBallMoving(ball)) {
        ball->velocity = Vector2Zero();
        return;
    }

    Vector2 oldPosition = ball->position;
    ball->position.x += ball->velocity.x * deltaTime;
    ball->position.y += ball->velocity.y * deltaTime;

    if (ball->position.x + ball->radius >= GetScreenWidth()) { // right wall
        ball->position.x = GetScreenWidth() - ball->radius;
        ball->velocity.x *= -bounce;
    } else if (ball->position.x - ball->radius <= 0) { // left wall
        ball->position.x = ball->radius;
        ball->velocity.x *= -bounce;
    }

    if ((ball->position.y + ball->radius >= GetScreenHeight())) { // floor
        ball->position.y = GetScreenHeight() - ball->radius;
        ball->velocity.y *= -bounce;
    } else if (ball->position.y <= ball->radius) { // ceil
        ball->position.y = ball->radius;
        ball->velocity.y *= -bounce;
    }

    if (CheckCollisionCircles(ball->position, ball->radius, circles[0].position, circles[0].radius)) {
        BallResolveCollisionCircle(ball, circles[0].position, circles[0].radius);
    }

    Vector2 lt = {rectangles[0].x, rectangles[0].y};
    Vector2 rt = {rectangles[0].x+rectangles[0].width, rectangles[0].y};
    Vector2 lb = {rectangles[0].x, rectangles[0].y+rectangles[0].height};
    Vector2 rb = {rectangles[0].x+rectangles[0].width, rectangles[0].y+rectangles[0].height};

    if (CheckCollisionCircles(ball->position, 0, lt, ball->radius)) {
        BallResolveCollisionCircle(ball, lt, ball->radius);
    }
    // if (CheckCollisionCircleRec(ball->position, ball->radius, rectangles[0])) {
    //     BallResolveCollisionRect(ball, &rectangles[0]);
    // }

    float velocity_len = old_velocity_len - friction * deltaTime;
    if (velocity_len < 0.0f) {
        velocity_len = 0.0f;
    }
    ball->velocity = Vector2Scale(ball->velocity, velocity_len / old_velocity_len);
}

void BallDraw(Ball* ball) {
    DrawCircleV(ball->position, (float)ball->radius, BROWN);
    DrawCircleV(ball->position, (float)ball->radius-1, ball->color);
}
