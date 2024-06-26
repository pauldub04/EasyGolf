#include "ball.h"
#include "shape.h"

bool IsBallMoving(Ball* ball) {
    return Vector2Length(ball->velocity) > 0.00001f;
}

void BallResolveCollisionCircle(Ball* ball, int ballRadius, Circle circle) {
    Vector2 delta = Vector2Subtract(ball->position, ShapeGetPositionCircle(circle));
    float oldLength = Vector2Length(delta);

    Vector2 mtd = Vector2Scale(delta, (ballRadius + circle.radius - oldLength) / oldLength);

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

void BallResolveCollisionRect(Ball* ball, Vector2 oldPosition, Rectangle* rect) {
    Vector2 tl = {rect->x, rect->y - ball->radius};
    Vector2 tr = Vector2Add(tl, (Vector2) {rect->width, 0});
    if (isIntersect(oldPosition, ball->position, tl, tr)) {
        printf("TOP\n");
        ball->position.y = tl.y;
        ball->velocity.y *= -bounce;
    }

    Vector2 rt = {rect->x + rect->width + ball->radius, rect->y};
    Vector2 rb = Vector2Add(rt, (Vector2) {0, rect->height});
    if (isIntersect(oldPosition, ball->position, rt, rb)) {
        printf("RIGHT\n");
        ball->position.x = rt.x;
        ball->velocity.x *= -bounce;
    }

    Vector2 bl = {rect->x, rect->y + rect->height + ball->radius};
    Vector2 br = Vector2Add(bl, (Vector2) {rect->width, 0});
    if (isIntersect(oldPosition, ball->position, bl, br)) {
        printf("BOTTOM\n");
        ball->position.y = bl.y;
        ball->velocity.y *= -bounce;
    }

    Vector2 lt = {rect->x - ball->radius, rect->y};
    Vector2 lb = Vector2Add(lt, (Vector2) {0, rect->height});
    if (isIntersect(oldPosition, ball->position, lt, lb)) {
        printf("LEFT\n");
        ball->position.x = lt.x;
        ball->velocity.x *= -bounce;
    }

    Circle ltCircle = {rect->x, rect->y, ball->radius};
    Circle rtCircle = {rect->x+rect->width, rect->y, ball->radius};
    Circle lbCircle = {rect->x, rect->y+rect->height, ball->radius};
    Circle rbCircle = {rect->x+rect->width, rect->y+rect->height, ball->radius};

    if (CheckCollisionCircles(ball->position, 0, ShapeGetPositionCircle(ltCircle), ball->radius)) {
        BallResolveCollisionCircle(ball, 0, ltCircle);
    }
    if (CheckCollisionCircles(ball->position, 0, ShapeGetPositionCircle(rtCircle), ball->radius)) {
        BallResolveCollisionCircle(ball, 0, rtCircle);
    }
    if (CheckCollisionCircles(ball->position, 0, ShapeGetPositionCircle(lbCircle), ball->radius)) {
        BallResolveCollisionCircle(ball, 0, lbCircle);
    }
    if (CheckCollisionCircles(ball->position, 0, ShapeGetPositionCircle(rbCircle), ball->radius)) {
        BallResolveCollisionCircle(ball, 0, rbCircle);
    }
}


void BallMove(Ball* ball, Shape* shapes[], int shapesLen, float deltaTime) {
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

    for (int i = 0; i < shapesLen; ++i) {
        switch (shapes[i]->type) {
        case RECTANGLE:
            if (CheckCollisionCircleRec(ball->position, ball->radius, shapes[i]->rectangle)) {
                BallResolveCollisionRect(ball, oldPosition, &shapes[i]->rectangle);
           }
        break;
        case CIRCLE:
            if (CheckCollisionCircles(ball->position, ball->radius, (Vector2) {shapes[i]->circle.x, shapes[i]->circle.y}, shapes[i]->circle.radius)) {
                BallResolveCollisionCircle(ball, ball->radius, shapes[i]->circle);
            }
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
