#include "Coin.hpp"
#include <utility>
#include <cmath>

Coin::Coin(const Cayley_Graph& g, double theta,std::vector<std::pair<int, double>> z):
        graph(g), theta_(theta), zeeman_(std::move(z)) {}

SpMat Coin::build() const{

    int dim = 2*graph.N;
    SpMat C(dim, dim);
    std::vector<Eigen::Triplet<Scalar>> triplets;
    triplets.reserve(4*graph.N);

    std::vector<double> theta_site(graph.N, theta_);
    for(const auto& [node, t] :zeeman_) theta_site[node] = t;
    
    for(int x = 0; x < graph.N; x++){

        double c = std::cos(theta_site[x]);
        Scalar ic(0.0, std::sin(theta_site[x]));
        int u = graph.indx(x, 0);
        int d = graph.indx(x, 1);
        triplets.push_back({u, u, Scalar(c)});
        triplets.push_back({u, d, ic});
        triplets.push_back({d, u, ic});
        triplets.push_back({d, d, Scalar(c)});
    }

    C.setFromTriplets(triplets.begin(), triplets.end());
    return C;
}