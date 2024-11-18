#include "SparseAccumulator.hpp"
#include <iostream>

SPA::SPA(int col_num) {
    w = std::vector<double>(col_num, 0);
    b = std::vector<bool>(col_num, false);
    LS = std::list<int>();
}

void SPA::reset() {
    for (int idx: LS) {
        w[idx] = 0;
        b[idx] = false;
    }
    LS.clear();
}

void SPA::accumulate(double value, int pos) {
    if (b[pos] == false){
        b[pos] = true;
        LS.push_back(pos);
    }
    w[pos] = w[pos] + value;
}

int SPA::output(std::vector<double> &val, std::vector<int> &col, int nzcur) {
    int nzi = 0;
    for (int idx: LS) {
        col[nzcur + nzi] = idx;
        val[nzcur + nzi] = w[idx];
        nzi = nzi + 1;
    }
    return nzi;
}