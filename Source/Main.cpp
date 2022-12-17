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
    std::vector<float> values = {0,1.2,5,2.0,0,0,3};
    std::vector<Gorgon::Geometry::Point> points =  {{0,0}, {1,0}, {2,1}, {3,0}, {4,0}, {5,0}};

    Types::PointList list2(std::move(points));
    auto a = adaptive.FindCandidatePoints(list2, 5);
    
    return 0;
}
