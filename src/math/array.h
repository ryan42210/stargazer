#ifndef MATRIX_H_
#define MATRIX_H_

#include <cstddef>
#include <cmath>
#include <memory>
#include <vector>
#include <iostream>
#include <initializer_list>

template<typename T, int N>
class Vector {
  T data[N];
 public:
  Vector() {
    memset(data, 0, sizeof(T) * N);
  }

  Vector(T init_val) {
    for (int i = 0; i < N; i++) {
      data[i] = init_val;
    }
  }

  Vector(std::initializer_list<T> values) {
    if (values.size() != N) {
        throw std::invalid_argument("Incorrect number of arguments");
    }

    std::copy(values.begin(), values.end(), data);
  }

  T& operator[] (size_t idx) {
    if (idx >= N) {
      throw std::invalid_argument("Vector index out of range.");
    }
    return data[idx];
  }
  const T& operator[] (size_t idx) const {
    if (idx >= N) {
      throw std::invalid_argument("Vector index out of range.");
    }
    return data[idx];
  }

  Vector<T, N> &operator*=(const T &s) {
    for (int i = 0; i < N; i++) {
      data[i] *= s;
    }
    return *this;
  }

  Vector<T, N> operator*(const T &rhs) {
    Vector<T, N> res = *this;
    res *= rhs;
    return res;
  }

  Vector<T, N> &operator+=(const Vector<T, N> &v) {
    for (int i = 0; i < N; i++) {
      data[i] += v[i];
    }
    return *this;
  }

  friend Vector<T, N> operator+(Vector<T, N> lhs, const Vector<T, N> &rhs) {
    lhs += rhs;
    return lhs;
  }

  Vector<T, N> &operator-=(const Vector<T, N> &v) {
    for (int i = 0; i < N; i++) {
      data[i] -= v[i];
    }
    return *this;
  }

  friend Vector<T, N> operator-(Vector<T, N> lhs, const Vector<T, N> &rhs) {
    lhs -= rhs;
    return lhs;
  }

  Vector<T, N> operator-() {
    Vector<T, N> res;
    res -= *this;
    return res;
  }

  float norm() const {
    float sum = 0.0f;
    for (int i = 0; i < N; i++) {
      sum += data[i] * data[i];
    }
    return std::sqrt(sum);
  }
  
  void normalize() {
    auto norm = this->norm();
    for (int i = 0; i < N; i++) {
      data[i] = static_cast<T>(data[i] / norm);
    }
  }

  Vector<float, N> normalized() const {
    Vector<float, N> result;
    auto norm = this->norm();
    for (int i = 0; i < N; i++) {
      result[i] = static_cast<float>(data[i] / norm);
    }
    return result;
  }

  float dot(const Vector<T, N> &v) const {
    float result = 0.0f;
    for (int i = 0; i < N; i++) {
      result += this->data[i] * v.data[i];
    }
    return result;
  }

};

template<typename T, int N>
class Matrix {
 protected:
  T data[N * N];
 public:
  Matrix() {
    memset(data, 0, sizeof(T) * N * N);
  };

  T& operator[] (size_t idx) {
    return data[idx];
  }
  const T& operator[] (size_t idx) const {
    return data[idx];
  }

  T& operator() (size_t row_idx, size_t col_idx) {
    if (row_idx >= N || col_idx >= N) {
      throw std::invalid_argument("Vector index out of range.");
    }
    return data[row_idx * N + col_idx];
  }
  const T& operator() (size_t row_idx, size_t col_idx) const {
    if (row_idx >= N || col_idx >= N) {
      throw std::invalid_argument("Vector index out of range.");
    }
    return data[row_idx * N + col_idx];
  }

  Vector<T, N> dot(const Vector<T, N> &v) const {
    Vector<T, N> result;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        result[j] += this->operator()(j, i) * v[i];
      }
    }
    return result;
  }

  Matrix<T, N> transpose() {
    Matrix<T, N> result;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        result(j, i) = this->operator()(i, j);
      }
    }
    return result;
  }

  static Matrix<T, N> identity() {
    Matrix<T, N> result;
    for (int i = 0; i < N; i++) {
      result(i, i) = static_cast<T>(1);
    }
    return result;
  }

};

template<typename T>
class DynamicMatrix {
  std::vector<T> data;
  size_t row, col;
 public:
  DynamicMatrix(const size_t row, const size_t col) {
    data.assign(row * col, 0);
  }

  T& operator() (size_t row_idx, size_t col_idx) {
    if (row_idx >= row || col_idx >= col) {
      throw std::invalid_argument("Vector index out of range.");
    }
    return data[row_idx * col + col_idx];
  }
  const T& operator() (size_t row_idx, size_t col_idx) const {
    if (row_idx >= row || col_idx >= col) {
      throw std::invalid_argument("Vector index out of range.");
    }
    return data[row_idx * col + col_idx];
  }

  DynamicMatrix<T> transpose() {
    DynamicMatrix<T> result(col, row);
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        result(j, i) = this->operator()(i, j);
      }
    }
    return result;
  }
};

#endif