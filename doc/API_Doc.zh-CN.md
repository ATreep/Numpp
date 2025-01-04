[English](./API_Doc.md) | 简体中文


# API 说明

此 API 说明将详细解释 NumPP 的函数和运算符。

## 初始化矩阵

1. 初始化列表

```c++
numpp::Matrix mat{
{1, 3, 5},
{2, 4, 6}
};
```

注意：`numpp::Matrix` 使用二维的 `std::vector<std::vector<double>>`（其别名为 `numpp::Vector2D`）来存储任何矩阵。

2. 从 `Vector2D` 初始化

```c++
numpp::Vector2D vec2d{
{1, 3, 5},
{2, 4, 6}
};

numpp::Matrix mat{vec2d};
```

3. 填充构造函数

```c++
numpp::Matrix mat1{3, 3}; // 3 乘 3 零矩阵或直接使用 `numpp::zeros(m, n);`
numpp::Matrix mat1{3, 3, 5}; // 3 乘 3 矩阵，所有元素都为 5 或直接使用 `numpp::ones(m, n) * 5;`
```

4. m 乘 m 的单位矩阵：`numpp::identity(m);`
5. m 乘 n 的随机矩阵，元素取值从 0 到 5（包括 0 和 5）：`numpp::random(m, n, 0, 5);`

## 矩阵操作

以下所有操作**不是**就地执行的。

1. 将矩阵漂亮地打印到控制台：`numpp::show(mat);`
2. 矩阵与矩阵或者矩阵与双精度小数之间的元素运算

```c++
numpp::Matrix mat1{{1, 2, 3}};
numpp::Matrix mat2{{4, 5, 6}};
std::cout << "逐元素乘法："; numpp::show(mat1 * mat2); std::cout << std::endl;
std::cout << "逐元素加法："; numpp::show(mat1 + mat2); std::cout << std::endl;
std::cout << "逐元素减法："; numpp::show(mat1 - mat2); std::cout << std::endl;
std::cout << "逐元素除法："; numpp::show(mat1 / mat2); std::cout << std::endl;
```

注意：如果 `*, +, -, /` 的右值是双精度值，则可以将双精度值视为相同形状的填充矩阵。

3. 矩阵乘法：`numpp::multiply(mat1, mat2);`

4. 转置矩阵：`mat.T();` 或 `numpp::transpose(mat);`
5. 关于第 m 行和第 n 列计算余子式：`numpp::minor(mat, m, n);`
6. 计算行列式：`numpp::determinant(mat);`
7. 逆矩阵：`numpp::invert(mat);`
8. 伴随矩阵：`numpp::adjugate(mat);`

## 矩阵变换

1. 连接两个矩阵：`numpp::concatenate(mat1, mat2, 0);`

注意：第 3 个参数要么是 0（垂直连接，默认值），要么是 1（水平连接）。

2. 交换两行：`numpp:swap(mat, r1, r2);`
3. 计算上三角形式：`numpp::upper_triangular(mat);`
4. 计算 RREF（简化行梯形形式）：`numpp::rref(mat);`

## 矩阵切片

NumPP 支持使用切片修改元素的值或返回矩阵的一部分。

### 行选择

1. 选择一行 `r1`：`mat[r1]`
2. 选择从 `r1`（包括）到 `r2`（不包括）的多个连续行：`mat[{r1, r2}]`
3. 选择所有行：`mat[numpp::ED]` 或 `mat[ED]`（建议在代码上方设置 `using numpp::ED`）
4. 从 `r1` 选择到最后一行：`mat[{r1, ED}]`
5. 反向索引，选择最后两行：`mat[{-2, ED}]`

### 列选择

1. 选择行 `r1` 和列 `c1` 处的元素（返回 1 x 1 矩阵）：`mat[r1][c1]`
2. 选择行 `r1` 和列 `c1` 处的元素作为双精度值：`mat[r1][c1].num()`
3. 选择一个部分： `mat[{r1, r2}][{c1, c2}]`
4. 选择矩阵的全部（一般无必要）：`mat[ED][ED]`

### 用数字填充某个部分

```c++
numpp::Matrix mat = numpp::zeros(5, 5);
mat[ED][{0, 2}] = 3; // 用 3 填充前两列
```

## 使用迭代器

NumPP 矩阵和矩阵切片支持迭代器操作。

```c++
numpp::Matrix mat = numpp::random(5, 5, 0, 10);
for(double num : mat) std::cout << num << std::endl; // 按顺序打印所有元素（从上到下，从左到右）
for(double num : mat[{0, 2}][{-1, ED}]) std::cout << num << std::endl; // 按顺序打印右上角方阵中的元素
```

提示：也可以用迭代器修改元素。