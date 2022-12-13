#include <iostream>
#include <vector>

#include "Processing/AdaptiveDetection.h"
int main() {
    std::cout << "Hello World !" << std::endl;
    std::vector<Types::PointList> list = {};
    list.push_back({{3,5}});
    CornerDetections::AdaptiveDetection adaptive(std::move(list));
    std::cout<<adaptive.Clamp(5,3,11)<<std::endl;
    std::cout<<adaptive.Clamp(5,3,2)<<std::endl;
    std::cout<<adaptive.Clamp(5,1,-2)<<std::endl;
    std::cout<<adaptive.Clamp(5,3,-1)<<std::endl;
    std::cout<<adaptive.Clamp(5,1,4)<<std::endl;
    std::cout<<adaptive.Clamp(5,2,-3)<<std::endl;
    std::cout<<adaptive.Clamp(5,1,-8)<<std::endl;

    return 0;
}
