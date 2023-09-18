#ifndef CTAR_H_
#define CTAR_H_
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

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

  void PreProcessing() {
    std::tie(contours, edges) = Common::GetContour(image);
  }

  Types::CvPointList CornerDetection(const Types::CvPointList &list) {
    Types::CvPointList response;
    // Corner map incldue index of the corners for iterating indexes
    std::vector<Types::PointMap<cv::Point>> corner_map;
    for (std::size_t index = 0; index < list.size(); ++index) {
      if (IsCurve(index, list)) corner_map.push_back({index, list[index]});
    }
    return response;
  }

 private:
  bool IsCurve(std::size_t index, const Types::CvPointList &contour) {
    if (index >= contour.size())
      throw std::out_of_range("index is more than contour size !");
    auto distances = GetDistances(index, contour);
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

  std::tuple<float, float, float> GetDistances(
      std::size_t index, const Types::CvPointList &contour) const {
    if (index >= contour.size())
      throw std::out_of_range("index is more than contour size !");

    auto eucledian = [](cv::Point p1, cv::Point p2) {
      int dx = p2.x - p1.x;
      int dy = p2.y - p1.y;
      return std::sqrt(dx * dx + dy * dy);
    };

    // origin
    cv::Point p0 = contour[index];

    int n_index = (index - k) <= 0 ? 0 : (index - k);
    // backward
    cv::Point p1 = contour[n_index];

    n_index = (index + k) >= contour.size() ? contour.size() - 1 : index + k;

    cv::Point p2 = contour[n_index];

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
