#ifndef CTAR_H_
#define CTAR_H_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "Common.h"
#include "CornerDetectors.h"

namespace CornerDetections {
class Ctar : public CornerDetectors<Ctar, Types::CvPointList> {
 public:
  Ctar(const cv::Mat &mat, std::size_t k = 3, float threshold = 0.989)
      : k(k), threshold(threshold) {
    assert(threshold > 0);
    assert(!mat.empty());
    image = mat;
  }

  ~Ctar() {}

  virtual void PreProcessing() {
    std::tie(contours, edges) = Common::GetContour(image);
  }

  virtual Types::CvPointList CornerDetection(const Types::CvPointList &list) {
    Types::CvPointList response;
    // Corner map incldue index of the corners for iterating indexes
    Types::PointMap<cv::Point> corner_map;
    for (auto &point_list : list) {
    }
    return response;
  }

 private:
  bool isCurve(int index, int contour_index) {
    auto distances = GetDistances(index, contour_index);
    auto ratio = GetRatio(distances);
    return ratio <= threshold;
  }

  float GetRatio(const std::tuple<float, float, float> &distances) const {
    // c++ 17 automatic solve ties
    auto [d1, d2, d3] = distances;
    assert(d2 + d3 > 0);
    float ratio = d1 / (d2 + d3);
    return ratio;
  }

  std::tuple<float, float, float> GetDistances(int index,
                                               int contour_index) const {
    auto eucledian = [](cv::Point p1, cv::Point p2) {
      int dx = p2.x - p1.x;
      int dy = p2.y - p1.y;
      return std::sqrt(dx * dx + dy * dy);
    };

    // origin
    cv::Point p0 = contours[contour_index][index];

    int n_index = (index - k) <= 0 ? 0 : (index - k);
    // backward
    cv::Point p1 = contours[contour_index][n_index];

    n_index = (index + k) >= contours[contour_index].size()
                  ? contours[contour_index].size() - 1
                  : index + k;

    cv::Point p2 = contours[contour_index][n_index];

    float d1 = eucledian(p1, p2);
    float d2 = eucledian(p0, p1);
    float d3 = eucledian(p0, p2);

    return std::make_tuple(d1, d2, d3);
  }

  cv::Mat image;
  cv::Mat edges;

  Types::CvContours contours;
  int k;
  float threshold;
};

}  // namespace CornerDetections
#endif
