#ifndef CornerDetectors_h
#define CornerDetectors_h
#include "../Types.h"

namespace CornerDetections {
class CornerDetectors {
public:
    virtual ~CornerDetectors() {}

    /*
     *  Pre process will render the polygon list and prepeare to detect corners
    */
    virtual void PreProcess() = 0;

    /*
     *  Detect Corners from given list and return coordinate list
    */
    virtual Types::PointList DetectCorners(const Types::PointList &list) = 0;
};

}
#endif
