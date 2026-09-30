#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <Eigen/Sparse>
#include "../include/Cayley.hpp"
#include "../include/Coin.hpp"

using namespace std; 

const double tol = 1e-12;

Eigen::MatrixXcd expected_coin(const Cayley_Graph& graph, const vector <double>& theta_site){

    Eigen::MatrixXcd E = Eigen::MatrixXcd::Zero(2*graph.N, 2*graph.N);
    for(int x = 0; x < graph.N; x++){

        int u = graph.indx(x, 0);
        int d = graph.indx(x, 1);
        E(u,u) = cos(theta_site[x]);
        E(u,d) = Scalar(0.0, sin(theta_site[x]));
        E(d,u) = Scalar(0.0, sin(theta_site[x]));
        E(d,d) = cos(theta_site[x]);

    }
    return E;

}

double unitarity_error(const SpMat& C){

    Eigen::MatrixXcd Cd = Eigen::MatrixXcd(C);
    return(Cd.adjoint()*Cd-Eigen::MatrixXcd::Identity(C.rows(), C.cols())).norm();
}

void check(double err, const char* label){

    cout << label << "-> err =" << err << "\n";
    assert(err<tol);
}

int main(){

    const int N = 6;
    Cayley_Graph graph(N);
    const int dim = 2*N;
    const double theta = 0.37;

    Coin coin(graph, 0.0);
    SpMat C = coin.build();
    assert(C.rows() == dim && C.cols() == dim);
    assert(C.nonZeros() == 4*N);
    Eigen::MatrixXcd Cd = Eigen::MatrixXcd(C);
    Eigen::MatrixXcd I = Eigen::MatrixXcd::Identity(dim,dim);
    check((Cd-I).norm(), "theta = 0");

    SpMat C_theta = Coin(graph, theta).build();
    vector<double> uniform(N, theta);
    check((Eigen::MatrixXcd(C_theta)-expected_coin(graph, uniform)).norm(), "generic theta:");
    check(unitarity_error(C_theta), "generic theta: ||C - I||");

    SpMat C_flip = Coin(graph, M_PI/2).build();
    State flipped = C_flip*graph.initial_state(2,0);
    State target = Scalar(0.0, 1.0)*graph.initial_state(2,1);
    check((flipped-target).norm(), "theta = pi/2: ||C|2,up>-i|2,down>||");

    State psi = C_theta*graph.initial_state(4, 0);
    State psi_target = cos(theta)*graph.initial_state(4, 0) + Scalar(0.0, sin(theta))*graph.initial_state(4, 1);
    check((psi - psi_target).norm(), "accion sobre |4,up>");
    check(abs(psi.norm() - 1.0), "norma tras aplicar C");
    
    Coin coin_z(graph, theta, {{1, 0.9}, {-1, 1.3}, {N + 2, 0.2}});
    SpMat C_z = coin_z.build();
    vector<double> theta_z(N, theta);
    theta_z[1] = 0.9;
    theta_z[N - 1] = 1.3;
    theta_z[2] = 0.2;
    check((Eigen::MatrixXcd(C_z) - expected_coin(graph, theta_z)).norm(), "zeeman: ||C - C_esperada||");
    check(unitarity_error(C_z), "zeeman: ||C^dag C - I||");


    SpMat C_dup = Coin(graph, theta, {{3, 0.1}, {3, 0.5}}).build();
    vector<double> theta_dup(N, theta);
    theta_dup[3] = 0.5;
    check((Eigen::MatrixXcd(C_dup) - expected_coin(graph, theta_dup)).norm(), "zeeman repetido: gana el ultimo");

    Cayley_Graph graph1(1);
    SpMat C1 = Coin(graph1, theta).build();
    assert(C1.rows() == 2 && C1.cols() == 2);
    check((Eigen::MatrixXcd(C1) - expected_coin(graph1, {theta})).norm(), "N = 1: ||C - C_esperada||");

    cout << "\n All good.\n";
    return 0;

}