#include "NumPPDeclaration.h"
#include <iostream>
#include <random>
#include <utility>
#include <algorithm>

namespace numpp {
    using std::cout;
    using std::endl;

    bool show_numpp_exception_details = false;

    IllegalArithmeticsException::IllegalArithmeticsException(std::string msg) : message(std::move(msg)) {
        if (show_numpp_exception_details)
            cout << "NumPP Exception: " << message << endl;
    }

    const char* IllegalArithmeticsException::what() const noexcept {
        return message.c_str();
    }

    Matrix::Matrix(Vector2D vector2d) : _matrix(std::move(vector2d)){}

    Matrix::Matrix(std::initializer_list<std::vector<double>> initList) {
        Vector2D res;
        for (const std::vector<double>& rowVec : initList) {
            res.emplace_back(rowVec);
        }
        _matrix = res;
    }

    Matrix::Matrix(MatrixSection matrixSection) : _matrix(matrixSection._matrix) {}

    Matrix::Matrix(size_t m, size_t n, double number = 0) : _matrix(Vector2D(m, std::vector<double>(n,number))) {}

    Matrix::Matrix(const Matrix &other) = default;

    Matrix::Matrix(Matrix&& other) noexcept : _matrix(std::move(other._matrix)) {}

    Matrix Matrix::operator*(double other) const {
        Matrix product_mat = *this;
        std::transform(product_mat.begin(), product_mat.end(),  product_mat.begin(),
                       [other](double elem)->double{return elem * other;});
        return product_mat;
    }

    Matrix Matrix::operator*(const Matrix &other) const {
        if (!(this->shape()[0] == other.shape()[0] && this->shape()[1] == other.shape()[1])) {
            throw IllegalArithmeticsException{"To apply element-wise operation between two matrices, their shapes must be the same."};
        }

        Matrix _mat = *this;
        Matrix product_mat = other;
        std::transform(_mat.begin(), _mat.end(), product_mat.begin(), product_mat.begin(),
                       [](double elem1, double elem2)->double{return elem1 * elem2;});
        return product_mat;
    }

    Matrix Matrix::operator+(double other) const {
        Matrix product_mat = *this;
        std::transform(product_mat.begin(), product_mat.end(),  product_mat.begin(),
                       [other](double elem)->double{return elem + other;});
        return product_mat;
    }

    Matrix Matrix::operator+(const Matrix& other) const {
        if (!(this->shape()[0] == other.shape()[0] && this->shape()[1] == other.shape()[1])) {
            throw IllegalArithmeticsException{"To apply element-wise operation between two matrices, their shapes must be the same."};
        }

        Matrix _mat = *this;
        Matrix product_mat = other;
        std::transform(_mat.begin(), _mat.end(), product_mat.begin(), product_mat.begin(),
                       [](double elem1, double elem2)->double{return elem1 + elem2;});
        return product_mat;
    }

    Matrix Matrix::operator+() const {
        return *this;
    }

    Matrix Matrix::operator/(const Matrix& other) const {
        if (!(this->shape()[0] == other.shape()[0] && this->shape()[1] == other.shape()[1])) {
            throw IllegalArithmeticsException{"To apply element-wise operation between two matrices, their shapes must be the same."};
        }

        Matrix _mat = *this;
        Matrix product_mat = other;
        std::transform(_mat.begin(), _mat.end(), product_mat.begin(), product_mat.begin(),
                       [](double elem1, double elem2)->double{return elem1 / elem2;});
        return product_mat;
    }

    Matrix Matrix::operator/(double other) const {
        Matrix product_mat = *this;
        std::transform(product_mat.begin(), product_mat.end(),  product_mat.begin(),
                       [other](double elem)->double{return elem / other;});
        return product_mat;
    }

    Matrix Matrix::operator-(const Matrix& other) const {
        return *this + (-other);
    }

    Matrix Matrix::operator-(double other) const {
        return *this + (-other);
    }

    Matrix Matrix::operator-() const {
        return *this * (-1);
    }

    Matrix Matrix::operator*=(double other) {
        Matrix res = *this * other;
        std::copy(res.begin(), res.end(), this->begin());
        return res;
    }

    Matrix Matrix::operator*=(const Matrix& other) {
        Matrix res = *this * other;
        std::copy(res.begin(), res.end(), this->begin());
        return res;
    }

    Matrix Matrix::operator+=(double other) {
        Matrix res = *this + other;
        std::copy(res.begin(), res.end(), this->begin());
        return res;
    }

