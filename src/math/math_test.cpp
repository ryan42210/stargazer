#include "array.h"
#include <iostream>

int main() {
  Vector<float, 3> x{1.0f, 2.3f, 3.1f};
  std::cout << "Vector x:\n[" << x[0] << ", " << x[1] << ", " << x[2] << "]" << std::endl;
  auto A = Matrix<float, 3>::identity();
  std::cout << "Matrix A:\n[[" << A(0, 0) << ", " << A(0, 1) << ", " << A(0, 2) << "],\n" 
                       << " [" << A(1, 0) << ", " << A(1, 1) << ", " << A(1, 2) << "],\n"
                       << " [" << A(2, 0) << ", " << A(2, 1) << ", " << A(2, 2) << "]]" << std::endl;

  auto b = A.dot(x);
  std::cout << "Vector b:\n[" << b[0] << ", " << b[1] << ", " << b[2] << "]" << std::endl;
  
  x = x * 2.0f;
  std::cout << "Vector x:\n[" << x[0] << ", " << x[1] << ", " << x[2] << "]" << std::endl;

  b *= 0.5f;
  std::cout << "Vector b:\n[" << b[0] << ", " << b[1] << ", " << b[2] << "]" << std::endl;

  std::cout << "Norm of vector b is: " << b.norm() << std::endl;
  b.normalize();
  std::cout << "Norm of vector b after normalization is: " << b.norm() << std::endl;
  std::cout << "Vector b:\n[" << b[0] << ", " << b[1] << ", " << b[2] << "]" << std::endl;

  auto c = b + x;
  std::cout << "Vector c:\n[" << c[0] << ", " << c[1] << ", " << c[2] << "]" << std::endl;

  c -= 1.2f;
  std::cout << "Vector c:\n[" << c[0] << ", " << c[1] << ", " << c[2] << "]" << std::endl;

  auto d = -c;
  std::cout << "Vector -c:\n[" << d[0] << ", " << d[1] << ", " << d[2] << "]" << std::endl;

  return 0;
}