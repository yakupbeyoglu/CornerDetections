#ifndef AdaptiveDetection_h
#define AdaptiveDetection_h
#include "CornerDetectors.h"
#include <vector>
#include <numeric>
namespace CornerDetections {

/*
 *  This class aims to detect corner with
 *  Adaptive algortihm for corner detecting based on
 *  degree of sharpness of the contour
 *  You can find reference at below :
 *  Xiao, J., Xiang, Z., Wang, B., & Liu, Y. (2011). Adaptive algorithm for corner detecting based on the degree of sharpness of the
 *  contour. Optical Engineering, 50(4), 047008.
 */
class AdaptiveDetection: public CornerDetectors {
public:
    AdaptiveDetection(std::vector<Types::PointList> list);
    static std::string Test() {
        return "TEST";
    }
    
    /*
     *  Delete copy constructors
    */
    AdaptiveDetection(const AdaptiveDetection &cpy) = delete;
    AdaptiveDetection &operator=(const AdaptiveDetection &cpy) = delete;

    /*
     *  Delete Move constructors
     */
    AdaptiveDetection(const AdaptiveDetection &&target) = delete;
    AdaptiveDetection &operator=(const AdaptiveDetection &&target) = delete;

    ~AdaptiveDetection() {}

    virtual void PreProcess() override;

    virtual Types::PointList DetectCorners(const Types::PointList &list) override {
        Types::PointList response;
        return response;
    }

    /*
     *  Find the candidate points with the sharpness degreee
     */
    Types::PointList FindCandidatePoints(const Types::PointList &list, const int &step);

    /*
     *  Return Sharpness value between three points
     *  Sharpness function of the article that references above of the class
     */
    float GetSharpness(const Types::Point& p1, const Types::Point& p2,
                       const Types::Point& p3)const;

    /*
     *  Return Adaptive sharpness Threshold value
     *  According to referenced article, we should calculate
     *  The mean of Non zero sharpness values
     */
    float GetAdaptiveTh(const std::vector<float> & sharpness) const;
    
    /*
     *  Clamp index to cycle Pi + k and Pi - k
     */
    int Clamp(int size, int current, int k) const;

    /*
     *  Get Distnace Between two points
     */
    virtual float GetDistance(const Types::Point &p1, const Types::Point &p2
                             ) const override;

    /*
    *  Return new point list which salient points already deleted
    *  The salient points are causing the localization problem on the detection
    *  The salient points should be filtered based on the degree and curve fitting
    */
    Types::PointList RemoveSalientPoints(const Types::PointList &list)const;

    std::vector<int>  FindSalientPoints(const Types::PointList &list, const int startindex = 0)const;
    /*
     *   Find direction between two points
     */
    Types::Direction FindDirection(const Types::Point &p1, const Types::Point &p2)const;
    
private:
    std::vector<Types::PointList> pointlist;
};

}

#endif

