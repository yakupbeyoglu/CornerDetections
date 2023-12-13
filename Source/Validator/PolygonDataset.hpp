#ifndef POLYGON_DATA_H_
#define POLYGON_DATA_H_
#include "../Types.h"
#include "Parser.hpp"

/**
 * @file PolygonDataset.hpp
 * @brief PolygonDataset
 * PolygonDataset is basically parsing given file dataset path using the
 * specified Parser The parser is basically parse polygon_id,x,y value to
 * map<polygon_id,vector<Point>>
 *
 * @author Yakup Beyoglu
 * @date December 13, 2023
 * @see https://github.com/yakupbeyoglu
 */

namespace Types {
template <class Parser_ = Validator::CsvToPolyList<cv::Point>>
class PolygonDataset {
 public:
  PolygonDataset(const std::string &file_path) : file_path(file_path) {
    maps = parser.Parse(file_path);
  }

  PolygonDataset(const PolygonDataset &other)
      : file_path(other.file_path), maps(other.maps) {}

  PolygonDataset &operator=(PolygonDataset other) {
    swap(*this, other);
    return *this;
  }

  // move constructor
  PolygonDataset(PolygonDataset &&other) {
    file_path = std::move(other.file_path);
    other.file_path.clear();

    maps = std::move(other.maps);
    other.maps.clear();
  }

  PolygonDataset &operator=(PolygonDataset &&other) {
    file_path = std::move(other.file_path);
    other.file_path.clear();

    maps = std::move(other.maps);
    other.maps.clear();
    return *this;
  }

  const Types::PolygonMap &GetPolygons() const { return maps; }

  friend std::ostream &operator<<(std::ostream &os, PolygonDataset &dataset) {
    for (auto it = dataset.maps.begin(); it != dataset.maps.end(); ++it) {
      std::stringstream line;
      line << "Polygon-" << it->first << "\t";
      for (auto point = it->second.begin(); point != it->second.end();
           ++point) {
        if (point != it->second.begin() && std::next(point) != it->second.end())
          line << ",";
        line << "{" << point->x << "," << point->y << "}";
      }

      os << line.str() << std::endl;
    }
    return os;
  }

  friend void swap(PolygonDataset &first, PolygonDataset &second) {
    using std::swap;
    std::swap(first.file_path, second.file_path);
    std::swap(first.maps, second.maps);
  }

 private:
  std::string file_path;
  Types::PolygonMap maps;
  Validator::Parser<Types::PolygonMap, Parser_> parser;
};

}  // namespace Types

#endif
