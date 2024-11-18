#include "CSRMatrix.hpp"
#include "SparseAccumulator.hpp"
#include "DenseVector.h"

#include <iostream>
#include <algorithm>

CSRMatrix::CSRMatrix(int num_rows, int num_columns,const std::vector<double>& nnz,const std::vector<int>& row_indices,const std::vector<int>& col_indices) :
num_rows(num_rows),
num_columns(num_columns)
{
    // sort into buckets for each row
    std::vector<std::vector<int>> row_buckets(num_rows, std::vector<int>());
    for (int i=0 ; i < nnz.size() ; i++)
        row_buckets[row_indices[i]].push_back(i);

    // build CSR structure
    int nnz_counter = 0;
    for (auto& row : row_buckets) {
        IR.push_back(nnz_counter);
        for (int i : row) {
            Num.push_back(nnz[i]);
            JC.push_back(col_indices[i]);
            nnz_counter++;
        }
    }
    IR.push_back(nnz_counter);
}


CSRMatrix::CSRMatrix(std::vector<std::vector<double>> init) :
num_rows(init.size()),
num_columns(init[0].size()) {
    IR.push_back(0);
    for (int r=0 ; r < num_rows ; r++) {
        int column_counter = 0;
        for (int c=0 ; c < num_columns ; c++) {
            if (init[r][c] != 0) {
                Num.push_back(init[r][c]);
                JC.push_back(c);
                column_counter++;
            }
        }
        IR.push_back(column_counter + IR[r]);
    }
}


CSRMatrix::CSRMatrix(int num_rows, int num_columns) :
num_rows(num_rows),
num_columns(num_columns) {
    IR = std::vector<int>(num_rows, 0);
    JC = std::vector<int>(num_columns*num_rows, 0);
    Num = std::vector<double>(num_columns*num_rows, -1);
}


void CSRMatrix::print() const {
    for (int r=0 ; r < num_rows ; r++) {

        // start with [0,0,0,0,....,0]
        std::vector<double> full_row(num_columns, 0);

        // iterate elements in JC/Num for this row
        // and insert them into the full_row vector
        for (int i=IR[r] ; i < IR[r+1] ; i++)
            full_row[JC[i]] = Num[i];
        // print row
        for (auto x : full_row) 
            std::cout << std::to_string(x) << ", ";
        std::cout << "\b\b " << std::endl;
    }
}


CSRMatrix CSRMatrix::operator*(const CSRMatrix &rhs) const {
    SPA spa = SPA(num_columns);
    CSRMatrix C = CSRMatrix(num_rows, rhs.num_columns);
    for (int i = 0; i<num_rows; i++) {
        for (int k = IR[i]; k < IR[i+1]; k++) {
            for (int j = rhs.IR[JC[k]]; j < rhs.IR[JC[k]+1]; j++) {
                double value = Num[k] * rhs.Num[j];
                spa.accumulate(value, rhs.JC[j]);
            }
        }
        int nznew = spa.output(C.Num, C.JC, C.IR[i]);
        C.IR[i+1] = C.IR[i] + nznew;
        spa.reset();
    }
    return C;
}

DenseVector CSRMatrix::operator*(const DenseVector &rhs) const {
    DenseVector result_vec = DenseVector(rhs.get_length(),0);
    for (int i = 0; i<num_rows; i++) {
        double new_value = 0;
        for (int k = IR[i]; k < IR[i+1]; k++) {
            new_value = new_value + Num[k] * rhs.get_value(JC[k]);
        }
        result_vec.set_self(i, new_value);
    }
    return result_vec;
}

DenseVector CSRMatrix::CG(const DenseVector &b, int max_iters) const{
    DenseVector x = DenseVector(b.get_length(),0);
    x.set_self(0,1);
    DenseVector p = b - *this*x;
    DenseVector r = DenseVector(p);
    double alpha_old = r*r;
    for(int t=0; t < max_iters; t++) {
        if(alpha_old < 0.00001){
            return x;
        }
        DenseVector v = *this*p;
        double lambda = alpha_old / (v*p);
        x = x + p*lambda;
        r = r - v*lambda;
        double alpha_new = r*r;
        p = r + p*(alpha_new/alpha_old);
        alpha_old = alpha_new;
    }
    return x;
}