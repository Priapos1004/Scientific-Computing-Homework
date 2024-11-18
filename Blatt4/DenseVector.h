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
};