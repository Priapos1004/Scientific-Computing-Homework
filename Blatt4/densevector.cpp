#include "DenseVector.h"
#include <string>


DenseVector::DenseVector(std::vector<double>::size_type n, double v) {
    length = n;
    self = std::vector<double> (n, v);
}

DenseVector::DenseVector(std::vector<double> vector) {
    length = vector.size();
    self = vector;
}

void DenseVector::repr() {
    std::string output_string = "DenseVector(";
    for (double i: self)
        output_string += std::to_string(i) + ", ";
    output_string += ")";
    std::cout << output_string << std::endl;
}

std::vector<double> DenseVector::get_self() {
    return self;
}

void DenseVector::set_self(std::vector<double>::size_type idx, double value) {
    self[idx] = value;
}
