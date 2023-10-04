#ifndef CCR_H_
#define CCR_H_
#include <algorithm>
#include <vector>

#include "CornerDetectors.h"

namespace CornerDetections {
class Ccr : public CornerDetectors<Ccr, Types::CvPointList> {
 public:
  Ccr(std::size_t k = 7, float threshold = 0.986) : k(k), threshold(threshold) {
    assert(threshold > 0);
  }

  ~Ccr() {}

  Types::CvPointList CornerDetection(const Types::CvPointList &contour) {
    Types::CvPointList corners;
    vector<double> curvature_values;

    auto eucledian_distance = [](const cv::Point &p1,
                                 const cv::Point &p2) -> double {
      return cv::norm(p1 - p2);
    };

    Types::CvPointList detected_corners;
    for (std::size_t i = k; i < contour.size() - k - 1; ++i) {
      double distance = cv::norm(contour[i - k] - contour[i + k]);
      double total = 0;
      for (int j = i - k; j < i + k - 1; j++)
        total += eucledian_distance(contour[i], contour[i + 1]);

      double distance_ratio = distance / total;
      curvature_values.emplace_back(distance_ratio);
    }

    for (std::size_t i = 0; i < curvature_values.size(); ++i) {
      // If the curvature value is less than the threshold and is a local
      // minimum, add the corresponding pixel to the vector of detected corners
      if (curvature_values[i] < threshold &&
          curvature_values[i] ==
              *min_element(std::begin(curvature_values) + i - k,
                           std::begin(curvature_values) + i + k)) {
        detected_corners.push_back(contour[i]);
      }
    }

    return detected_corners;
  }

 private:
  std::size_t k;
  float threshold;
};
}  // namespace CornerDetections
#endif
