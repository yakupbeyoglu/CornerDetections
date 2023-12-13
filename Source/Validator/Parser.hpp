#ifndef PARSER_H_
#define PARSER_H_
#include <filesystem>
#include <fstream>
#include <string>

#include "../Types.h"
namespace Validator {
template <class T_, class P_>
class Parser {
 public:
  T_ Parse(const std::string &file_path) {
    if (!std::filesystem::exists(file_path))
      throw std::runtime_error("Given file path does not exists!");
    return static_cast<P_ *>(this)->Parse();
  }
};

template <class PointType = cv::Point>
class CsvToPolyList
    : public Parser<Types::PolygonMap, CsvToPolyList<PointType>> {
 public:
  Types::PolygonMap Parse(const std::string &file_path) {
    Types::PolygonMap map;
    std::ifstream file(file_path);
    if (!file.is_open()) throw std::runtime_error("File can not open!");

    std::string line;
    while (std::getline(file, line)) {
      std::string value;
      std::stringstream stream_line(line);
      int i = 0;
      int id;
      PointType point;

      while (std::getline(stream_line, value, ',')) {
        int int_value = std::stoi(value);
        switch (i) {
          case 0:
            if (map.find(i) == map.end())
              map.insert({int_value, std::vector<PointType>()});
            id = int_value;
            break;
          case 1:
            point.X = int_value;
            break;
          case 2:
            point.Y = int_value;
            break;
          default:
            throw std::runtime_error("Parsing Error !");
        }
        ++i;
      }

      map[id].emplace_back(std::move(point));
    }
    return map;
  }
};

}  // namespace Validator

#endif
