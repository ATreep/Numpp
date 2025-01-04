[English](./README.md) | 简体中文


# NumPP：C++ 2D 矩阵库

一个仅包含头文件的 2D 线性代数库，通过将文件夹 `header/NumPP` 复制到 `include` 目录来开始使用 NumPP。

请注意： NumPP **不支持** MSVC，且仅支持 c++11 及以上标准。

## 简介

易于使用、可读性高、像 NumPy 一样切片矩阵以及使用迭代器访问矩阵

## 用法

请阅读 [NumPP API 文档](doc/API_Doc.zh-CN.md) 中的 NumPP API 文档。

## 功能

- 矩阵操作
- 转置、余子式、行列式、逆矩阵
- 矩阵连接
- 行交换
- 计算上三角矩阵和 RREF
- (⭐️) 像 NumPy 一样切片矩阵
- (⭐️) 所有矩阵和矩阵切片都支持 C++ 迭代器

## 简要示例

```c++
#include <NumPP/Matrix2D>
#include <iostream>

using numpp::ED;

int matOper() {
// 初始化矩阵
numpp::Matrix mat{
{3, 7, 4},
{6, 14, 8},
{1, 5, 9}
};

// 初始化向量
numpp::Matrix vec{2, 3, 4};

numpp:Matrix product = numpp::multiply(mat, vec.T()); // 转置 vec 、矩阵乘法

numpp::Matrix aug = numpp::concatenate(mat, vec); // 垂直连接两个矩阵

aug = numpp::ero_swap(aug, 0, 1); // 交换矩阵 aug 中的第 1 行和第 2 行

numpp::Matrix rref = numpp::rref(aug); // 计算矩阵 aug 的 RREF 形式

mat[ED][0] = 0; // 将矩阵 RREF 中的第一列清零，类似于 NumPy 语句：`mat[:, 0] = 0`

// 使用 for-each 按顺序打印所有元素
for (double elem : mat)
std::cout << elem << std::endl;

return 0;
}
```

更多使用示例请参阅 `usage_example.cpp`。

## 预期实现

- 矩阵的布尔运算
- 带索引运算符的条件过滤器

## 后记

版本：0.0.1

原始开发周期：4 天。

欢迎发布问题或通过 treepzhang@outlook.com 与我联系！