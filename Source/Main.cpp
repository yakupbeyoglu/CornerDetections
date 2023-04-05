#include <iostream>
#include <vector>
#include "Processing/Ctar.hpp"
#include "Processing/Common.h"
void show(const Types::PointList &list) {
    for(auto &point : list) {
        std::cout<<"{"<<point.X<<","<<point.Y<<"} ";
    }

    std::cout<<std::endl;

}

int main() {
    std::vector<cv::Point> tjunctionlist = {{0,0}, {0,3}, {4,6}, {5,5}, {9,10}, {5,5}, {0,3}};
    auto t_junctions = CornerDetections::Common::FindTJunctions(tjunctionlist);
    std::cout << CornerDetections::Common::StreamPointList(t_junctions) << std::endl;
    return 0;
    cv::Mat image = cv::imread("/media/yakup/Samsung980/remote/EdgeDetections/Bin/ataturk.jpg");
    assert(!image.empty());
    
    auto contour = CornerDetections::Common::GetContour(image);
    Types::CvPointList list;
    cv::Mat detected;
    std::tie(list, detected) = contour;
    
    std::cout << "contours " << CornerDetections::Common::StreamPointList(list)<< std::endl;
    auto pointed = CornerDetections::Common::DrawPoints(cv::Size(image.rows, image.cols), list);
    CornerDetections::Ctar c(image);


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
