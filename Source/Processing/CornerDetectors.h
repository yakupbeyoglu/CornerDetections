#ifndef CornerDetectors_h
#define CornerDetectors_h
#include "../Types.h"

namespace CornerDetections {

template <typename T, class PointList>
class CornerDetectors {
  void PreProcess() { static_cast<T *>(this)->PreProcessing(); }

  PointList DetectCorners(const PointList &list) {
    return static_cast<T *>(this)->CornerDetection();
  }
};
}  // namespace CornerDetections
#endif
