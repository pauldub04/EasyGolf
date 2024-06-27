#include "ball.h"
#include "constants.h"
#include "math.h"
#include "physics.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

bool BallIsMoving(Ball* ball) {
    return Vector2Length(ball->velocity) > 0.00001f;
}

Ball BallResolveCollisionCircle(Ball* ball, Vector2 oldPosition, int ballRadius, Circle circle, float deltaTime) {
    Ball newBall = *ball;

    float vx = newBall.velocity.x, vy = newBall.velocity.y;
    float x0 = oldPosition.x, y0 = oldPosition.y;
    float x2 = circle.x, y2 = circle.y;
    float R = circle.radius, r = ballRadius;

    // x(t) = x0 + vx*t
    // y(t) = y0 + vy*t

    // want find t1 : x(t1) = x1, y(t1) = y1
    // (x1-x2)^2 + (y1-y2)^2 = (R+r)^2
    // (x0 + vx*t - x2)^2 + (y0 + vy*t - y2)^2 = (R+r)^2
    // (x0-x2)^2 + 2vx*t(x0-x2) + vx^2*t^2 + (y0-y2)^2 + 2vy*t(y0-y2) + vy^2*t^2 = (R+r)^2
    // t^2*(vx^2 + vy^2) + t*2*(vx*(x0-x2) + vy*(y0-y2)) + (x0-x2)^2 + (y0-y2)^2 - (R+r)^2 = 0
    //     ------a------     --------------b------------   -----------------c-------------

    float a = vx*vx + vy*vy;
    float b = 2*(vx*(x0-x2) + vy*(y0-y2));
    float c = (x0-x2)*(x0-x2) + (y0-y2)*(y0-y2) - (R+r)*(R+r);

    float roots[2];
    int rootsCount = QuadraticEquation(a, b, c, roots);
    
    // no roots -> moving away
    // 1 root -> tangent to a circle, ball goes straight
    // 2 roots and 1st negative -> moving away + let ball go if its inside circle
    if (rootsCount <= 1 || roots[0] < 0) {
        return newBall;
    }

    float t = roots[0];

    // not in this frame
    if (t >= deltaTime) {
        return newBall;
    }

    newBall.position = ApplyVelocity(oldPosition, newBall.velocity, t);

    Vector2 delta = Vector2Subtract(ShapeGetPositionCircle(circle), newBall.position);
    newBall.velocity = Vector2Reflect(newBall.velocity, Vector2Normalize(delta));
    newBall.velocity = Vector2Scale(newBall.velocity, bounce);
    
    // maybe make next frame deltaTime-t, not deltaTime;
    return newBall;
}

Ball BallResolveCollisionHorizontalSegment(Ball *ball, Vector2 oldPosition, Vector2 left, Vector2 right) {
    Ball newBall = *ball;
    if (CheckSegmentsIntersect(oldPosition, newBall.position, left, right)) {
        newBall.position.y = left.y;
        newBall.velocity.y *= -bounce;
    }
    return newBall;
}

Ball BallResolveCollisionVerticalSegment(Ball *ball, Vector2 oldPosition, Vector2 top, Vector2 bottom) {
    Ball newBall = *ball;
    if (CheckSegmentsIntersect(oldPosition, newBall.position, top, bottom)) {
        newBall.position.x = top.x;
        newBall.velocity.x *= -bounce;
    }
    return newBall;
}