    Matrix Matrix::operator+=(const Matrix& other) {
        Matrix res = *this + other;
        std::copy(res.begin(), res.end(), this->begin());
        return res;
    }

    Matrix Matrix::operator/=(double other) {
        Matrix res = *this / other;
        std::copy(res.begin(), res.end(), this->begin());
        return res;
    }

    Matrix Matrix::operator/=(const Matrix& other) {
        Matrix res = *this / other;
        std::copy(res.begin(), res.end(), this->begin());
        return res;
    }

    Matrix Matrix::operator-=(double other) {
        Matrix res = *this - other;
        std::copy(res.begin(), res.end(), this->begin());
        return res;
    }

    Matrix Matrix::operator-=(const Matrix& other) {
        Matrix res = *this - other;
        std::copy(res.begin(), res.end(), this->begin());
        return res;
    }

    MatrixSection Matrix::operator[](SignedSlice slice_numpp) {
        Vector2D vec2d;
        Slice slice {
            static_cast<size_t>(std::abs(slice_numpp.start_idx)),
            static_cast<size_t>(std::abs(slice_numpp.end_idx))
        };

        Section section;

        if (slice_numpp.start_idx < 0) {
            slice.start_idx = shape()[0] + slice_numpp.start_idx;
        }
        if (slice_numpp.end_idx < 0) {
            slice.end_idx = shape()[0] + slice_numpp.start_idx;
        }
        else if (slice_numpp.end_idx == ED) {
            slice.end_idx = shape()[0];
        }

        for (size_t r = slice.start_idx; r < slice.end_idx; r++) {
            vec2d.push_back(this->_matrix[r]);
        }

        section.state = 1;
        section.row_slice = slice;
        section.col_slice = {0, shape()[1]};

        MatrixSection res{vec2d, this, section};
        return res;
    }

    MatrixSection Matrix::operator[](int index_numpp) {
        SignedSlice signedSlice;
        if (index_numpp == ED) {
            signedSlice = {0, static_cast<int>(shape()[0])};
        }
        else if (index_numpp >= 0) {
            signedSlice = {index_numpp, index_numpp + 1};
        }
        else {
            signedSlice = {index_numpp, static_cast<int>(shape()[0] + index_numpp + 1)};
        }
        return (*this)[signedSlice];
    }

    Matrix& Matrix::operator=(const Matrix& other) = default;

    Matrix::Iterator::Iterator(Vector2D& vector2d, size_t row, size_t col) :
            _vector2d(vector2d), cur_row(row), cur_col(col) {}

    Matrix::Iterator::Iterator(const Iterator& iterator) = default;

    Matrix::Iterator::IteratorBeyondRangeException::IteratorBeyondRangeException(std::string msg) : message(
            std::move(msg)) {
        if (show_numpp_exception_details)
            cout << "NumPP Exception: " << message << endl;
    }

    const char *Matrix::Iterator::IteratorBeyondRangeException::what() const noexcept {
        return message.c_str();
    }

    Matrix::Iterator::reference Matrix::Iterator::operator*() {
        return _vector2d[cur_row][cur_col];
    }

    Matrix::Iterator& Matrix::Iterator::operator++() {
        size_t max_row = _vector2d.size();
        size_t max_col = _vector2d[0].size();

        if (cur_row == max_row) {
            throw IteratorBeyondRangeException{"You have gotten the end of the iterator."};
        }

        if (cur_col == max_col - 1) {
            // cur is at the last element of this row
            cur_row++;
            cur_col = 0;
        }
        else {
            cur_col++;
        }

        return *this;
    }

    Matrix::Iterator Matrix::Iterator::operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool Matrix::Iterator::operator==(const Iterator &other) const {
        return &(other._vector2d) == &(this->_vector2d) && other.cur_row == this->cur_row
               && other.cur_col == this->cur_col;
    }

    bool Matrix::Iterator::operator!=(const Iterator &other) const {
        return !(*this == other);
    }

    Matrix::Iterator &Matrix::Iterator::operator--() {
        size_t max_row = _vector2d.size();
        size_t max_col = _vector2d[0].size();

        if (cur_row  == max_row) {
            // cur is at the row beyond the last row (the end of iterator)
            cur_row--;
            cur_col = max_col - 1;
        }
        else
        {
            if (cur_col == 0) {
                // cur is at the first element of this row
                if (cur_row == 0) {
                    // cur is at the first row
                    throw IteratorBeyondRangeException{"You are at the beginning of the iterator."};
                } else {
                    cur_row--;
                    cur_col = max_col - 1;
                }
            } else {
                cur_col--;
            }
        }

        return *this;
    }

