#include "PerformanceMetrics.h"

#include <limits>

#include "../Processing/Common.h"

namespace Validator {
Metrics GeneralMetrics::CalculateMetrics(

    Types::PolygonMap original_map, Types::PolygonMap predicted_map) {
  if (original_map.size() != predicted_map.size())
    throw std::runtime_error(
        "Original polygon size is not matched with predicted !");

  ResetMetrics();

  for (auto original = std::begin(original_map),
            predicted = std::begin(predicted_map);
       original != std::end(original_map); ++original, ++predicted) {
    avg_metrics += CalculateMetric(original->second, predicted->second, 1.5);
  }

  avg_metrics /= original_map.size();
  return avg_metrics;
}

Metrics GeneralMetrics::CalculateMetric(const Types::CvPointList original_list,
                                        Types::CvPointList predicted_list,
                                        double th) {
  /*
   * ng = number of ground-truth corners(Real corners)
   * nt = number of detected corners after transformation
   * no = nnumber of detected corners on the input image before transformation
   */
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

  Metrics metrics;
  // number of groundtruth(correct corners)
  int ng = original_list.size();

  // number of detected corner
  int nt = predicted_list.size();

  // before transformation
  int no = original_list.size();

  // number of true corners among the detected corners
  int nr = 0;

  for (const auto &original : original_list) {
    std::size_t found_index;
    cv::Point matched =
        FindMatchedPoint(original, predicted_list, found_index, th);
    if (matched.x != -1 && matched.y != -1) {
      ++nr;
      predicted_list.erase(std::begin(predicted_list) + found_index);
#ifdef VERBOSE
      std::cout << "Original Point: (" << original
                << ") matches with Found Point: (" << matched << ")"
                << std::endl;
#endif
    } else {
#ifdef VERBOSE
      std::cout << "Original Point : (" << original << ") has no match"
                << std::endl;
#endif
    }
  }

  metrics.precision = get_precision(nt, nr);
  metrics.recall = get_recall(nr, ng);
  metrics.apr = get_apr(metrics.precision, metrics.recall);
  metrics.fscore = get_fscore(metrics.precision, metrics.recall);
  metrics.ar = get_ar(nr, nt, no);

  return metrics;
}

cv::Point GeneralMetrics::FindMatchedPoint(cv::Point original_point,
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
