#include "CSRMatrix.hpp"
#include <iostream>

// run code with 'g++ -std=c++11 -o CSR main.cpp CSRMatrix.cpp SparseAccumulator.cpp' and './CSR'

int main() {

    CSRMatrix matr1(4,5);
    matr1.print();

    std::cout << "\b\b " << std::endl;

    std::vector<double> matr2_vec1{ 4,0,2,0,0 };
    std::vector<double> matr2_vec2{ 0,0,0,0,2 };
    std::vector<double> matr2_vec3{ 1,1,0,1,0 };
    std::vector<double> matr2_vec4{ 0,0,0,4,1 };
    std::vector<double> matr2_vec5{ 0,0,3,0,0 };
    std::vector<std::vector<double>> matr2_matr{ matr2_vec1, matr2_vec2, matr2_vec3, matr2_vec4, matr2_vec5 };    
    CSRMatrix matr2(matr2_matr);
    matr2.print();

    std::cout << "\b\b " << std::endl;

    std::vector<double> matr3_vec1{ 0,3,0,0,1 };
    std::vector<double> matr3_vec2{ 2,0,2,3,0 };
    std::vector<double> matr3_vec3{ 0,0,0,2,2 };
    std::vector<double> matr3_vec4{ 0,4,1,0,0 };
    std::vector<double> matr3_vec5{ 2,0,0,1,0 };
    std::vector<std::vector<double>> matr3_matr{ matr3_vec1, matr3_vec2, matr3_vec3, matr3_vec4, matr3_vec5 };    
    CSRMatrix matr3(matr3_matr);
    matr3.print();

    std::cout << "\b\b " << std::endl;

    CSRMatrix C = matr2*matr3;
    C.print();

	return 0;
};