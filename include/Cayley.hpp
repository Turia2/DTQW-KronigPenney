#pragma once
#include <Eigen/Sparse>
#include <complex>
#include <cstdint>

using Scalar = std::complex<double>;
using State = Eigen::VectorX<Scalar>;

class Cayley_Graph{
    public:

        const int N; 
        explicit Cayley_Graph(int N):N(N){}

        int indx(int x, int s) const{ 
            return 2*((x % N + N) + s); 
        }

        State ini_state(int x0, int spin) const {

            State psi = State::Zero(2*N);
            psi(indx(x0, spin)) = Scalar(1.0, 0.0);
            return psi;

        }

        State ini_superp(int x0) const{

            static const Scalar inv2 = 1.0/std::sqrt(2.0);
            State psi = State::Zero(2*N);
            psi(indx(x0,0)) = inv2;
            psi(indx(x0, 1)) = Scalar(0.0, 1.0)*inv2;
            return psi;

        }

};