#ifndef MATRIX_H_
#define MATRIX_H_

template<typename T, int row, int col>
class Matrix {
  T data[row * col];
 public:
  T& operator[] (size_t idx) {
    return data[idx];
  }

  T& operator() (size_t row_idx, size_t col_idx) {
    return data[row_idx][col_idx];
  }

  // normalized
  // normalize
  // norm
  // transpose
  // dot
};

template <typename T, int n>
using Vector = Matrix<T, n, 1>;

#endif