Ball BallResolveCollisionRectangle(Ball* ball, Vector2 oldPosition, Rectangle rectangle, float deltaTime) {
    Ball newBall = *ball;
    if (!CheckCollisionCircleRec(ball->position, ball->radius, rectangle)) {
        return newBall;
    }

    Ball newBalls[MAX_SHAPES];
    int nextFree = 0;

    Vector2 tl = {rectangle.x, rectangle.y - newBall.radius};
    Vector2 tr = Vector2Add(tl, (Vector2) {rectangle.width, 0});
    newBalls[nextFree++] = BallResolveCollisionHorizontalSegment(ball, oldPosition, tl, tr);

    Vector2 rt = {rectangle.x + rectangle.width + newBall.radius, rectangle.y};
    Vector2 rb = Vector2Add(rt, (Vector2) {0, rectangle.height});
    newBalls[nextFree++] = BallResolveCollisionVerticalSegment(ball, oldPosition, rt, rb);

    Vector2 bl = {rectangle.x, rectangle.y + rectangle.height + newBall.radius};
    Vector2 br = Vector2Add(bl, (Vector2) {rectangle.width, 0});
    newBalls[nextFree++] = BallResolveCollisionHorizontalSegment(ball, oldPosition, bl, br);

    Vector2 lt = {rectangle.x - newBall.radius, rectangle.y};
    Vector2 lb = Vector2Add(lt, (Vector2) {0, rectangle.height});
    newBalls[nextFree++] = BallResolveCollisionVerticalSegment(ball, oldPosition, lt, lb);

    Circle ltCircle = {rectangle.x, rectangle.y, newBall.radius};
    Circle rtCircle = {rectangle.x+rectangle.width, rectangle.y, newBall.radius};
    Circle lbCircle = {rectangle.x, rectangle.y+rectangle.height, newBall.radius};
    Circle rbCircle = {rectangle.x+rectangle.width, rectangle.y+rectangle.height, newBall.radius};

    if (CheckCollisionCircles(newBall.position, 0, ShapeGetPositionCircle(ltCircle), newBall.radius)) {
        newBalls[nextFree++] = BallResolveCollisionCircle(ball, oldPosition, 0, ltCircle, deltaTime);
    }
    if (CheckCollisionCircles(newBall.position, 0, ShapeGetPositionCircle(rtCircle), newBall.radius)) {
        newBalls[nextFree++] = BallResolveCollisionCircle(ball, oldPosition, 0, rtCircle, deltaTime);
    }
    if (CheckCollisionCircles(newBall.position, 0, ShapeGetPositionCircle(lbCircle), newBall.radius)) {
        newBalls[nextFree++] = BallResolveCollisionCircle(ball, oldPosition, 0, lbCircle, deltaTime);
    }
    if (CheckCollisionCircles(newBall.position, 0, ShapeGetPositionCircle(rbCircle), newBall.radius)) {
        newBalls[nextFree++] = BallResolveCollisionCircle(ball, oldPosition, 0, rbCircle, deltaTime);
    }

    int minIdx = -1;
    int cnt = 0;
    for (int i = 0; i < nextFree; ++i) {
        if (Vector2Equals(ball->position, newBalls[i].position)) {
            continue;
        }
        ++cnt;
        minIdx = i;
    }
    printf("%d\n", cnt);
    if (minIdx == -1) {
        return newBall;
    }

    for (int i = 0; i < nextFree; ++i) {
        if (Vector2Equals(ball->position, newBalls[i].position)) {
            continue;
        }
        float curMin = Vector2Distance(ball->position, newBalls[minIdx].position);
        float cur = Vector2Distance(ball->position, newBalls[i].position);
        if (cur < curMin) {
            printf("%f < %f\n", cur, curMin);
            minIdx = i;
        }
    }
    return newBalls[minIdx];
}


void BallMove(Ball* ball, Shape* shapes[], int shapesLen, float deltaTime) {
    float old_velocity_len = Vector2Length(ball->velocity);
    if (!BallIsMoving(ball)) {
        ball->velocity = Vector2Zero();
        return;
    }

    Vector2 oldPosition = ball->position;
    ball->position = ApplyVelocity(oldPosition, ball->velocity, deltaTime);

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

    for (int i = 0; i < shapesLen; ++i) {
        switch (shapes[i]->type) {
        case RECTANGLE:
            *ball = BallResolveCollisionRectangle(ball, oldPosition, shapes[i]->rectangle, deltaTime);
        break;
        case CIRCLE:
            *ball = BallResolveCollisionCircle(ball, oldPosition, ball->radius, shapes[i]->circle, deltaTime);
        break;
        default:
            __builtin_unreachable();
        break;
        }
    }


    float velocity_len = old_velocity_len - friction * deltaTime;
    if (velocity_len < 0.0f) {
        velocity_len = 0.0f;
    }
    ball->velocity = Vector2Scale(ball->velocity, velocity_len / old_velocity_len);
}

void BallDraw(Ball* ball) {
    // next frame
    if (DEBUG) {
        Vector2 position = ball->position;
        position.x += ball->velocity.x;
        position.y += ball->velocity.y;
        DrawCircleV(position, (float)ball->radius, RED);
    }

    DrawCircleV(ball->position, (float)ball->radius, BROWN);
    DrawCircleV(ball->position, (float)ball->radius-1, ball->color);

    // small dot
    if (DEBUG) {
        DrawCircleV(ball->position, 1, BLACK);
    }
}
