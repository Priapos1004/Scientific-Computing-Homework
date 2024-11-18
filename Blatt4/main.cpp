#include "DenseVector.h"
#include "DenseMatrix.h"

// run code with 'g++ -Wall -Werror -Wextra -Wconversion -std=c++11 -o DV main.cpp densevector.cpp' and './DV'

int main() {
    DenseVector vec1(10, 2);
	vec1.repr();

    std::vector<double> vec2_vec{ 1.0, 2.0, 3.0 };
    DenseVector vec2(vec2_vec);
    vec2.repr();

    DenseMatrix matr1(4,5,3);
    matr1.repr();

    std::vector<double> matr2_vec1{ 1.0, 2.0, 3.0 };
    std::vector<double> matr2_vec2{ 2.0, 3.0, 4.0 };
    std::vector<double> matr2_vec3{ 3.0, 4.0, 5.0 };
    std::vector<double> matr2_vec4{ 4.0, 5.0, 6.0 };
    std::vector<std::vector<double>> matr2_matr{ matr2_vec1, matr2_vec2, matr2_vec3, matr2_vec4 };    
    DenseMatrix matr2(matr2_matr);
    matr2.repr();

    DenseMatrix matr3(3,6,3);
    DenseVector c = matr3.multiply(vec2);
    c.repr();

	return 0;
};