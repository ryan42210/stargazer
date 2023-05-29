#ifndef CORE_H_
#define CORE_H_

#include <Eigen/Core>
#include <Eigen/Dense>

using Vec2f = Eigen::Vector2f;
using Vec2i = Eigen::Vector2i;
using Vec3f = Eigen::Vector3f;
using Vec3i = Eigen::Vector3i;
using Vec4f = Eigen::Vector4f;
using Vec4i = Eigen::Vector4i;
using Vecf = Eigen::VectorXf;
using MatXf = Eigen::MatrixXf;


constexpr float PI = 3.141592653579;
constexpr float INV_PI = 1 / 3.141592653579;

constexpr float EPS = 1e-4;

#endif //CORE_H_