    Matrix::Iterator Matrix::Iterator::operator--(int) {
        Iterator temp = *this;
        --(*this);
        return temp;
    }

    Matrix::Iterator::pointer Matrix::Iterator::operator->() {
        return &_vector2d[cur_row][cur_col];
    }

    Matrix::Iterator Matrix::begin() {
        return Iterator{_matrix, 0, 0};
    }

    Matrix::Iterator Matrix::end() {
        return Iterator{_matrix, _matrix.size(), 0};
    }

    std::vector<size_t> Matrix::shape() const {
        return std::vector<size_t>{this->_matrix.size(), this->_matrix[0].size()};
    }

    double Matrix::at(size_t x, size_t y) const {
        return this->_matrix[x][y];
    }

    Matrix Matrix::row(int row_index) const {
        return Matrix{*this}[row_index];
    }

    Matrix Matrix::column(int col_index) const {
        return Matrix{*this}[{0, ED}][col_index];
    }

    std::vector<Matrix> Matrix::rows() const {
        std::vector<Matrix> res;
        Matrix matrix = *this;
        for (int r = 0; r < shape()[0]; r++) {
            res.push_back(matrix.row(r));
        }
        return res;
    }

    std::vector<Matrix> Matrix::columns() const {
        std::vector<Matrix> res;
        Matrix matrix = *this;
        for (int c = 0; c < shape()[1]; c++) {
            res.push_back(matrix.column(c));
        }
        return res;
    }

    Matrix Matrix::T() const {
        Vector2D transposed_vec2d(shape()[1], std::vector<double> (shape()[0], 0));
        for (int row = 0; row < shape()[0]; row++) {
            Matrix cur_row = this->row(row);
            int elem_index = 0;
            for (double element : cur_row) {
                transposed_vec2d[elem_index][row] = element;
                elem_index++;
            }
        }
        return Matrix{transposed_vec2d};
    }

    Vector2D Matrix::toVector2D() const {
        return _matrix;
    }

    Vector2D* Matrix::dataHolder() {
        return &_matrix;
    }

    double Matrix::num() const {
        if (shape()[0] == 1 && shape()[1] == 1) {
            return at(0, 0);
        }
        else {
            throw IllegalArithmeticsException{"Only 1 by 1 matrices can call num() method."};
        }
    }

    Matrix::~Matrix() = default;

    MatrixSection::MatrixSection(Vector2D vector2D) : Matrix(std::move(vector2D)), _parentMatrix(nullptr), _indexesOfParentMatrix({0}) {}

    MatrixSection::MatrixSection(Vector2D vector2D, Matrix *parentMatrix, Section indexesOfParentMatrix) : Matrix(std::move(vector2D)), _parentMatrix(parentMatrix), _indexesOfParentMatrix(indexesOfParentMatrix) {}

    MatrixSection MatrixSection::operator[](SignedSlice slice_numpp) {
        if (_indexesOfParentMatrix.state == 2) {
            throw IllegalArithmeticsException{"Cannot slice a matrix too many times, two times at most."};
        }

        Vector2D vec2d;
        Slice slice {
            static_cast<size_t>(std::abs(slice_numpp.start_idx)),
            static_cast<size_t>(std::abs(slice_numpp.end_idx))
        };

        Section section;
        if (slice_numpp.start_idx < 0) {
            slice.start_idx = shape()[1] + slice_numpp.start_idx;
        }

        if (slice_numpp.end_idx < 0) {
            slice.end_idx = shape()[1] + slice_numpp.start_idx;
        }
        else if (slice_numpp.end_idx == ED) {
            slice.end_idx = shape()[1];
        }

        for(size_t r = 0; r < shape()[0]; r++) {
            std::vector<double> vec;
            for (size_t c = slice.start_idx; c < slice.end_idx; c++) {
                vec.push_back(this->at(r, c));
            }

            vec2d.emplace_back(vec);
        }

        section.state = 2;
        section.row_slice = this->_indexesOfParentMatrix.row_slice;
        section.col_slice = slice;

        MatrixSection res{vec2d, this->_parentMatrix, section};
        return res;
    }

