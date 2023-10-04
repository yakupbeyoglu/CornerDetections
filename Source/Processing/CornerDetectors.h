#ifndef CornerDetectors_h
#define CornerDetectors_h
#include "../Types.h"

namespace CornerDetections {

template <typename T, typename PointList>
class CornerDetectors {
 public:
  void PreProcess() { static_cast<T *>(this)->PreProcessing(); }

  PointList DetectCorners(const PointList &list) {
    return static_cast<T *>(this)->CornerDetection(list);
  }
};
}  // namespace CornerDetections
#endif
