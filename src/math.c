#include "math.h"

float FloatSign(float x) {
    if (FloatEquals(x, 0.0f)) {
        return 0;
    } else if (x > 0) {
        return 1;
    } else {
        return -1;
    }
}

float Vector2CrossProductLength(Vector2 a, Vector2 b) {
    return a.x*b.y - a.y*b.x;
}

bool PointIsOnSegment(Vector2 point, Vector2 a, Vector2 b) {
    float dotProduct = Vector2DotProduct(Vector2Subtract(point, a), Vector2Subtract(point, b));
    float crossProduct = Vector2CrossProductLength(Vector2Subtract(a, b), Vector2Subtract(point, b));
    return  FloatEquals(crossProduct, 0.0f) && (dotProduct < 0 || FloatEquals(dotProduct, 0));
}
 
bool CheckSegmentsIntersectClearly(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    return  FloatSign(Vector2CrossProductLength(Vector2Subtract(a, b), Vector2Subtract(d, b))) *
            FloatSign(Vector2CrossProductLength(Vector2Subtract(a, b), Vector2Subtract(c, b))) < 0 &&
            FloatSign(Vector2CrossProductLength(Vector2Subtract(c, d), Vector2Subtract(a, d))) *
            FloatSign(Vector2CrossProductLength(Vector2Subtract(c, d), Vector2Subtract(b, d))) < 0;
}
 
bool CheckSegmentsIntersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    return  CheckSegmentsIntersectClearly(a, b, c, d) ||
            PointIsOnSegment(a, c, d) ||
            PointIsOnSegment(b, c, d) ||
            PointIsOnSegment(c, a, b) ||
            PointIsOnSegment(d, a, b);
}

int QuadraticEquation(float a, float b, float c, float* roots) {
    float discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return 0;
    }

    roots[0] = (-b - sqrtf(discriminant)) / (2*a);
    roots[1] = (-b + sqrtf(discriminant)) / (2*a);

    if (FloatEquals(discriminant, 0)) {
        return 1;
    }
    return 2;
}
