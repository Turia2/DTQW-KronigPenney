#include <iostream>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <Eigen/Sparse>
#include "../include/Cayley.hpp"
#include "../include/Coin.hpp"
#include "../include/Shift.hpp"
#include "../include/Walk.hpp"

using namespace std;
const double tol = 1e-12;

void check(double err, const char* label){
    
    cout << label << " err = " << err << "\n";
    assert(err < tol);
}

void test_exceptions(){

    Cayley_Graph g(5);
    Coin coin(g, 0.0);
    SpMat U = coin.build();

    bool threw_psi = false;
    try{
        State bad_psi = State::Zero(4);
        Walk walk(g, U, bad_psi);
    
    }catch(const invalid_argument& e){
        threw_psi = true;
    }

    assert(threw_psi && "mal tamaño de psi0" );

    bool threw_U = false;
    try{

        SpMat bad_U(2,2);
        State psi = g.initial_state(0,0);
        Walk walk(g, bad_U, psi);

    } catch(const invalid_argument& e){
        threw_U = true;
    }
    assert(threw_U && "mal tamaño de U");

}


void test_tirival_walk(){

    int N = 5;
    Cayley_Graph g(N);
    Coin coin(g, 0.0);
    Shift shift(g, 0.0);
    SpMat U = shift.build()*coin.build();

    int x0 = 2;
    State psi0 = g.initial_state(x0, 0.0);
    Walk walk(g, U, psi0);
    walk.step();
    check(abs(walk.norm() -1.0), "norm after a single step");
    
    Eigen::VectorXd P = walk.probabilities();
    check(abs(P(x0+1) - 1.0), "probability = 1 in x0 + 1");
    check(abs(P(x0)-0.0), "probability  = 0 in x0 ");
    check(abs(mean_position(P, x0) - 1.0), "mean_position moved + 1");
    check(abs(variance(P, x0) - 0.0), "variance = 0");
}

void test_run_wrapped(){

    int N = 5;
    Cayley_Graph g(N);
    Coin coin(g, 0.0);
    Shift shift(g, 0.0);
    SpMat U = shift.build()*coin.build();

    int x0 = 2;
    Walk walk(g, U, g.initial_state(x0, 0.0));
    assert(!wrapped(walk.probabilities(), x0));
    walk.run(2);
    assert(wrapped(walk.probabilities(), x0) == true);
}

int main(){

    cout << "Testing Walk \n";
    test_exceptions();
    test_tirival_walk();
    test_run_wrapped();
    cout << "\n All good\n";
    return 0;
}

