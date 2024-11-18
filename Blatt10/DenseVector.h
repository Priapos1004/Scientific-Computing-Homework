# pragma once
#include <iostream>
#include <vector>

class DenseVector {
    std::vector<double>::size_type length;
    std::vector<double> self;

    public:
        DenseVector(std::vector<double>::size_type n, double v);
        DenseVector(std::vector<double> vector);
        void repr();
        std::vector<double> get_self();
        void set_self(std::vector<double>::size_type idx, double value);
        std::vector<double>::size_type get_length() const;
        double get_max_value() const;
        double get_value(std::vector<double>::size_type idx) const;
        double norm1() const;
        DenseVector operator-(const DenseVector &rhs) const;
        DenseVector operator+(const DenseVector &rhs) const;
        double operator*(const DenseVector &rhs) const;
        DenseVector operator*(const double &rhs) const;
};