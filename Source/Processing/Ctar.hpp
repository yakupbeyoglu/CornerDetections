#ifndef CTAR_H_
#define CTAR_H_
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "CornerDetectors.h"
#include "Common.h"

namespace CornerDetections {
class Ctar: public CornerDetectors {
public:

    Ctar(const cv::Mat &mat, std::size_t k = 3,  float threshold = 0.989):k(k), threshold(threshold) {
        assert(threshold  > 0);
        assert(!mat.empty());
        image = mat;

    }

    ~Ctar() {}


    virtual void PreProcess()override {
        std::tie(list, edges) = Common::GetContour(image);
        
    }

    virtual Types::PointList DetectCorners(const Types::PointList &list) override {
        Types::PointList response;
        return response;
    }

    /*
    *  Get Distnace Between two points
    */
    float GetDistance(const Types::Point &p1, const Types::Point &p2
                     ) const ;
private:


    
    bool isCurve(int index) {
        auto distances = GetDistances(index);
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

    std::tuple<float, float, float> GetDistances(int index)const {
        auto eucledian = [](cv::Point p1, cv::Point p2) {
            int dx = p2.x - p1.x;
            int dy = p2.y - p1.y;
            return std::sqrt(dx * dx + dy * dy);
        };
        assert(index >= k);
        // origin
        cv::Point p0 = list[index];

        // backward
        cv::Point p1 = list[index - k];

        // forward
        cv::Point p2 = list[index + k];

        float d1 = eucledian(p1, p2);
        float d2 = eucledian(p0, p1);
        float d3 = eucledian(p0, p2);

        return std::make_tuple(d1, d2, d3);
    }


    cv::Mat image;
    cv::Mat edges;
    Types::CvPointList  list;
    int k;
    float threshold;

};


}
#endif
