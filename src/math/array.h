#ifndef MATRIX_H_
#define MATRIX_H_

#include<cstddef>
#include<cmath>


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

  T& operator[] (size_t idx) {
    return data[idx];
  }
  const T& operator[] (size_t idx) const {
    return data[idx];
  }

  float norm() const {
    float sum = 0.0f;
    for (int i = 0; i < N; i++) {
      sum += data[i] * data[i];
    }
    return std::sqrt(sum);
  }
  
  void normalized() {
    auto norm = this->norm();
    for (int i = 0; i < N; i++) {
      data[i] = static_cast<T>(data[i] / norm);
    }
  }

  Vector<float, N> normalize() const {
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

template<typename T, int Row, int Col>
class Matrix {
 protected:
  T data[Row * Col];
 public:
  Matrix() {
    memset(data, 0, sizeof(T) * Row * Col);
  };

  T& operator[] (size_t idx) {
    return data[idx];
  }
  const T& operator[] (size_t idx) const {
    return data[idx];
  }

  T& operator() (size_t row_idx, size_t col_idx) {
    return data[row_idx * Col + col_idx];
  }
  const T& operator() (size_t row_idx, size_t col_idx) const {
    return data[row_idx * Col + col_idx];
  }

  Vector<T, Row> dot(const Vector<T, Col> &v) const {
    Vector<T, Row> result;
    for (int i = 0; i < Col; i++) {
      for (int j = 0; j < Row; j++) {
        result[j] += data(j, i) * v[i];
      }
    }
    return result;
  }

};

#endif