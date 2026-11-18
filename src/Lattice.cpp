#include "Lattice.hpp"
#include <stdexcept>
#include <vector>
#include <utility>

Lattice::Lattice(const Cayley_Graph& g, int period, int width, double B, int offset):graph_(g), period_(period), width_(width), B_(B), offset_(offset){

    if(period_ <= 0){
        throw std::invalid_argument("period must be positive");
    }
    if(width_ < 0 || width_ > period_){
        throw std::invalid_argument("width must be in [0, period]");
    }
    if(graph_.N%period_ != 0){
        throw std::invalid_argument("N must be a multiple of period");
    }
    offset_ = (offset_%graph_.N + graph_.N)%graph_.N;

    


}

bool Lattice::is_barrier(int x) const{
        
        int r = ((x - offset_)%period_ + period_)%period_;
        return r < width_;
}

std::vector<int> Lattice::barrier_sites() const{

    std::vector<int> sites;
    sites.reserve(cells()*width_);
    for(int x = 0; x < graph_.N; x++){
        if(is_barrier(x)){
            sites.push_back(x);
        }
    }
    return sites;
}

std::vector<std::pair<int, double>> Lattice::zeeman() const{

    std::vector<std::pair<int, double>> z;
    z.reserve(cells()*width_);
    for(int x: barrier_sites()){
        z.emplace_back(x, B_);
    }
    return z;
}