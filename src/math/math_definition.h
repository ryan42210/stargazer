#ifndef MATH_DEFINITION_H_
#define MATH_DEFINITION_H_

#include "array.h"


using Vec2f = Vector<float, 2>;
using Vec2i = Vector<int, 2>;
using Vec3f = Vector<float, 3>;
using Vec3i = Vector<int, 3>;
using Vec4f = Vector<float, 4>;
using Vec4i = Vector<int, 4>;
template<int n> using Vecf = Vector<int, n>;

using Mat3f = Matrix<float, 3>;
using MatXXf = DynamicMatrix<float>;


constexpr float PI = 3.14159265358979323846264338328;
constexpr float INV_PI = 1 / PI;

constexpr float EPS = 1e-5;

#endif //MATH_DEFINITION_H_
