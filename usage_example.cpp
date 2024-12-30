/*
 * This is a non-comprehensive example on using NumPP.
 * For more usage information, see README.md please.
 * */
#include <iostream>
#include "NumPP/Matrix2D"

using numpp::ED;

/*
 * Print a matrix with a name.
 * */
void printMatrix(const numpp::Matrix& matrix, const std::string& name) {
    std::cout << name << std::endl;
    numpp::show(matrix);
    std::cout << std::endl;
}

int main() {
    /*
     * Set this variable to true to print exception details to the console for IllegalArithmeticsException and IteratorBeyondRangeException.
     * The default is false;
     * */
    numpp::show_numpp_exception_details = false;

    // 1. Let's create a matrix A|b.
    numpp::Matrix A_b{
        {1, 2, 2, 1},
        {3, 4, 6, 5},
        {3, 6, 8, 6}
    };

    // 2. Retrieve A and b by slicing matrix A|b.

    numpp::Matrix A = A_b[{ 0, ED }][{0, 3}];
    printMatrix(A, "A =");

    numpp::Matrix b = A_b[{ 0, ED }][3];
    printMatrix(b, "b =");

    // 3. Solve the equation Ax = b by left multiply A inverse on the both sides.
    numpp::Matrix x = numpp::multiply(numpp::invert(A), b);
    printMatrix(x, "Solution of Ax = b:");

    // 4. Calculate the RREF form for A|I, and get the elimination matrix E.

    numpp::Matrix aug_mat = numpp::concatenate(A, numpp::identity(A.shape()[0]), 1);
    numpp::Matrix upper_triangle = numpp::rref(aug_mat);
    printMatrix(upper_triangle, "Gauss elimination of the augmented matrix:");

    numpp::Matrix R = upper_triangle[{0, ED}][{0, 3}];
    printMatrix(R, "R =");

    numpp::Matrix E = upper_triangle[{0, ED}][{3, ED}];
    printMatrix(E, "E =");

    // 5. Modify elements with index operator.
    numpp::Matrix border_mat = numpp::zeros(4, 4);
    border_mat[ED][0] = border_mat[ED][-1] =
            border_mat[0][ED] = border_mat[-1][ED] = 1;
    printMatrix(border_mat, "Zero matrix with ones on borders:");

    // 6. All matrices or matrix slices are iterable.
    numpp::Matrix random_mat = numpp::random(3, 3, 0, 5);
    printMatrix(random_mat, "A random matrix:");

    std::cout << "Elements of this random matrix: ";
    for (double elem : random_mat)
        std::cout << elem << " ";
    std::cout << std::endl;

    std::cout << "Elements in the left top corner square in this matrix: ";
    for (double elem : random_mat[{0, 2}][{0, 2}])
        std::cout << elem << " ";
    std::cout << std::endl;


    return 0;
}