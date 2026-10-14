#include "Walk.hpp"
#include <highfive/H5File.hpp>
#include <vector>
#include <stdexcept>
#include <utility>
#include <cmath>

Walk::Walk(Cayley_Graph g, SpMat U, State psi0, WalkParams params): graph_(g), U_(std::move(U)), psi_(std::move(psi0)), params_(params){
    
    int dim = 2*graph_.N;
    if(U_.rows() != dim || U_.cols() != dim){
        throw std::invalid_argument("U must be 2Nx2N");
    }
    if(psi_.size() != dim){
        throw std::invalid_argument("psi0 must have size 2N");
    }
}

void Walk::run(int T, int every, const std::string& path){

    if(T < 0 || every <= 0){
        throw std::invalid_argument("need T>= 0 and save_every positive");
    }
    namespace H5 = HighFive; 
    H5::File file(path, H5::File::Truncate);
    const size_t N = graph_.N;
    const size_t rows = T/every+1;
    auto ds_prob = file.createDataSet<double>("prob", H5::DataSpace({rows, N}));
    
    auto ds_norm = file.createDataSet<double>("norm",H5::DataSpace({rows}));
    file.createAttribute("N", graph_.N);
    file.createAttribute("theta", params_.theta);
    file.createAttribute("phi", params_.phi);
    file.createAttribute("B", params_.B);
    file.createAttribute("x0", params_.x0);
    file.createAttribute("timesteps", T);
    file.createAttribute("save_every", every);

    for(int t =0; t <= T; t++){
        
        if(t%every == 0){
            size_t row = t/every;
            Eigen::VectorXd P = probabilities();
            std::vector<double> prob(P.data(), P.data()+P.size());
            ds_prob.select({row, 0}, {1, N}).squeezeMemSpace({0}).write(prob);
            ds_norm.select({row}, {1}).write(std::vector<double>{norm()});
        }

        if(t<T){
            step();
        }
    }
}

void Walk::step(){
    psi_ = U_*psi_;
}

void Walk::run(int T){
    for(int t = 0; t < T; t++){
        step();
    }
}

Eigen::VectorXd Walk::probabilities() const{
    Eigen::VectorXd P(graph_.N);
    for(int x = 0; x < graph_.N; x++){
        P(x) = std::norm(psi_(graph_.indx(x,0)))+std::norm(psi_(graph_.indx(x,1)));
    }
    return P;

}

double Walk::norm() const{
    return psi_.norm();
}

namespace{
    int folded(int x, int x0, int N){
        return((x-x0+N/2)%N+N)%N-N/2;
    }
}

double mean_position(const Eigen::VectorXd& P, int x0){
    const int N = P.size();
    double mean = 0.0;
    for(int x = 0; x < N; x++){
        mean = mean+folded(x,x0,N)*P(x);
    }
    return mean;
}

double variance(const Eigen::VectorXd& P, int x0){
    const int N = P.size();
    double mean = 0.0;
    double mean2 = 0.0;
    for(int x = 0; x < N; x++){
        double d = folded(x,x0,N);
        mean = mean + d*P(x);
        mean2 = mean2 + d*d*P(x);
    }
    return mean2-mean*mean;
}

bool wrapped(const Eigen::VectorXd& P, int x0, double eps){

    const int N = P.size();
    const int lo = -(N/2);
    const int hi = lo + N - 1;
    for(int x = 0; x < N; x++){
        int d = folded(x, x0, N);
        if((d == lo || d == hi) && P(x) > eps){
            return true;
        }
    }
    return false;
}
