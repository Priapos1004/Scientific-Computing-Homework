#include "CSRMatrix.hpp"
#include "DenseVector.h"
#include <iostream>

class PowerMethod {
    const CSRMatrix A;

    public:
        PowerMethod(const CSRMatrix &A);
        std::pair<double, DenseVector> find_max(const DenseVector& initial_guess, double tol, int max_iters);
        std::pair<double, DenseVector> find_min(const DenseVector& initial_guess, double tol, int max_iters);
        void print();
};