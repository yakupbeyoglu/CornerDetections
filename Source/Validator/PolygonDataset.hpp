#ifndef POLYGON_DATA_H_
#define POLYGON_DATA_H_
#include "../Types.h"
#include "Parser.hpp"

namespace Types {
template <class P_ = Validator::CsvToPolyList<cv::Point>>
class PolygonDataSet {
 public:
  PolygonDataSet(const std::string &file_path) : file_path(file_path) {
    maps = std::move(parser.Parse(file_path));
  }

  PolygonDataSet(const PolygonDataSet &other)
      : file_path(other.file_path), maps(other.maps) {}

  PolygonDataSet &operator=(PolygonDataSet other) {
    swap(*this, other);
    return *this;
  }

  // move constructor
  PolygonDataSet(PolygonDataSet &&other) {
    file_path = std::move(other.file_path);
    other.file_path.clear();

    maps = std::move(other.maps);
    other.maps.clear();
  }

  PolygonDataSet &operator=(PolygonDataSet &&other) {
    file_path = std::move(other.file_path);
    other.file_path.clear();

    maps = std::move(other.maps);
    other.maps.clear();
    return *this;
  }

  const Types::PolygonMap &GetPolygons() const { return maps; }

  friend void swap(PolygonDataSet &first, PolygonDataSet &second) {
    using std::swap;
    std::swap(first.file_path, second.file_path);
    std::swap(first.maps, second.maps);
  }

 private:
  std::string file_path;
  Types::PolygonMap maps;
  Validator::Parser<Types::PolygonMap, P_> parser;
};

}  // namespace Types

#endif
