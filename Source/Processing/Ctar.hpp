#ifndef CTAR_H_
#define CTAR_H_
#include <vector>
#include <algorithm>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "Common.h"
#include "CornerDetectors.h"

namespace CornerDetections {

class Ctar : public CornerDetectors<Ctar, Types::CvPointList> {
 public:

  Ctar(std::size_t k = 7, float threshold = 0.989, bool t_junction = false)
      : k(k), threshold(threshold), t_junction(t_junction) {
    assert(threshold > 0);
  }

  ~Ctar() {}

  Types::CvPointList CornerDetection(const Types::CvPointList &list, bool use_tjunction = false) {
    Types::CvPointList response;


    Types::CvPointList smoothed_list = CornerDetections::Common::GausianSmooth(list, 3);

    // Corner map incldue index of the corners for iterating indexes
    std::vector<Types::PointMap<cv::Point>> corner_map;
    for (std::size_t index = k; index < list.size(); ++index) {
      auto [is_curve, value] = IsCurve(index, smoothed_list);
      if (is_curve) corner_map.push_back({index, value, smoothed_list[index]});
    }

    Types::PointMapList<cv::Point> non_max;

    // local minima
    for (std::size_t i = 0; i < corner_map.size(); ++i) {
          if (corner_map[i].curvature_value <= corner_map[i - 1].curvature_value &&
                corner_map[i].curvature_value <= corner_map[i + 1].curvature_value) {
                non_max.push_back(corner_map[i]);
            }
    }

    Types::PointMapList<cv::Point> tjunctions;
    Types::PointList<cv::Point> merged_points;
    if(use_tjunction) {
       // Find Tjunctions
       tjunctions =
          Common::FindTJunctions<Types::PointMapList<cv::Point>>(smoothed_list);
      // Filter tjunctions
      Types::PointMapList<cv::Point> filter_tjunction;
      std::for_each(
          std::begin(tjunctions), std::end(tjunctions), [&](auto &tjunction) {
            bool is_in_window = false;

            for (const auto &corner : corner_map)
              is_in_window |=
                  Common::IsWithinWindow(tjunction.point, corner.point, 5);

            if (!is_in_window && !tjunction.added) {
              tjunction.added = true;
              filter_tjunction.emplace_back(std::move(tjunction));
            }
          });


          merged_points = Common::MergePointMaps(non_max, filter_tjunction);
      // todo imPLEMENT LOCAL MINIMA
    }

    return merged_points;
  }

 private:
  std::tuple<bool, float> IsCurve(std::size_t index, const Types::CvPointList &contour) {
    if (index >= contour.size())
      throw std::out_of_range("index is more than contour size !");
    auto distances = GetDistances(index, contour);
    auto ratio = GetRatio(distances);
    return std::make_tuple(ratio <= threshold, ratio);
  }

  float GetRatio(const std::tuple<float, float, float> &distances) const {
    // c++ 17 automatic solve ties
    auto [d1, d2, d3] = distances;
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

  Types::CvContours contours;
  int k;
  float threshold;
  bool t_junction;
};

}  // namespace CornerDetections
#endif
