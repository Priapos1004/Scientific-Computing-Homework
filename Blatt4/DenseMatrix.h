# pragma once
#include <iostream>
#include <vector>
#include "DenseVector.h"

class DenseMatrix {

    std::vector<double>::size_type row_num;
    std::vector<double>::size_type col_num;
    std::vector<std::vector<double>> self;

    public:
        DenseMatrix(std::vector<double>::size_type n, std::vector<double>::size_type m, double v);
        DenseMatrix(std::vector<std::vector<double>> matrix);
        void repr();
        DenseVector multiply(DenseVector vec);
        //DenseVector operator*(const DenseVector &rhs) const;
};