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

void BallMove(Ball* ball, float deltaTime) {
    float old_velocity_len = Vector2Length(ball->velocity);
    if (!IsBallMoving(ball)) {
        ball->velocity = Vector2Zero();
        return;
    }

    // if (!FloatEquals(deltaTime, 1.0f)) {
    //     printf("deltaTime=%f\n", deltaTime);
    // }

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

void BallResolveCollisionCircle(Ball* ball, Vector2 position, float radius) {
    Vector2 delta = Vector2Subtract(ball->position, position);
    float oldLength = Vector2Length(delta);

    Vector2 mtd = Vector2Scale(delta, (ball->radius + radius - oldLength) / oldLength);

    ball->position = Vector2Add(ball->position, mtd);
    ball->velocity = Vector2Reflect(ball->velocity, Vector2Normalize(mtd));
    ball->velocity = Vector2Scale(ball->velocity, bounce);
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
