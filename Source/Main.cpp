#include <iostream>
#include <vector>

#include "Processing/AdaptiveDetection.h"
void show(const Types::PointList &list) {
    for(auto &point : list) {
        std::cout<<"{"<<point.X<<","<<point.Y<<"} ";
    }
    
    std::cout<<std::endl;
    
}

int main() {
    Types::PointList list = {{3,0}, {4,0}, {5,1}};
    show(list);
    
    list.Erase(0);
    show(list);
    
    /*
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
    Types::PointList testlist = {
        {0, 1}, {1, 1}, {2, 1}, {3, 0}, {4, 0}, {5, 0}, {6, 1}, {7, 1}, {8, 1}
    };
    std::vector<int> expected = {3, 4, 5};
    std::vector<int> nlist = adaptive.FindSalientPoints(testlist, 0);*/
    return 0;
}
