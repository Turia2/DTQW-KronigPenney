#include "Shift.hpp"
#include "Cayley.hpp"
#include <vector>
#include <cmath>

Shift::Shift(const Cayley_Graph& g, double phi): graph_(g), phi_(phi) {}

SpMat Shift::build() const{

    int dim = 2*graph_.N;
    SpMat S{dim, dim};
    std::vector<Eigen::Triplet<Scalar>> triplets;
    triplets.reserve(2*graph_.N);

    Scalar phase_r(std::cos(phi_), std::sin(phi_));
    Scalar phase_l(std::cos(phi_), std::sin(-phi_));

    for(int x = 0; x < graph_.N; x++){

        int xr = (x+1)%graph_.N;
        int xl = (x-1+graph_.N)%graph_.N;
        
        triplets.push_back({graph_.indx(xr ,0), graph_.indx(x, 0), phase_r});
        triplets.push_back({graph_.indx(xl, 1), graph_.indx(x, 1), phase_l});
    }

    S.setFromTriplets(triplets.begin(), triplets.end());
    return S;
}