    MatrixSection MatrixSection::operator[](int index_numpp) {
        SignedSlice signedSlice;
        if (index_numpp == ED) {
            signedSlice = {0, static_cast<int>(shape()[1])};
        }
        else if (index_numpp >= 0) {
            signedSlice = {index_numpp, index_numpp + 1};
        }
        else {
            signedSlice = {index_numpp, static_cast<int>(shape()[1] + index_numpp + 1)};
        }
        return (*this)[signedSlice];
    }

    Matrix& MatrixSection::operator=(double other) {
        std::vector<double> fill_vector(shape()[0] * shape()[1], other);
        std::copy(fill_vector.begin(), fill_vector.end(), begin());
        Vector2D new_vector2d(shape()[0], std::vector<double>(shape()[1], other));
        this->_matrix = new_vector2d;
        return *this;
    }

    Matrix& MatrixSection::operator=(const Matrix& other) {
        Matrix other_copy{other};
        std::copy(other_copy.begin(), other_copy.end(), begin());
        this->_matrix = other_copy.toVector2D();
        return *this;
    }

    MatrixSection::Iterator::Iterator(Vector2D& vector2d, size_t row, size_t col, Section sect = {0}) :
            Matrix::Iterator(vector2d, row, col), _section(sect) {}

    MatrixSection::Iterator::Iterator(const Iterator& iterator) = default;

    MatrixSection::Iterator& MatrixSection::Iterator::operator++() {
        // If section is activated, move the section into the left top position, aligning (0, 0) element.
        size_t offset_row = 0;  // Vertical offset
        size_t offset_col = 0;  // Horizontal offset

        size_t max_row = _vector2d.size();
        size_t max_col = _vector2d[0].size();

        if (_section.state > 0) {
            offset_row = _section.row_slice.start_idx;
            offset_col = _section.col_slice.start_idx;

            max_row = _section.row_slice.end_idx - _section.row_slice.start_idx;
            max_col = _section.col_slice.end_idx - _section.col_slice.start_idx;
        }

        if (cur_row - offset_row == max_row) {
            throw IteratorBeyondRangeException{"You have gotten the end of the iterator."};
        }

        if (cur_col - offset_col == max_col - 1) {
            // cur is at the last element of this row
            if (cur_row - offset_row == max_row - 1) {
                // cur is at the last row
                if (cur_col < _vector2d[0].size() - 1) {
                    // Have gotten in the right board of section but there are gaps to get in the right board of origin matrix
                    cur_col++;

                }
                else {
                    cur_row++;
                    cur_col = 0;
                }
            }
            else {
                cur_row++;
                cur_col = offset_col;
            }
        }
        else {
            cur_col++;
        }

        return *this;
    }

    MatrixSection::Iterator MatrixSection::Iterator::operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    MatrixSection::Iterator &MatrixSection::Iterator::operator--() {
        size_t offset_row = 0;  // Vertical offset
        size_t offset_col = 0;  // Horizontal offset

        size_t max_row = _vector2d.size();
        size_t max_col = _vector2d[0].size();

        if (_section.state > 0) {
            offset_row = _section.row_slice.start_idx;
            offset_col = _section.col_slice.start_idx;

            max_row = _section.row_slice.end_idx - _section.row_slice.start_idx;
            max_col = _section.col_slice.end_idx - _section.col_slice.start_idx;
        }

        if (cur_row - offset_row == max_row) {
            // cur is at the row beyond the last row (the end of iterator)
            cur_row--;
            cur_col = max_col + offset_col - 1;
        }
        else
        {
            if (cur_col - offset_col == 0) {
                // cur is at the first element of this row
                if (cur_row - offset_row == 0) {
                    // cur is at the first row
                    throw IteratorBeyondRangeException{"You are at the beginning of the iterator."};
                } else {
                    cur_row--;
                    cur_col = max_col + offset_col - 1;
                }
            } else {
                cur_col--;
            }
        }

        return *this;
    }

    MatrixSection::Iterator MatrixSection::Iterator::operator--(int) {
        Iterator temp = *this;
        --(*this);
        return temp;
    }

    MatrixSection::Iterator MatrixSection::begin() {
        return Iterator{*_parentMatrix->dataHolder(),
                            _indexesOfParentMatrix.row_slice.start_idx,
                            _indexesOfParentMatrix.col_slice.start_idx,
                            _indexesOfParentMatrix};
    }

    MatrixSection::Iterator MatrixSection::end() {
        return ++Iterator{*_parentMatrix->dataHolder(), _indexesOfParentMatrix.row_slice.end_idx - 1,
                            _indexesOfParentMatrix.col_slice.end_idx - 1, _indexesOfParentMatrix};
    }

