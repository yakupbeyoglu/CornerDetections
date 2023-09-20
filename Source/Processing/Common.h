#ifndef COMMON_H_
#define COMMON_H_
#include <algorithm>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>

#include "../Types.h"

namespace CornerDetections::Common {

/*
 *  Basically doing Canny Edge Detection then change it to PointList
 *
 */

std::tuple<Types::CvContours, cv::Mat> GetContour(cv::Mat &image,
                                                  std::size_t filter = 0,
                                                  std::size_t threshold = 0) {
  // Convert image to grayscale
  cv::Mat grayscale;
  cv::cvtColor(image, grayscale, cv::COLOR_RGB2GRAY);

  // Ctar Accept as 3,3 and sigma value = 3
  cv::Mat blured;
  cv::GaussianBlur(grayscale, blured, cv::Size(3, 3), 3);

  // Do canny Edge Detection
  cv::Mat edges;
  cv::Canny(blured, edges, 250, 255);

  // Collect coordinatest to point list
  Types::CvContours contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_NONE);
  // filter small polygons
  if (filter > 0) {
    std::remove_if(
        contours.begin(), contours.end(),
        [&](Types::CvPointList &list) { return list.size() < filter; });
  }
  return {contours, edges};
}

// Convert contours to point list
Types::CvPointList ContoursToList(Types::CvContours &contours) {
  Types::CvPointList list;
  for (auto &contour : contours) {
    for (auto &point : contour) {
      list.emplace_back(point);
    }
  }
  return list;
}

std::string StreamPointList(const Types::CvPointList &list) {
  std::size_t size = list.size();
  std::stringstream stream;
  for (int i = 0; i < size; ++i) {
    cv::Point point = list[i];
    stream << "{" << point.x << "," << point.y << "}";
    if (i + 1 != size) stream << ",";
  }
  return stream.str();
}

cv::Mat DrawPoints(const cv::Size &size, const Types::CvPointList &list) {
  cv::Mat output(size, CV_8UC1);
  cv::polylines(output, list, false, cv::Scalar(0, 255, 0), 2);
  return output;
}

/*
 *  Draw circles to given coordinates on the image
 */
cv::Mat PointOnImage(const cv::Mat &image, const Types::CvPointList &list,
                     std::size_t radius = 10, std::size_t thickness = 2) {
  assert(!image.empty());
  cv::Mat output = image;

  cv::Scalar color(255, 0, 0);
  for (auto &point : list) {
    cv::circle(output, point, radius, color, thickness);
  }

  return output;
}

/// TJunction function forward initialize
template <typename T, typename PList>
T FindTJunctions(const PList &list);
template <>
Types::CvPointList FindTJunctions(const Types::CvPointList &list) {
  auto isequal = [](cv::Point p1, cv::Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
  };

  // sure that its ordered
  Types::CvPointList tjunctions;

  Types::CvPointList endpoints;

  // create start and end point for each
  for (std::size_t point = 0; point < list.size() - 1; ++point) {
    endpoints.push_back(list[point]);
    endpoints.push_back(list[point + 1]);
  }

  for (const auto &point : endpoints) {
    std::size_t count = 0;
    for (std::size_t i = 0; i < list.size() - 1; ++i) {
      if (isequal(list[i], point) || isequal(list[i + 1], point)) ++count;
    }

    if (count > 1) {
      int segmentcounter = 0;
      // check if point cross at least three line segment
      for (std::size_t i = 0; i < list.size() - 1; ++i) {
        if (isequal(list[i], point) || isequal(list[i + 1], point)) {
          if (++segmentcounter > 2) {
            if (std::find(std::begin(tjunctions), std::end(tjunctions),
                          point) == std::end(tjunctions))
              tjunctions.push_back(point);
            break;
          }
        }
      }
    }
  }
  return tjunctions;
}

template <>
Types::PointMapList<cv::Point> FindTJunctions(const Types::CvPointList &list) {
  auto isequal = [](cv::Point p1, cv::Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
  };

  Types::PointMapList<cv::Point> tjunctions;
  // sure that its ordered

  Types::CvPointList endpoints;

  // create start and end point for each
  for (std::size_t point = 0; point < list.size() - 1; ++point) {
    endpoints.push_back(list[point]);
    endpoints.push_back(list[point + 1]);
  }

  for (std::size_t point_index = 0; point_index < endpoints.size();
       ++point_index) {
    const auto &point = endpoints[point_index];
    std::size_t count = 0;
    for (std::size_t i = 0; i < list.size() - 1; ++i) {
      if (isequal(list[i], point) || isequal(list[i + 1], point)) ++count;
    }

    auto find_condition = [&point](const Types::PointMap<cv::Point> &p) {
      return point ==
             p.point;  // Assuming p has a 'point' member of type cv::Point
    };

    if (count > 1) {
      int segmentcounter = 0;
      // check if point cross at least three line segment
      for (std::size_t i = 0; i < list.size() - 1; ++i) {
        if (isequal(list[i], point) || isequal(list[i + 1], point)) {
          if (++segmentcounter > 2) {
            if (std::find_if(std::begin(tjunctions), std::end(tjunctions),
                             find_condition) == std::end(tjunctions))
              tjunctions.push_back({point_index, point});
            break;
          }
        }
      }
    }
  }
  return tjunctions;
}

template <typename T>
Types::PointList<T> MergePointMaps(const Types::PointMapList<T> &list_1,
                                      const Types::PointMapList<T> &list_2) {
  Types::PointList<T> sorted_list;
  std::size_t i = 0;
  std::size_t j = 0;
  // Merge two list
  while (i < list_1.size() && j < list_2.size()) {
    if (list_1[i].index < list_2[j].index) {
      sorted_list.emplace_back(std::move(list_1[i].point));
      ++i;
    } else {
      sorted_list.emplace_back(std::move(list_2[j].point));
      ++j;
    }
  }

  // Append remaining elements

  auto append_remanings = [&sorted_list](const Types::PointMapList<T> &list,
                                         std::size_t &index) {
    for (; index < list.size(); ++index) {
      sorted_list.emplace_back(std::move(list[index].point));
    }
  };

  append_remanings(list_1, i);
  append_remanings(list_2, j);
  return sorted_list;
}

// Check data is in window or not
bool IsWithinWindow(const cv::Point &target, const cv::Point &center, const int &window_size) {
    int size = window_size / 2;
    return (target.x >= center.x - size && target.x <= center.x + size &&
        target.y >= center.y - size && target.y <= center.y + size);
}

}  // namespace CornerDetections::Common

#endif
