#include <iostream>
#include <cassert>
#include <cmath>
#include <complex>
#include <Eigen/Sparse>
#include "../include/Cayley.hpp"
#include "../include/Shift.hpp"

using namespace std;

const double tol = 1e-12;
void check(double err, const char* label){
    
    cout << label << "-> err = " << err << "\n";
    assert(err < tol);
}

double unitarity_error(const SpMat& S){

    Eigen::MatrixXcd Sd = Eigen::MatrixXcd(S);
    return(Sd.adjoint()*Sd - Eigen::MatrixXcd::Identity(S.rows(), S.cols())).norm();
}

Eigen::MatrixXcd calc_SN(const Cayley_Graph& g, double phi){

    Eigen::VectorXcd d(2*g.N);
    for(int i = 0; i < g.N; i++){
        d(g.indx(i,0)) = polar(1.0, g.N*phi);
        d(g.indx(i,1)) = polar(1.0,-g.N*phi);
    }
    return d.asDiagonal();
}


Eigen::MatrixXcd power(const SpMat& S, int n){
    Eigen::MatrixXcd Sd = Eigen::MatrixXcd(S);
    Eigen::MatrixXcd P = Eigen::MatrixXcd::Identity(S.rows(), S.cols());
    for(int k = 0; k < n; k++){

        P = Sd*P;
    }
    return P;

}


int main(){

    const int N = 6;
    const int dim = 2*N;
    const double phi = 0.37;
    Cayley_Graph graph(N);
    const Scalar e_r = polar(1.0, phi);
    const Scalar e_l = polar(1.0, -phi);

    SpMat S0 = Shift(graph).build();
    SpMat S = Shift(graph, phi).build();
    assert(S.rows() == dim && S.cols() == dim);
    assert(S.nonZeros() == 2*N);

    check(unitarity_error(S0), "||S0^dag*S0 - I|| = ");
    check(unitarity_error(S), "||S^dag*S- I|| = ");

    State up = S*graph.initial_state(2, 0);
    check((up - e_r*graph.initial_state(3,0)).norm(), "S|2, up> = exp(i*phi)|3, up>");
    State down = S*graph.initial_state(2, 1);
    check((down - e_l*graph.initial_state(1,1)).norm(), "S|2, down> = exp(-i*phi)|1, down>");

    {
    State up = S*graph.initial_state(N-1, 0);
    check((up - e_r*graph.initial_state(0, 0)).norm(), "S|N-1, up> = exp(i*phi)|0, up>");
    State down = S*graph.initial_state(0, 1);
    check((down - e_l*graph.initial_state(N-1, 1)).norm(), "S|0, down> = exp(-i*phi)|N-1,down>");
    }

    check((power(S,N)- calc_SN(graph, phi)).norm(), "S^N = diag(exp(iN*phi), exp(-iN*phi))");
    check((power(S0, N) - Eigen::MatrixXcd::Identity(dim, dim)).norm(), "S0^N = I");
    
    Cayley_Graph g1(1);
    SpMat S1 = Shift(g1, phi).build();
    check((power(S1, 1) - calc_SN(g1, phi)).norm(), "N=1: S = diag(exp(i*phi), exp(-i*phi)");
    assert(S1.nonZeros() == 2);
    Cayley_Graph g2(2);
    SpMat S2 = Shift(g2, phi).build();
    check((power(S2, 2)- calc_SN(g2, phi)).norm(), "N = 2: S^2 = diag(exp(2i*phi), exp(-2i*phi))");
    assert(S2.nonZeros() == 4);

    cout << "\n all good\n";


}



