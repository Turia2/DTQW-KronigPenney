#pragma once
#include <Eigen/Sparse>
#include <complex>
#include <cstdint>

using Scalar = std::complex<double>;
using State = Eigen::VectorX<Scalar>;
using SpMat = Eigen::SparseMatrix<Scalar>;

class Cayley{
    public:

        const int N; 
        explicit Cayley(int N):N(N){}

        int indx(int x, int s) const{ 
            return 2*((x % N + N) + s); 

        }
};