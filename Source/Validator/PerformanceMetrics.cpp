#include "PerformanceMetrics.h"

#include <limits>

#include "../Processing/Common.h"

namespace Validator {
std::vector<std::tuple<std::string, double>> GeneralMetrics::CalculateMetrics(
    /*
     * ng = number of ground-truth corners(Real corners)
     * nt = number of detected corners after transformation
     * no = nnumber of detected corners on the input image before transformation
     */
    Types::PolygonMap original_map, Types::PolygonMap predicted_map) {
  if (original_map.size() != predicted_map.size())
    throw std::runtime_error(
        "Original polygon size is not matched with predicted !");
  auto get_precision = [](const int &nt, const int &nr) {
    return nt > 0 ? static_cast<double>(nr) / nt : 0.0;
  };

  auto get_recall = [](const int &nr, const int &ng) {
    return ng > 0 ? static_cast<double>(nr) / ng : 0.0;
  };

  // arichtmetic mean of precision and recall
  auto get_apr = [](const double &precision, const double &recall) {
    return (precision + recall) / 2;
  };

  auto get_fscore = [](const double &precision, const double &recall) {
    return (2 * precision * recall) / (precision + recall);
  };

  // Average repeatability
  auto get_ar = [](const int nr, const int &nt, const int &no) {
    return 100 * (static_cast<double>(nr) / (nt + no));
  };

  ResetMetrics();

  for (auto original = std::begin(original_map),
            predicted = std::begin(predicted_map);
       original != std::end(original_map); ++original, ++predicted) {
  }
}

cv::Point GeneralMetrics::FindMatchedPoints(cv::Point original_point,
                                            const Types::CvPointList predicted,
                                            std::size_t &found_index,
                                            double th) {
  cv::Point matched = {-1, -1};
  double min_distance = std::numeric_limits<double>::max();
  for (std::size_t i = 0; i < predicted.size(); ++i) {
    double distance = CornerDetections::Common::EucledianDistance(
        original_point, predicted[i]);
    if (distance < min_distance && distance < th) {
      min_distance = distance;
      matched = predicted[i];
      found_index = i;
    }
  }
}

}  // namespace Validator
