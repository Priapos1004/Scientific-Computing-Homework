#include "DenseMatrix.h"
#include <string>

DenseMatrix::DenseMatrix(std::vector<double>::size_type n, std::vector<double>::size_type m, double v) {
    // rows m, columns n, value v
    row_num = m;
    col_num = n;
    std::vector<double> rows = std::vector<double>(n, v);
    self = std::vector<std::vector<double>> (m, rows);
}

DenseMatrix::DenseMatrix(std::vector<std::vector<double>> matrix) {
    row_num = matrix.size();
    col_num = matrix[1].size();
    self = matrix;
}

void DenseMatrix::repr() {
    std::cout << "DenseMatrix(" << std::endl;
    for (std::vector<double> row: self) {
        std::string output_string = "(";
        for (double i: row) {
            output_string += std::to_string(i)+ ", ";
        }
        output_string += ")";
        std::cout << output_string << std::endl;
    }
    std::cout << ")" << std::endl;
}

DenseVector DenseMatrix::multiply(DenseVector vec) {
    DenseVector result(row_num, 0);
    for (std::vector<double>::size_type j = 1; j <= row_num; ++j) {
        std::vector<double> row = self[j];
        double mid_result = 0;
        for (std::vector<double>::size_type i = 1; i <= col_num; ++i) {
            mid_result += row[i] * vec.get_self()[i];
        }
        result.set_self(j, mid_result);
    }
    return result;
}

/*
DenseVector DenseMatrix::operator*(const DenseVector &rhs) const {
    DenseVector result(row_num, 0);
    for (std::vector<double>::size_type j = 1; j <= row_num; ++j) {
        std::vector<double> row = self[j];
        double mid_result = 0;
        for (std::vector<double>::size_type i = 1; i <= col_num; ++i) {
            mid_result += row[i] * rhs.get_self()[i];
        }
        result.set_self(j, mid_result);
    }
    return result;
}
*/