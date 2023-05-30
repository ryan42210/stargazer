#include "math_util.h"
#include <cmath>

float radToDeg(float rad) {
  return rad * 180.f * INV_PI;
}

float degToRad(float deg) {
  return deg * PI / 180.f;
}

Vec3f sphereToCartesian(float ra, float dec) {
  float z = - std::cos(degToRad(ra)) * std::cos(degToRad(dec));
  float x = std::sin(degToRad(ra)) * std::cos(degToRad(dec));
  float y = std::sin(degToRad(dec));
  return {x, y, z};
}

bool distEqual(float a, float b) {
  if (abs(a - b) < EPS) return true;
  return false;
}