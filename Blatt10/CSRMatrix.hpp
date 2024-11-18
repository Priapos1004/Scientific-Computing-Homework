#pragma once
#include <vector>
#include "DenseVector.h"

class CSRMatrix {
    std::vector<int> IR;
    std::vector<int> JC;
    std::vector<double> Num;
    int num_rows;
    int num_columns;

    public:
        // takes nnz and their indices to build a CSRMatrix
        CSRMatrix(int num_rows, int num_columns,const std::vector<double>& nnz,const std::vector<int>& row_indices,const std::vector<int>& col_indices);
        // takes a vector of vectors to build a CSRMatrix
        CSRMatrix(std::vector<std::vector<double>> init);
        // minimal information constructor
        CSRMatrix(int num_rows, int num_columns);
        // output as string in console
        void print() const;
        // SpGEMM
        CSRMatrix operator*(const CSRMatrix &rhs) const;
        DenseVector operator*(const DenseVector &rhs) const;
        DenseVector CG(const DenseVector &b, int max_iters) const;
};