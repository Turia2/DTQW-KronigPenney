#include <iostream>
#include <cassert>
#include <cmath>
#include <Eigen/Sparse>
#include "../include/Cayley.hpp"
#include "../include/Coin.hpp"

using namespace std; 

int main(){

    const int N = 6;
    Cayley_Graph graph(N);
    const int dim = 2*N;

    Coin coin(graph, 0.0);
    SpMat C = coin.build();
    assert(C.rows() == dim && C.cols() == dim);
    assert(C.nonZeros() == 4*N);
    Eigen::MatrixXcd Cd = Eigen::MatrixXcd(C);
    Eigen::MatrixXcd I = Eigen::MatrixXcd::Identity(dim,dim);
    double err = (Cd - I).norm();
    cout << "theta = 0 -> ||C - I|| =" << err << "\n";
    assert(err < 1e-12);

    

}