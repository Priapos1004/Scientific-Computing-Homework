#include "PowerMethod.hpp"

PowerMethod::PowerMethod(const CSRMatrix &A): A(A) {};

void PowerMethod::print(){
    A.print();
}

std::pair<double, DenseVector> PowerMethod::find_max(const DenseVector& initial_guess, double tol, int max_iters) {
    DenseVector x_old = initial_guess;
    DenseVector x_new = DenseVector(x_old.get_length(), 0);
    DenseVector eigenvalues = DenseVector(max_iters, -1);
    for(int k=1; k <= max_iters; k++) {
        x_new = A*x_old;
        eigenvalues.set_self(k-1, x_new.norm1()/x_old.norm1());
        double max = x_new.get_max_value();
        for(int i=0; i<x_new.get_length();i++){
            x_new.set_self(i, x_new.get_value(i)/max);
        }
        if(k>2 and std::abs(eigenvalues.get_value(k-1)-eigenvalues.get_value(k-2))<tol){
            std::cout << "Eigenvalues for max: " << std::endl; // outputs the eigenvalues of the different iterations
            eigenvalues.repr();
            return std::make_pair(eigenvalues.get_value(k-1), x_new);
        }
        x_old = x_new;
    }
    std::cout << "Eigenvalues for max: " << std::endl; // outputs the eigenvalues of the different iterations
    eigenvalues.repr();
    return std::make_pair(eigenvalues.get_value(max_iters-1), x_new);
}

std::pair<double, DenseVector> PowerMethod::find_min(const DenseVector& initial_guess, double tol, int max_iters) {
    DenseVector x_old = initial_guess;
    DenseVector x_new = DenseVector(x_old.get_length(), 0);
    DenseVector eigenvalues = DenseVector(max_iters, -1);
    for(int k=1; k <= max_iters; k++) {
        x_new = A.CG(x_old, max_iters);
        eigenvalues.set_self(k-1, x_new.norm1()/x_old.norm1());
        double max = x_new.get_max_value();
        for(int i=0; i<x_new.get_length();i++){
            x_new.set_self(i, x_new.get_value(i)/max);
        }
        if(k>2 and std::abs(eigenvalues.get_value(k-1)-eigenvalues.get_value(k-2))<tol){
            std::cout << "Eigenvalues for min: " << std::endl; // outputs the eigenvalues of the different iterations
            eigenvalues.repr();
            return std::make_pair(eigenvalues.get_value(k-1), x_new);
        }
        x_old = x_new;
    }
    std::cout << "Eigenvalues for min: " << std::endl; // outputs the eigenvalues of the different iterations
    eigenvalues.repr();
    return std::make_pair(eigenvalues.get_value(max_iters-1), x_new);
}
