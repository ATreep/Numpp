#ifndef NUMPP_H
#define NUMPP_H

#include <vector>
#include <cstdint>

namespace numpp {
    int ED = INT32_MAX;

    extern bool show_numpp_exception_details;

    using Vector2D = std::vector<std::vector<double>>;

    /*
     * Possible to pass negative indexes like -1 indicating the last element.
     * */
    typedef struct {
        int start_idx;
        int end_idx;
    } SignedSlice;

    /*
     * Transfer negative indexes stored with SignedSlice to positive indexes and store with Slice
     * */
    typedef struct {
        size_t start_idx;
        size_t end_idx;
    } Slice;

    typedef struct {
        /*
         * A sign to indicate whether this section is set or not.
         * state = 0: Not set section (not activated)
         * state = 1: Row selection
         * state = 2: Column selection
         * */
        size_t state;

        Slice row_slice;
        Slice col_slice;
    } Section;

    class IllegalArithmeticsException : std::exception {
    private:
        std::string message;
    public:
        explicit IllegalArithmeticsException(std::string msg);
        const char* what() const noexcept override;
    };

    class MatrixSection;

    class Matrix {
    protected:
        Vector2D _matrix;

    public:
        // General Constructor
        explicit Matrix(Vector2D vector2d);

        Matrix(std::initializer_list<std::vector<double>> initList);

        Matrix(MatrixSection matrixSection);

        // Fill Constructor
        Matrix(size_t m, size_t n, double number);

        // Copy Constructor
        Matrix(const Matrix& other);

        // Move Constructor
        Matrix(Matrix&& other) noexcept;

         /*
         * Element-wise product
         * */
        Matrix operator*(const Matrix& other) const;

        Matrix operator*(double other) const;

        Matrix operator+(const Matrix& other) const;

        Matrix operator+(double other) const;

        Matrix operator+() const;

        Matrix operator/(const Matrix& other) const;

        Matrix operator/(double other) const;

        Matrix operator-(const Matrix& other) const;

        Matrix operator-(double other) const;

        Matrix operator-() const;

        Matrix operator*=(double other);

        Matrix operator*=(const Matrix& other);

        Matrix operator+=(double other);

        Matrix operator+=(const Matrix& other);

        Matrix operator/=(double other);

        Matrix operator/=(const Matrix& other);

        Matrix operator-=(double other);

        Matrix operator-=(const Matrix& other);

        /*
         * Get section of this matrix.
         *
         * Quick assignment: When `mat[i]` as an l-value,
         * you can assign values to the origin matrix (or called the parent matrix) by `mat[i] = num` or `mat[i] = other_mat[j]`
         *
         * Index inversely: You can access the last element with negative indexes.
         * For instance, `mat[-1]` is the last row of the origin matrix; `mat[0][-2]` is the penultimate element of the fist row.
         * */
        virtual MatrixSection operator[](SignedSlice slice_numpp);
        virtual MatrixSection operator[](int index_numpp);

        Matrix& operator=(const Matrix& other);

        class Iterator {
        protected:
            Vector2D& _vector2d;

            size_t cur_row;
            size_t cur_col;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = double;
            using pointer = double*;
            using reference = double&;
            using difference_type = std::ptrdiff_t;

            class IteratorBeyondRangeException : std::exception {
            private:
                std::string message;
            public:
                explicit IteratorBeyondRangeException(std::string msg);
                const char* what() const noexcept override;
            };

            Iterator(Vector2D& vector2d, size_t row, size_t col);
            Iterator(const Iterator& iterator);
            reference operator*();

            Iterator& operator++();

            Iterator operator++(int);

            bool operator==(const Iterator& other) const;

            bool operator!=(const Iterator& other) const;

            Iterator& operator--();

            Iterator operator--(int);
            pointer operator->();
        };

        Iterator begin();
        Iterator end();

        std::vector<size_t> shape() const;

        double at(size_t x, size_t y) const;

        Matrix row(int row_index) const;

        Matrix column(int col_index) const;

        std::vector<Matrix> rows() const;

        std::vector<Matrix> columns() const;

        /*
         * Transpose form of this matrix
         * */
        Matrix T() const;

        Vector2D toVector2D() const;

        Vector2D* dataHolder();

        /*
         * Quick way to get number from a 1 by 1 matrix
         * */
        double num() const;

        virtual ~Matrix();
    };

