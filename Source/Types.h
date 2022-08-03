#ifndef Types_h
#define Types_h
#include <vector>
#include <map>
#include <Gorgon/Geometry.h>
#include <Gorgon/Geometry/Point.h>
#include <Gorgon/Geometry/Line.h>
#include <Gorgon/Geometry/PointList.h>

namespace Types{
        using Point = Gorgon::Geometry::Point;
        using Pointf = Gorgon::Geometry::Pointf;
        using PointList = Gorgon::Geometry::PointList<Point>;
        using PointfList = Gorgon::Geometry::PointList<Pointf>;
        using PolygonMap = std::map<int, PointList>;
}

#endif
