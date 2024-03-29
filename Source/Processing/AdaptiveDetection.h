#ifndef AdaptiveDetection_h
#define AdaptiveDetection_h
#include <numeric>
#include <vector>

#include "CornerDetectors.h"
namespace CornerDetections {

/*
 *  This class aims to detect corner with
 *  Adaptive algortihm for corner detecting based on
 *  degree of sharpness of the contour
 *  You can find reference at below :
 *  Xiao, J., Xiang, Z., Wang, B., & Liu, Y. (2011). Adaptive algorithm for
 * corner detecting based on the degree of sharpness of the contour. Optical
 * Engineering, 50(4), 047008.
 */
class AdaptiveDetection : public CornerDetectors {
 public:
  struct LineFitting {
    double slope;  // m
    double b;
    bool overy;
  };

  AdaptiveDetection(std::vector<Types::PointList> list);
  static std::string Test() { return "TEST"; }

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

  virtual Types::PointList DetectCorners(
      const Types::PointList &list) override {
    Types::PointList response;
    return response;
  }

  /*
   *  Find the candidate points with the sharpness degreee
   */
  Types::PointList FindCandidatePoints(const Types::PointList &list,
                                       const int &step);

  /*
   *  Return Sharpness value between three points
   *  Sharpness function of the article that references above of the class
   */
  float GetSharpness(const Types::Point &p1, const Types::Point &p2,
                     const Types::Point &p3) const;

  /*
   *  Return Adaptive sharpness Threshold value
   *  According to referenced article, we should calculate
   *  The mean of Non zero sharpness values
   */
  float GetAdaptiveTh(const std::vector<float> &sharpness) const;
  /*
   *  Clamp index to cycle Pi + k and Pi - k
   */
  int Clamp(int size, int current, int k) const;

  /*
   *  Get Distnace Between two points
   */
  float GetDistance(const Types::Point &p1, const Types::Point &p2) const;

  /*
   *  Return new point list which salient points already deleted
   *  The salient points are causing the localization problem on the detection
   *  The salient points should be filtered based on the degree and curve
   * fitting
   */
  Types::PointList RemoveSalientPoints(const Types::PointList &list,
                                       double threshold = 1) const;

  /*
   *  The threshold vlaue can be changed between 1 and 1.5 according to
   *  Article (18)
   *  y =  0, if |AD| >= TS
   *  y =  1, if |AD| < TS
   *  1 <= TS <= 1.5
   *  if overy == true,  |AD| = | m * p.x + b - p.y | * (1 /  sqrt(1 + pow(m,
   * 2))) if overy == false, |AD| = | p.x - m * p.y - b |
   */
  bool IsSalient(const Types::Point &p, const LineFitting &fitinfo,
                 const double &threshold = 1) const;

  std::vector<int> FindSalientPoints(const Types::PointList &list,
                                     const int startindex = 0) const;

  /*
   *   Find direction between two points
   */
  Types::Direction FindDirection(const Types::Point &p1,
                                 const Types::Point &p2) const;

 private:
  std::vector<Types::PointList> pointlist;

  LineFitting FitLine(const Types::Point &p1, const Types::Point &p2,
                      const bool overy,
                      const Types::BCalculation &bcalculation) const;
};

}  // namespace CornerDetections

#endif
