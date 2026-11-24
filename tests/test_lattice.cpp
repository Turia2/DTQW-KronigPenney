#include <iostream>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <utility>
#include "Cayley.hpp"
#include "Lattice.hpp"
#include "Coin.hpp"
#include "Shift.hpp"
#include <Eigen/Dense>

using namespace std;
const double tol = 1e-12;

template <class F>
bool throw_invalid(F f){
    try{ f(); }
    catch(const std::invalid_argument&){ return true; }
    return false;
}

void check_sites(const Lattice& L, const vector <int>& expected, const char* label){
    
    vector<int> sites = L.barrier_sites();
    cout << label << "->" << sites.size() << "barrier sites\n";
    assert(sites == expected);
}

Eigen::MatrixXcd translation(const Cayley_Graph& g, int a){
    const int dim = 2*g.N;
    Eigen::MatrixXcd T = Eigen::MatrixXcd::Zero(dim, dim);
    for(int x = 0; x < g.N; x++){

        for(int s = 0; s < 2; s++){
            T(g.indx(x + a, s), g.indx(x, s)) = 1.0;
        }

    }
    return T;
}

double commutation_error(const Eigen::MatrixXcd& T, const Eigen::MatrixXcd& U){
    return (T*U*T.adjoint() - U).norm();
}


int main(){

    const int N = 12;
    Cayley_Graph graph(N);

    assert(throw_invalid([&]{ Lattice(graph, 0, 0, 1.0);}));
    assert(throw_invalid([&]{ Lattice(graph, -3, 1, 1.0); }));
    assert(throw_invalid([&]{ Lattice(graph, 4, -1, 1.0); }));
    assert(throw_invalid([&]{ Lattice(graph, 4, 5, 1.0); }));
    assert(throw_invalid([&]{ Lattice(graph, 5, 1, 1.0); }));
    
    cout << "invalid parameters\n";

    const double B = 0.75;
    Lattice L(graph, 4,2,B);
    assert(L.cells() == 3);
    assert(L.period() == 4);
    assert(L.width() == 2);
    assert(L.offset() == 0);
    assert(abs(L.B() - B) < tol);
    check_sites(L, {0, 1, 4, 5, 8, 9}, "period 4, width 2");
    assert(L.barrier_sites().size() == static_cast<size_t>(L.cells()*L.width()));

    for(int x = -2*N; x < 2*N; x++){
        assert(L.is_barrier(x) == L.is_barrier(x+L.period()));
        assert(L.is_barrier(x) == L.is_barrier(x+N));
    }
    assert(L.is_barrier(-4));
    assert(!L.is_barrier(-1));
    cout << "is_barrier is periodic \n";

    Lattice Lo(graph, 4,2,B,1);
    assert(Lo.offset() == 1);
    check_sites(Lo, {1,2,5,6,9,10}, "offset 1");

    Lattice Lw(graph, 4,2,B,3);
    check_sites(Lw, {0,3,4,7,8,11}, "offset 3");

    Lattice Ln(graph, 4,2,B,-1);
    assert(Ln.offset() == N-1);
    check_sites(Ln, {0,3,4,7,8,11}, "offset -1");
    
    Lattice Lbig(graph, 4,2,B,N+1);
    assert(Lbig.offset() == 1);
    check_sites(Lbig, Lo.barrier_sites(), "offset N+1");

    Lattice Lfree(graph, 4, 0, B);
    check_sites(Lfree, {}, "width 0");
    assert(Lfree.zeeman().empty());

    Lattice Lfull(graph, 4, 4, B);
    vector<int> all(N);
    for(int x = 0; x < N; x++){
        all[x] = x;
    }
    check_sites(Lfull, all, "width == period");

    Lattice Lone(graph, N, 3, B, 2);
    assert(Lone.cells() == 1);
    check_sites(Lone, {2, 3, 4}, "single cell");

    vector<pair<int, double>> z = L.zeeman();
    vector<int> sites = L.barrier_sites();
    assert(z.size() == sites.size());
    for(size_t i = 0; i < z.size(); i++){

        assert(z[i].first == sites[i]);
        assert(abs(z[i].second - B) < tol);
    }
    cout << "zeeman consistent with barrier sites\n";

    const double theta = 0.37;
    for(double phi : {0.0, 0.4}){

        Coin coin(graph, theta, L.zeeman());
        Shift shift(graph, phi);
        Eigen::MatrixXcd U = Eigen::MatrixXcd(SpMat(shift.build()*coin.build()));
    
         double errL = commutation_error(translation(graph, L.period()), U);
        cout << "phi = " << phi << ": T_L U T_L^dag = U -> err = " << errL << "\n";
        assert(errL < tol);

        double err1 = commutation_error(translation(graph, 1), U);
        cout << "phi = " << phi << ": T_1 (should be large) -> err = " << err1 << "\n";
        assert(err1 > 0.1);
    
    }


   


    cout << "\n all good\n";
    return 0;


}