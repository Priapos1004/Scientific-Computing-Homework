#pragma once
#include <vector>
#include <list>

class SPA {
    std::vector<double> w;
    std::vector<bool> b;
    std::list<int> LS;

    public:
        SPA(int col_num);
        void reset();
        void accumulate(double value, int pos);
        int output(std::vector<double> &val, std::vector<int> &col, int nzcur);
};