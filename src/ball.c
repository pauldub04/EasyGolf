#include "ball.h"

bool BallIsMoving(Ball* ball) {
    return Vector2Length(ball->velocity) > 0.00001f;
}

Ball BallResolveCollisionCircle(Ball* ball, int ballRadius, Circle circle) {
    Ball newBall = *ball;
    if (!CheckCollisionCircles(newBall.position, newBall.radius, ShapeGetPositionCircle(circle), circle.radius)) {
        return newBall;
    }

    Vector2 delta = Vector2Subtract(ball->position, ShapeGetPositionCircle(circle));
    float oldLength = Vector2Length(delta);

    Vector2 mtd = Vector2Scale(delta, (ballRadius + circle.radius - oldLength) / oldLength);

    newBall.position = Vector2Add(newBall.position, mtd);
    newBall.velocity = Vector2Reflect(newBall.velocity, Vector2Normalize(mtd));
    newBall.velocity = Vector2Scale(newBall.velocity, bounce);
    return newBall;
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

Ball BallResolveCollisionHorizontalSegment(Ball *ball, Vector2 oldPosition, Vector2 left, Vector2 right) {
    Ball newBall = *ball;
    if (isIntersect(oldPosition, newBall.position, left, right)) {
        newBall.position.y = left.y;
        newBall.velocity.y *= -bounce;
    }
    return newBall;
}

Ball BallResolveCollisionVerticalSegment(Ball *ball, Vector2 oldPosition, Vector2 top, Vector2 bottom) {
    Ball newBall = *ball;
    if (isIntersect(oldPosition, newBall.position, top, bottom)) {
        newBall.position.x = top.x;
        newBall.velocity.x *= -bounce;
    }
    return newBall;
}

Ball BallResolveCollisionRectangle(Ball* ball, Vector2 oldPosition, Rectangle rectangle) {
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
        newBalls[nextFree++] = BallResolveCollisionCircle(ball, 0, ltCircle);
    }
    if (CheckCollisionCircles(newBall.position, 0, ShapeGetPositionCircle(rtCircle), newBall.radius)) {
        newBalls[nextFree++] = BallResolveCollisionCircle(ball, 0, rtCircle);
    }
    if (CheckCollisionCircles(newBall.position, 0, ShapeGetPositionCircle(lbCircle), newBall.radius)) {
        newBalls[nextFree++] = BallResolveCollisionCircle(ball, 0, lbCircle);
    }
    if (CheckCollisionCircles(newBall.position, 0, ShapeGetPositionCircle(rbCircle), newBall.radius)) {
        newBalls[nextFree++] = BallResolveCollisionCircle(ball, 0, rbCircle);
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

    for (int i = 0; i < shapesLen; ++i) {
        switch (shapes[i]->type) {
        case RECTANGLE:
            *ball = BallResolveCollisionRectangle(ball, oldPosition, shapes[i]->rectangle);
        break;
        case CIRCLE:
            *ball = BallResolveCollisionCircle(ball, ball->radius, shapes[i]->circle);
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
