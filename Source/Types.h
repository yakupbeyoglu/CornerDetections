#ifndef Types_h
#define Types_h
#include <map>
#include <opencv2/opencv.hpp>
#include <vector>

namespace Types {

using CvPointList = std::vector<cv::Point>;
using CvContours = std::vector<CvPointList>;
using PolygonMap = std::map<int, CvPointList>;

// Type trait to check if T is either cv::Point or Yakup::Point
template <typename T_>
struct IsSupportedPoint : std::disjunction<std::is_same<T_, cv::Point>> {};

template <class T_>
struct PointMap {
  static_assert(IsSupportedPoint<T_>::value, "T_ must be cv::Point");
  std::size_t index;
  double curvature_value;
  T_ point;
  bool added = false;
};

template <class T_>
using PointList = std::vector<T_>;

template <class T_>
using PointMapList = std::vector<PointMap<T_>>;

enum class Direction {
  SAME = 0,  // No change
  N = 1,     // north
  S = 2,     // south
  W = 3,     // west
  E = 4,     // east
  NW = 5,    // north west
  NE = 6,    // north east
  SW = 7,    // south west
  SE = 8     // south east
};

enum class BCalculation { MIN = 0, MAX = 1 };

}  // namespace Types

#endif
