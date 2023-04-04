#ifndef CTAR_H_
#define CTAR_H_
#include <opencv2/opencv.hpp>
#include "CornerDetectors.h"
namespace CornerDetections {

class Ctar: public CornerDetectors {
public:

    Ctar(const cv::Mat &mat, std::size_t k = 3, unsigned float threshold = 0.989):k(k), threshold(threshold) {
        assert(!mat.empty());
        mat.copyTo(image);
    }


    virtual void PreProcess()override {
        cv::Mat edges;
        cv::Canny(image, edges);
        
    }

    virtual Types::PointList DetectCorners(const Types::PointList &list) override {
        Types::PointList response;
        return response;
    }

    /*
    *  Get Distnace Between two points
    */
    virtual float GetDistance(const Types::Point &p1, const Types::Point &p2
                             ) const override;
private:

    bool isCurve(int listindex, int index) {
        auto distances = GetDistances(listindex, index);
        auto ratio = GetRatio(distances);
        return ratio <= threshold;
    }

    float GetRatio(const std::tuple<float,float,float> &distances)const {
        // c++ 17 automatic solve ties
        auto [d1,d2,d3] = distances;
        assert(d2 + d3 > 0);
        float ratio  = d1 / (d2 + d3);
        return ratio;
    }

    std::tuple<float, float, float> GetDistances(int listindex, int index)const {
        assert(index >= k);
        // origin
        Gorgon::Geometry::Point p0 = list[listindex][index];
        // backward
        Gorgon::Geometry::Point p1 = list[listindex][index - k];
        // forward
        Gorgon::Geometry::Point p2 = list[listindex][index + k];

        float d1 = p1.Distance(p2);
        float d2 = p0.Distance(p1);
        float d3 = p0.Distance(p2);

        return std::make_tuple(d1, d2, d3);
    }


    cv::Mat image;
    std::vector<Types::PointList> list;
    int k;
    float threshold;

};


}
#endif
