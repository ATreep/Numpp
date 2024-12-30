# NumPP: C++ 2D Matrix Library

A header-only 2D linear algebra library, start the journey with NumPP by copying folder `header/NumPy` to your include directory.



## Introduction

Easy to Use, High-Readability, Slicing Matrices Like NumPy, and Accessing Matrices with Iterator



## Features

- Matrices Manipulation
- Transpose, Minor, Determinant, Inverse
- Matrix Concatenation
- Row Swap
- Calculating Upper Triangle and RREF
- (⭐️) Slicing Matrices Like NumPy
- (⭐️) All Matrices and Matrix Slices Support C++ Iterator



## Brief Example

```c++
#include <NumPP/Matrix2D>
#include <iostream>

using numpp::ED;

int matOper() {
  // Initialize a matrix
  numpp::Matrix mat{
    {3, 7, 4},
    {6, 14, 8},
    {1, 5, 9}
  };
  
  // Initialize a vector
  numpp::Matrix vec{2, 3, 4};
  
  numpp:Matrix product = numpp::multiply(mat, vec.T());  // Transpose vec & Matrix multiplication
  
  numpp::Matrix aug = numpp::concatenate(mat, vec);  // Concatenate two matrix vertically
  
  aug = numpp::ero_swap(aug, 0, 1);  // Swap row 1 and row 2 in matrix aug
  
  numpp::Matrix rref = numpp::rref(aug);  // Calculate RREF form for matrix aug
  
  mat[ED][0] = 0;  // Zero out the first column in matrix rref, similar to NumPy statement: `rref[:, 0] = 0`
  
  // Print all element in order with for-each
  for (double elem : mat)
    std::cout << elem << std::endl;
  
  return 0;
}
```



## Expected to Implement

- Boolean operations for matrices
- Conditional filter with index operators