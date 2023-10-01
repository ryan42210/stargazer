#ifndef CORE_H_
#define CORE_H_

#include "matrix.h"

// #include <Eigen/Core>
// #include <Eigen/Dense>

using Vec2f = Vector<float, 2>;
using Vec2i = Vector<int, 2>;
using Vec3f = Vector<float, 3>;
using Vec3i = Vector<int, 3>;
using Vec4f = Vector<float, 4>;
using Vec4i = Vector<int, 4>;
template<int n> using Vecf = Vector<int, n>;

using Mat3f = Matrix<float, 3, 3>;
template<int n> using MatXf = Matrix<float, n, n>;


constexpr float PI = 3.14159265358979323846264338328;
constexpr float INV_PI = 1 / PI;

constexpr float EPS = 1e-5;

#endif //CORE_H_
