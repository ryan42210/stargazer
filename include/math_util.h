#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_

#include "core.h"

float radToDeg(float rad);
float degToRad(float deg);
Vec3f sphereToCartesian(float ra, float dec);
bool distEqual(float a, float b);

#endif //MATH_UTIL_H_
