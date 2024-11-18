#include "DenseVector.h"
#include <string>
#include <math.h>


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

std::vector<double>::size_type DenseVector::get_length() const {
    return length;
}

double DenseVector::get_max_value() const {
    double max = std::abs(self[0]);
    for (double value: self){
        if(std::abs(value)>max){
            max = std::abs(value);
        }
    }
    return max;
}

double DenseVector::get_value(std::vector<double>::size_type idx) const {
    return self[idx];
}

double DenseVector::norm1() const {
    double sum = 0;
    for(double value: self){
        sum = sum + std::abs(value);
    }
    return sum;
}

DenseVector DenseVector::operator-(const DenseVector &rhs) const{
    DenseVector result = DenseVector(length, 0);
    for(int i=0; i<length; i++){
        result.set_self(i, self[i]-rhs.get_value(i));
    }
    return result;
}

DenseVector DenseVector::operator+(const DenseVector &rhs) const{
    DenseVector result = DenseVector(length, 0);
    for(int i=0; i<length; i++){
        result.set_self(i, self[i]+rhs.get_value(i));
    }
    return result;
}

double DenseVector::operator*(const DenseVector &rhs) const{
    double result = 0;
    for(int i=0; i<length; i++){
        result = result + self[i]*rhs.get_value(i);
    }
    return result;
}

DenseVector DenseVector::operator*(const double &rhs) const{
    DenseVector result = DenseVector(length, 0);
    for(int i=0; i<length; i++){
        result.set_self(i, self[i]*rhs);
    }
    return result;
}