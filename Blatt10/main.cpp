#include "PowerMethod.hpp"
#include <iostream>

// run code with 'g++ -std=c++11 -o PM main.cpp CSRMatrix.cpp PowerMethod.cpp DenseVector.cpp SparseAccumulator.cpp' and './PM'

#include <fstream>
#include <vector>



template <class Type>
size_t length_of_file(const std::string &path) {
    std::fstream stream(path, std::ios::binary | std::ios::in | std::ios::ate);
    size_t length = stream.tellg() / sizeof(Type);
    stream.close();
    return length;
}

// Reads a binary file that stores a sequence of values with type 'Type' and returns
// them as a std::vector<Type>
template <class Type>
std::vector<Type> read_vec(const std::string &path) {
    const size_t size = length_of_file<Type>(path);
    std::vector<Type> to_read(size);

    std::ifstream read_stream(path, std::ios::binary);
    read_stream.read(reinterpret_cast<char *>(&to_read[0]), size * sizeof(Type));
    read_stream.close();

    return to_read;
}

int main(int argc, char **argv) {
    std::vector<double> matr2_vec1{ 1.0,1/2,1.0/3,1.0/4,1.0/5 };
    std::vector<double> matr2_vec2{ 1.0/2,1.0/3,1.0/4,1.0/5,1.0/6 };
    std::vector<double> matr2_vec3{ 1.0/3,1.0/4,1.0/5,1.0/6,1.0/7 };
    std::vector<double> matr2_vec4{ 1.0/4,1.0/5,1.0/6,1.0/7,1.0/8 };
    std::vector<double> matr2_vec5{ 1.0/5,1.0/6,1.0/7,1.0/8,1.0/9 };
    std::vector<std::vector<double>> matr2_matr{ matr2_vec1, matr2_vec2, matr2_vec3, matr2_vec4, matr2_vec5 };    
    CSRMatrix matr2(matr2_matr);

    PowerMethod pm = PowerMethod(matr2);
    // pm.print();

    DenseVector initial_guess = DenseVector(5, 0);
    initial_guess.set_self(0,1);
    // initial_guess.repr();

    // auto result = pm.find_max(initial_guess, 0.0001, 20);
    // std::cout << result.first << std::endl;
    // result.second.repr();

    // auto result2 = pm.find_min(initial_guess, 0.0001, 1000);
    // std::cout << result2.first << std::endl;
    // result2.second.repr();

    // second example

    std::vector<double> matr1_vec1{ 1.0, -3.0, -2.0 };
    std::vector<double> matr1_vec2{ -1.0, -4.0, 1.0 };
    std::vector<double> matr1_vec3{ -2.0, 1.0, -3.0 };
    std::vector<std::vector<double>> matr1_matr{ matr1_vec1, matr1_vec2, matr1_vec3 };    
    CSRMatrix matr1(matr1_matr);

    PowerMethod pm1 = PowerMethod(matr1);
    pm1.print();

    DenseVector initial_guess1 = DenseVector(3, 0);
    initial_guess1.set_self(0,1);
    initial_guess1.repr();

    auto result1 = pm1.find_max(initial_guess1, 0.0001, 100);
    std::cout << result1.first << std::endl;
    result1.second.repr();

    // auto result12 = pm1.find_min(initial_guess1, 0.0001, 1000);
    // std::cout << result12.first << std::endl;
    // result12.second.repr();




    // Read the vectors to initialize the sparse matrix
    int number = 1; // 1 to 4
    std::vector<int> system_sizes = {100, 1000, 5000, 10};
    const auto rows = read_vec<uint64_t>("sparse-systems/system-0"+std::to_string(number)+"/rows.bin"), cols = read_vec<uint64_t>("sparse-systems/system-0"+std::to_string(number)+"/cols.bin");
    const auto vals = read_vec<double>("sparse-systems/system-0"+std::to_string(number)+"/vals.bin");
    // CSRMatrix A(system_sizes[number], system_sizes[number], vals, rows, cols); // code from HA 6 does not work...
    DenseVector b(read_vec<double>("sparse-systems/system-0"+std::to_string(number)+"/b.bin"));

    // PowerMethod pm_system = PowerMethod(A);
    // auto res_max = pm_system.find_max(b, 0.0001, 1000);
    // auto res_min = pm_system.find_min(b, 0.0001, 1000);
}

/*
Solution Exercise 26

Verschiebung von A um lambda1*I (A-lambda1*I) und dann Berechnung des kleinsten Eigenwertes der nicht Null ist, d.h. lambda2. 
Da die Differenz am geringsten zwischen lambda1 und lambda2 ist, aber lambda1 durch die Verschiebung auf 0 gesetzt wurde.

Analog um den zweit-kleinsten Eigenwert zu berechnen.
*/