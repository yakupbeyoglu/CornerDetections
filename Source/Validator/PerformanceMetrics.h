#ifndef PERFORMANCE_METRICS_H_
#define PERFORMANCE_METRICS_H_
#include <tuple>

#include "../Types.h"

/**
 * @file PerformanceMetrics.h
 * @brief PerformanceMetrics
 * PerformanceMetrics is used to calculate performance of the algorithms
 * You can us calculate function to receive whole metrics and ExportMetricsToCSV
 * @author Yakup Beyoglu
 * @date December 13, 2023
 * @see https://github.com/yakupbeyoglu
 */
namespace Validator {
struct Metrics {
  double precision;
  double recall;
  double apr;
  double fscore;
  double ar;

  Metrics &operator/=(const int &divisor) {
    if (divisor == 0)
      throw std::runtime_error("Divisor can not be less than 0!");
    precision /= divisor;
    recall /= divisor;
    apr /= divisor;
    fscore /= divisor;
    ar /= divisor;
    return *this;
  }

  Metrics &operator+=(const Metrics &other) {
    precision += other.precision;
    recall += other.recall;
    apr += other.apr;
    fscore += other.fscore;
    ar += other.ar;
    return *this;
  }
};

template <class P_ = cv::Point>

class PerformanceMetrics {
 public:
  virtual Metrics CalculateMetrics(Types::PolygonMap original_map,
                                   Types::PolygonMap predicted_map) = 0;
  virtual void ExportMetricsToCSV() = 0;
};

class GeneralMetrics : public PerformanceMetrics {
 public:
  virtual Metrics CalculateMetrics(Types::PolygonMap original_map,
                                   Types::PolygonMap predicted_map) override;
  virtual void ExportMetricsToCSV() override;

 private:
  void ResetMetrics() {
    avg_metrics.precision = 0;
    avg_metrics.recall = 0;
    avg_metrics.apr = 0;
    avg_metrics.fscore = 0;
    avg_metrics.ar = 0;
    avg_metrics.nr = 0;
    avg_metrics.
  }

  Metrics CalculateMetric(const Types::CvPointList original_list,
                          Types::CvPointList predicted_list, double th = 1.5);
  cv::Point FindMatchedPoint(cv::Point original_point, const Types::CvPointList,
                             std::size_t &found_index, double th = 1.5);

  Metrics avg_metrics;
};
}  // namespace Validator

#endif
