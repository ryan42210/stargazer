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
using Mat3f = Eigen::Matrix3f;
using MatXf = Eigen::MatrixXf;


constexpr float PI = 3.14159265358979323846264338328;
constexpr float INV_PI = 1 / PI;

constexpr float EPS = 1e-5;

#endif //CORE_H_