    MatrixSection::~MatrixSection() = default;


    Matrix zeros(size_t m, size_t n) {
        return {m, n};
    }

    Matrix ones(size_t m, size_t n) {
        return {m, n, 1};
    }

    Matrix identity(size_t m) {
        Matrix res{m, m};
        for (int r = 0; r < m; r++) {
            res[r][r] = 1;
        }
        return res;
    }

    Matrix random(size_t m, size_t n, int min, int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        Vector2D _vector2d(m, std::vector<double>(n));
        for (int row = 0; row < m; row++) {
            for (int col = 0; col < n; col++) {
                _vector2d[row][col] = dis(gen);
            }
        }
        return Matrix{_vector2d};
    }

    void show(const Matrix &matrix) {
        Matrix _matrix = matrix;
        cout << "Matrix([" << endl;
        int cur_index = 0;
        const size_t col_size = _matrix.shape()[1];
        for(double element : _matrix) {
            if (cur_index == 0)
                cout << "\t[";

            cout << std::defaultfloat << element;

            if (cur_index == col_size - 1) {
                cout << "]" << endl;
            }
            else {
                cout << ", ";
            }

            cur_index++;
            if (cur_index == col_size)
                cur_index = 0;
        }
        cout << "])" << endl;
    }

    Matrix multiply(const Matrix& matrix, double c) {
        return matrix * c;
    }

    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
        // Checking shapes of two matrices.
        if (matrix1.shape()[1] == matrix2.shape()[0]) {
            Vector2D product_vec2d(matrix1.shape()[0], std::vector<double>(matrix2.shape()[1], 0));

            for (int row_of_mat1 = 0; row_of_mat1 < matrix1.shape()[0]; row_of_mat1++) {
                Matrix mul_row = matrix1.row(row_of_mat1);
                for (int col_of_mat2 = 0; col_of_mat2 < matrix2.shape()[1]; col_of_mat2++) {
                    Matrix mul_col = matrix2.column(col_of_mat2);
                    Matrix element_wise_product = mul_row * mul_col.T();
                    double sum = std::accumulate(element_wise_product.begin(), element_wise_product.end(),
                                                 0.0);
                    product_vec2d[row_of_mat1][col_of_mat2] = sum;
                }
            }

            return Matrix{product_vec2d};
        }
        else {
            throw IllegalArithmeticsException{
                    "The column size of the first matrix must be the same as the row size of the second matrix on "
                    "the matrix multiplication operation."
            };
        }
    }

    Matrix sum(const Matrix& matrix, double c) {
        return matrix + c;
    }

    Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
        return matrix1 + matrix2;
    }

    Matrix transpose(const Matrix& matrix) {
        return matrix.T();
    }

    Matrix minor(const Matrix& matrix, size_t m, size_t n) {
        // Checking shape of the given matrix (must be a square matrix)
        if (matrix.shape()[0] != matrix.shape()[1]) {
            throw IllegalArithmeticsException{"Cannot calculate minor for a non-square matrix."};
        }

        if (!(m < matrix.shape()[0] && n < matrix.shape()[1])) {
            throw IllegalArithmeticsException("Illegal index of row or column.");
        }

        Vector2D vector2d = matrix.toVector2D();
        vector2d.erase(vector2d.begin() + (long) m);
        for (auto& row : vector2d) {
            row.erase(row.begin() + (long) n);
        }
        return Matrix{vector2d};
    }

    double determinant(const Matrix& matrix) {
        if (matrix.shape()[0] == matrix.shape()[1]) {
            if (matrix.shape()[0] == 2) {
                return matrix.at(0, 0) * matrix.at(1, 1) - matrix.at(0, 1) * matrix.at(1, 0);
            }
            else
            {
                Matrix upper_triangle = upper_triangular(matrix);
                    double res = 1;
                    for (int row = 0; row < upper_triangle.shape()[0]; row++) {
                        res *= upper_triangle.at(row, row);
                    }
                    return res;
            }
        }
        else {
            throw IllegalArithmeticsException{"Cannot calculate determinant for a non-square matrix."};
        }
    }

    Matrix adjugate(const Matrix& matrix) {
        Vector2D adjugate_vec2d = matrix.toVector2D();
        for (int row = 0; row < matrix.shape()[0]; row++) {
            for (int col = 0; col < matrix.shape()[1]; col++) {
                adjugate_vec2d[row][col] =
                        std::pow(-1, row + col + 2) * determinant(minor(matrix, row, col));
            }
        }
        return Matrix{adjugate_vec2d}.T();
    }

    Matrix invert(const Matrix& matrix) {
        double det = determinant(matrix);
        if (det == 0)
        {
            throw IllegalArithmeticsException{"The given matrix has no invert since its determinant is zero."};
        }

        Matrix res = adjugate(matrix) / det;
        return res;
    }

    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0) {
        if (axis == 0) {
            // Concatenate vertically
            if (matrix1.shape()[1] == matrix2.shape()[1]) {
                Vector2D res_vec2d = matrix1.toVector2D();
                Vector2D vec2d_other = matrix2.toVector2D();
                res_vec2d.insert(res_vec2d.end(), vec2d_other.begin(), vec2d_other.end());
                return Matrix{res_vec2d};
            }
            else {
                throw IllegalArithmeticsException{"To concatenate two matrices through axis 0, "
                                                  "their column sizes must be same."};
            }
        }
        else if (axis == 1) {
            // Concatenate horizontally
            if (matrix1.shape()[0] == matrix2.shape()[0]) {
                Vector2D res_vec2d = matrix1.toVector2D();
                Vector2D vec2d_other = matrix2.toVector2D();
                for (int row = 0; row < res_vec2d.size(); row++) {
                    res_vec2d[row].insert(res_vec2d[row].end(), vec2d_other[row].begin(),
                                          vec2d_other[row].end());
                }
                return Matrix{res_vec2d};
            }
            else {
                throw IllegalArithmeticsException{"To concatenate two matrices through axis 1, "
                                                  "their row sizes must be same."};
            }
        }
        else {
            throw IllegalArithmeticsException("Axis is either 1 or 0.");
        }
    }

    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
        Vector2D res = matrix.toVector2D();
        std::vector<double> tmp = res[r1];
        std::transform(res[r2].begin(), res[r2].end(), res[r1].begin(),
                       [](double elem) -> double {return elem;});
        std::transform(tmp.begin(), tmp.end(), res[r2].begin(),
                       [](double elem) -> double {return elem;});
        return Matrix{res};
    }

    Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
        Vector2D res_vec2d = matrix.toVector2D();
        std::transform(res_vec2d[r].begin(), res_vec2d[r].end(), res_vec2d[r].begin(),
                       [c](double elem) -> double {
                           return elem * c;
        });
        return Matrix{res_vec2d};
    }

    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
        Vector2D res_vec2d = matrix.toVector2D();
        std::transform(res_vec2d[r1].begin(), res_vec2d[r1].end(), res_vec2d[r2].begin(),
                       res_vec2d[r2].begin(),
                       [c](double elem1, double elem2) -> double {
                           return elem2 + c * elem1;
        });
        return Matrix{res_vec2d};
    }

    Matrix upper_triangular(const Matrix& matrix) {
        Matrix eliminated_mat = matrix;
        int pivot_num = 0;

        for (int col = 0; col < eliminated_mat.shape()[1]; col++) {
            // Find the first non-zero element to be as pivot.
            // If the pivot has been found (pivot_row != -1), use it to eliminate other non-zero elements.
            int pivot_row = -1;
            for (int row = pivot_num; row < eliminated_mat.shape()[0]; row++) {
                if (eliminated_mat.at(row, col) != 0) {
                    if (pivot_row == -1){
                        // Found the pivot.
                        pivot_row = row;
                        pivot_num++;
                    }
                    else {
                        eliminated_mat = ero_sum(eliminated_mat, pivot_row,
                                -(eliminated_mat.at(row, col)/eliminated_mat.at(pivot_row, col)),
                                row);
                    }
                }
            }
            if (pivot_row != -1) {
                eliminated_mat = ero_swap(eliminated_mat, pivot_row, pivot_num - 1);
            }
        }
        return eliminated_mat;
    }

    Matrix rref(const Matrix& matrix) {
        Matrix res = upper_triangular(matrix);

        for (int r = 0; r < res.shape()[0]; r++) {
            int pivot_idx = -1;
            for (int c = 0; c < res.shape()[1]; c++) {
                if (res[r][c].num() != 0) {
                    pivot_idx = c;
                    if (res[r][c].num() != 1) {
                        res[r] /= res[r][c].num();
                    }
                    break;
                }
            }

            // Eliminate the element above this pivot element
            if (r > 0 && pivot_idx > -1 && res[r - 1][pivot_idx].num() != 0) {
                res[r - 1] += res[r] * res[r-1][pivot_idx].num();
            }
        }
        return res;
    }
}