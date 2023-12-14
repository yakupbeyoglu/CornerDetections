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

class PerformanceMetrics {
 public:
  std::vector<std::tuple<std::string, double>> void CalculateMetrics(
      Types::PolygonMap original_map, Types::PolygonMap predicted_map) = 0;
  virtual void ExportMetricsToCSV() = 0;
};

class GeneralMetrics : public PerformanceMetrics {
 public:
  virtual std::vector<std::tuple<std::string, double>> CalculateMetrics(
      Types::PolygonMap original_map,

      Types::PolygonMap predicted_map) override;
  virtual void ExportMetricsToCSV() override;

 private:
  void ResetMetrics() {
    avg_precision = 0;
    avg_recall = 0;
    avg_apr = 0;
    avg_fscore = 0;
    avg_ar = 0;
  }

  cv::Point FindMatchedPoints(cv::Point original_point,
                              const Types::CvPointList,
                              std::size_t &found_index, double th = 1.5);

  double avg_precision;
  double avg_recall;
  double avg_apr;
  double avg_fscore;
  double avg_ar;
};
}  // namespace Validator

#endif
