English | [简体中文](./API_Doc.zh-CN.md)


# API Instruction

This API instruction is going to interpret functions and operators of NumPP detailedly.



## Initializing a Matrix

1. Initializer list

```c++
numpp::Matrix mat{
  {1, 3, 5},
  {2, 4, 6}
};
```

Note: `numpp::Matrix` uses a 2-D `std::vector<std::vector<double>>` (which has an alias `numpp::Vector2D`) to store any matrix.



2. Initializing from a `Vector2D` 

```c++
numpp::Vector2D vec2d{
  {1, 3, 5},
  {2, 4, 6}
};

numpp::Matrix mat{vec2d};
```



3. Fill constructor

```c++
numpp::Matrix mat1{3, 3};  // A 3 by 3 zero matrix or just use `numpp::zeros(m, n);`
numpp::Matrix mat1{3, 3, 5};  // A 3 by 3 matrix with all fives or just use `numpp::ones(m, n) * 5;`
```



4. Identity matrix with m by m: `numpp::identity(m);`
5. A random matrix with m by n random from 0 to 5 (0 and 5 included): `numpp::random(m, n, 0, 5);`



## Matrix Manipulation

All operations below are **not** in-place.



1. Printing a matrix to console beautifully: `numpp::show(mat);`
2. Elementwise arithemetics between matrices or a matrix and a double

```c++
numpp::Matrix mat1{{1, 2, 3}};
numpp::Matrix mat2{{4, 5, 6}};
std::cout << "Element-wise multiplication: "; numpp::show(mat1 * mat2); std::cout << std::endl;
std::cout << "Element-wise addition: "; numpp::show(mat1 + mat2); std::cout << std::endl;
std::cout << "Element-wise subtraction: "; numpp::show(mat1 - mat2); std::cout << std::endl;
std::cout << "Element-wise division: "; numpp::show(mat1 / mat2); std::cout << std::endl;
```

Note: If the right value of `*, +, -, /` is double, you can see the double value as a fill matrix with the same shape.

3. Matrix multiplication: `numpp::multiply(mat1, mat2);`

4. Tranpose a matrix: `mat.T();` or `numpp::transpose(mat);`
5. Minor with respect to m'th row and n'th column: `numpp::minor(mat, m, n);`
6. Calculate determinant: `numpp::determinant(mat);`
7. Inverse matrix: `numpp::invert(mat);`
8. Adjugate matrix: `numpp::adjugate(mat);`



## Matrices Transformation

1. Concatenate two matrices: `numpp::concatenate(mat1, mat2, 0);`

Note: The 3rd argument is either 0 (concatenating vertically, default value) or 1 (concatenating horizontally). 

2. Swap two rows: `numpp:swap(mat, r1, r2);`
3. Calculate upper triangle form: `numpp::upper_triangular(mat);`
4. Calculate RREF (Reduced Row Echelon Form): `numpp::rref(mat);`



## Matrix Slice

NumPP supports to use a slice to modify elements' values or return a section of the matrix.



### Row Selection

1. Select a row `r1`: `mat[r1]`
2. Select multiple continuous rows from `r1` (included) to `r2` (not included): `mat[{r1, r2}]`
3. Select all rows: `mat[numpp::ED]` or `mat[ED]` (Recommended to set `using numpp::ED` above your codes)
4. Select from `r1` to the end row: `mat[{r1, ED}]`
5. Indexed inversely, select the last two row: `mat[{-2, ED}]`



### Column Selection

1. Select a element at row `r1` and column `c1`  (return a 1 by 1 matrix): `mat[r1][c1]`
2. Select a element at row `r1` and column `c1` as a double: `mat[r1][c1].num()`
3. Select a section: `mat[{r1, r2}][{c1, c2}]` 
4. Select all of the matrix (in fact not necessary): `mat[ED][ED]`



### Fill a Section with a Number

```c++
numpp::Matrix mat = numpp::zeros(5, 5);
mat[ED][{0, 2}] = 3;  // Fill the first two columns with 3
```



## Using Iterator

NumPP matrices and matrix slices support iterator operations.

```c++
numpp::Matrix mat = numpp::random(5, 5, 0, 10);
for(double num : mat) std::cout << num << std::endl;  // Print all elements in order (from top to down, from left to right)
for(double num : mat[{0, 2}][{-1, ED}]) std::cout << num << std::endl;  // Print elements in the top right corner square matrix in order
```

Tips: You can also modify elements with iterator.