    class MatrixSection : public Matrix {
    private:
        /*
         * operator[] will store the address of the operand matrix (parent matrix),
         * so statements like `mat[i][j] = num` can directly modify the parent matrix;
         * */
        Matrix *_parentMatrix;

        /*
         * Index combination selecting elements from the parent matrix.
         * When modifying an element through statements like `mat[i][j] = num`,
         * at which this vector's value is {{i, i + 1}, {j, j + 1}},
         * it is useful to know what elements need to be modified in the parent matrix.
         * */
        Section _indexesOfParentMatrix;
    public:
        explicit MatrixSection(Vector2D vector2D);
        MatrixSection(Vector2D vector2D, Matrix *parentMatrix, Section indexesOfParentMatrix);

        MatrixSection operator[](SignedSlice slice_numpp) override;

        MatrixSection operator[](int index_numpp) override;

        Matrix& operator=(double other);

        Matrix& operator=(const Matrix& other);

        /*
         * Different from Matrix::Iterator,
         * MatrixSection::Iterator can iterate only sliced elements which point to the origin matrix (parent matrix).
         *
         * For instance,
         *
         * Matrix mat1{{ 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }};
         * Matrix mat2 = mat1[{ 0, 2 }][{ 0, 2 }]
         * for (double elem : mat2) {
         *      std::cout << elem << std::endl;
         * }
         *
         * Output:
         * 1, 2, 4, 5
         * */
        class Iterator : public Matrix::Iterator {
        private:
            /*
             * For a slice of a matrix (the section of a matrix), its iterator should only iterate elements in this section.
             * */
            Section _section;

        public:
            Iterator(Vector2D& vector2d, size_t row, size_t col, Section sect);
            Iterator(const Iterator& iterator);
            Iterator& operator++();
            Iterator operator++(int);
            Iterator& operator--();
            Iterator operator--(int);
        };

        Iterator begin();
        Iterator end();

        ~MatrixSection() override;
    };

    /*
     * Create an m by n zero matrix.
     * */
    Matrix zeros(size_t m, size_t n);

    /*
     * Create an n by m one matrix.
     * */
    Matrix ones(size_t m, size_t n);

    /*
     * Create an identity matrix with m by m.
     * */
    Matrix identity(size_t m);

    /*
     * Create a random matrix with n by m size in range [min, max].
     * */
    Matrix random(size_t m, size_t n, int min, int max);

    /*
     * Print matrix in a beautiful way on the console.
     * */
    void show(const Matrix &matrix);

    /*
     * Create a matrix of multiplying a matrix into a constant scalar.
     * */
    Matrix multiply(const Matrix& matrix, double c);

    /*
     * Create dot-product matrix of two matrices.
     * */
    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2);

    /*
     * Create a matrix adding a constant number `c` into every element of `matrix`.
     * */
    Matrix sum(const Matrix& matrix, double c);

    /*
     * Create the sum matrix of two matrices.
     * */
    Matrix sum(const Matrix& matrix1, const Matrix& matrix2);

    /*
     * Create `matrix`'s transpose.
     * */
    Matrix transpose(const Matrix& matrix);

    /*
     * Create a minor matrix of `matrix` with respect to mth row and nth column (the indices start from 0).
     * */
    Matrix minor(const Matrix& matrix, size_t m, size_t n);

    /*
     * Calculate the determinant of `matrix`.
     * */
    double determinant(const Matrix& matrix);

    /*
     * Generate `matrix`'s inverse.
     * */
    Matrix invert(const Matrix& matrix);

    Matrix adjugate(const Matrix& matrix);

    /*
     * Concatenate two matrices into a new matrix along the specified axis (0 or 1).
     * Axis 0: Concatenate two matrices vertically
     * Axis 1: Concatenate two matrices horizontally
     * */
    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis);

    /*
     * Following functions implement elementary row operations (ERO)
     * ero_swap: Swaps r1'th row with r2'th row
     * ero_multiply: Multiplies every element in rth row with constant `c`
     * ero_sum: Adds ero_multiply(r1, c) into r2'th row
     * */
    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2);
    Matrix ero_multiply(const Matrix& matrix, size_t r, double c);
    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2);

    /*
     * Calculate upper triangular form (row echelon form, REF) of the given matrix
     * */
    Matrix upper_triangular(const Matrix& matrix);

    /*
     * Calculate RREF form (reduced row echelon form, RREF) of the given matrix
     * */
    Matrix rref(const Matrix& matrix);

}

#endif //NUMPP_H