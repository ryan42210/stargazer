#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_

#include "core.h"

#include <cmath>

float radToDeg(float rad) {
  return rad * 180.f * INV_PI;
}

float degToRad(float deg) {
  return deg * PI / 180.f;
}

Vec3f sphereToCartesian(float ra, float dec) {
  float x = std::cos(degToRad(ra)) * std::cos(degToRad(dec));
  float y = std::sin(degToRad(ra)) * std::cos(degToRad(dec));
  float z = std::sin(degToRad(dec));
  return {x, y, z};
}

#endif //MATH_UTIL_H_
