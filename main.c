#include "raylib.h"
#include "raymath.h"
#include "ball.h"
#include "shape.h"
#include "constants.h"


int main(void) {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "EasyGolf");

    SetTargetFPS(FPS);
    // 60fps  -> 1
    // 120fps -> 0.5
    // so that fps does not change ball movement

    Ball ball = {
        // .radius = 15,
        .radius = 35,
        .position = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f},
        .velocity = Vector2Zero(),
        .color = WHITE
    };

    Shape* shapes[] = {
        ShapeCreateRectangle(200, 200, 100, 300),
        ShapeCreateCircle(100, 100, 50)
    };
    int shapesLen = 2;


    bool pause = 0;

    Vector2 startMousePosition = Vector2Zero();
    Vector2 mouseMove = Vector2Zero();
    bool mouseWasPressed = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            pause = !pause;
        }

        if (!pause) {
            if (!IsBallMoving(&ball)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    startMousePosition = GetMousePosition();
                    mouseWasPressed = 1;
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mouseWasPressed) {
                    mouseMove = Vector2Subtract(GetMousePosition(), startMousePosition);
                    float mouseMoveLength = Vector2Length(mouseMove);

                    if (!FloatEquals(mouseMoveLength, 0.0f)) {
                        // SCREEN_WIDTH = MAX_LAUNCH_VELOCITY
                        // mouseMoveLength   = newLength

                        // mouseMove: 0 - diag
                        // velocity:  0 - MAX_LAUNCH_VELOCITY
                        // TODO: maybe use Remap()
                        float newLength = MAX_LAUNCH_VELOCITY * mouseMoveLength / SCREEN_WIDTH;
                        mouseMove = Vector2Scale(mouseMove, newLength / mouseMoveLength);
                        // printVector2("mouseMove", mouseMove);
                    }
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    ball.velocity = Vector2Negate(mouseMove);
                    mouseMove = startMousePosition = Vector2Zero();
                    mouseWasPressed = 0;
                }
            }

            float deltaTime = 60.0f/GetFPS();
            BallMove(&ball, shapes, shapesLen, deltaTime);
        }


        BeginDrawing();
            ClearBackground(DARKGREEN);

            if (pause) {
                DrawText("PAUSED", GetScreenWidth()/2-100, GetScreenHeight()/2-50, 30, WHITE);
            }

            for (int i = 0; i < shapesLen; ++i) {
                ShapeDraw(shapes[i], BLUE, &ball);
            }

            if (mouseWasPressed) {
                // Vector2 endPos = Vector2Add(ball.position, Vector2Scale(mouseMove, 5));
                // DrawLineEx(ball.position, endPos, 5, YELLOW);
                // DrawCircleV(endPos, 7, ball.color);

                // arrow = Vector2Negate(arrow);
                // endPos = Vector2Add(ball.position, arrow);
                // DrawLineEx(ball.position, endPos, 5, RED);
                // DrawCircleV(endPos, 7, ball.color);

                Vector2 arrow = Vector2Scale(Vector2Negate(mouseMove), 5);
                float length = Vector2Length(arrow);
                float startDotRadius = 7.0f;

                int dotsCnt = 6;
                float dotsDist = length / dotsCnt;
                while (dotsCnt > 3 && dotsDist < 4*(startDotRadius)) {
                    --dotsCnt;
                    dotsDist = length / dotsCnt;
                }

                Vector2 normArrow = Vector2Normalize(arrow);
                for (int i = 0; i < dotsCnt; ++i) {
                    Vector2 dotPos = Vector2Add(ball.position, Vector2Scale(normArrow, (i+1)*dotsDist));
                    DrawCircleV(dotPos, startDotRadius-i/2.0, (Color){0, 0, 0, 50});
                }
            }
            BallDraw(&ball);

            DrawFPS(10, 10);
        EndDrawing();
        //-----------------------------------------------------
    }

    CloseWindow();
    return 0;
}
