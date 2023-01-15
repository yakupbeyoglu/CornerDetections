#ifndef Types_h
#define Types_h
#include <vector>
#include <map>
#include <Gorgon/Geometry.h>
#include <Gorgon/Geometry/Point.h>
#include <Gorgon/Geometry/Line.h>
#include <Gorgon/Geometry/PointList.h>

namespace Types {
using Point = Gorgon::Geometry::Point;
using Pointf = Gorgon::Geometry::Pointf;
using PointList = Gorgon::Geometry::PointList<Point>;
using PointfList = Gorgon::Geometry::PointList<Pointf>;
using PolygonMap = std::map<int, PointList>;


enum class Direction {
    SAME = 0, // No change
    N = 1, // north
    S = 2, // south
    W = 3, // west
    E = 4, // east
    NW = 5, // north west
    NE = 6, // north east
    SW = 7, // south west
    SE = 8 // south east
};

}

#endif

