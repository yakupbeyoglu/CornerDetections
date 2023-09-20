#include <iostream>
#include <vector>

#include "Processing/Common.h"
#include "Processing/Ctar.hpp"
cv::RNG rng(12345);

void show(const Types::CvPointList &list) {
  for (auto &point : list) {
    std::cout << "{" << point.x << "," << point.y << "} ";
  }

  std::cout << std::endl;
}

int main() {
  std::vector<cv::Point> tjunctionlist = {{0, 0},  {0, 3}, {4, 6}, {5, 5},
                                          {9, 10}, {5, 5}, {0, 3}};
  auto t_junctions =
      CornerDetections::Common::FindTJunctions<Types::CvPointList>(
          tjunctionlist);
  std::cout << CornerDetections::Common::StreamPointList(t_junctions)
            << std::endl;
  cv::Mat image = cv::imread(
      "/media/yakup/Samsung980/remote/CornerDetections/Bin/grayscale.png");
  assert(!image.empty());

  auto contour = CornerDetections::Common::GetContour(image, 0);
  Types::CvContours contours;
  cv::Mat detected;
  std::tie(contours, detected) = contour;
  cv::Mat drawing = cv::Mat::zeros(image.size() * 4, CV_8UC3);
  int index = 0;
  for (auto &contour : contours) {
    cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
                                  rng.uniform(0, 256));
    cv::drawContours(drawing, contours, (int)index, color, 1, cv::LINE_8);
    auto t_junctions =
        CornerDetections::Common::FindTJunctions<Types::CvPointList>(contour);
    std::cout << "TJUNCTION"
              << CornerDetections::Common::StreamPointList(t_junctions)
              << std::endl;
    cv::Mat drawing2 = cv::Mat::zeros(image.size() * 4, CV_8UC3);
    contours[0].push_back({24, 23});
    cv::polylines(drawing2, contours, true, cv::Scalar(0, 255, 0), 2);
    cv::imshow("MAT", drawing2);
    cv::waitKey(0);
    std::cout << "contours  " << index++
              << CornerDetections::Common::StreamPointList(contour)
              << std::endl;
  }
  // auto pointed = CornerDetections::Common::DrawPoints(cv::Size(image.rows,
  // image.cols), list); CornerDetections::Ctar c(image);

  /*
  std::cout << "Hello World !" << std::endl;
  std::vector<Types::PointList> list = {};
  list.push_back({{3,5}});
  CornerDetections::AdaptiveDetection adaptive(std::move(list));
  std::cout<<adaptive.Clamp(5,3,11)<<std::endl;
  std::cout<<adaptive.Clamp(5,3,2)<<std::endl;
  std::cout<<adaptive.Clamp(5,1,-2)<<std::endl;
  std::cout<<adaptive.Clamp(5,3,-1)<<std::endl;
  std::cout<<adaptive.Clamp(5,1,4)<<std::endl;
  std::cout<<adaptive.Clamp(5,2,-3)<<std::endl;
  std::cout<<adaptive.Clamp(5,1,-8)<<std::endl;
  std::vector<float> values = {0,1.2,5,2.0,0,0,3};
  std::vector<Gorgon::Geometry::Point> points =  {{0,0}, {1,0}, {2,1}, {3,0},
  {4,0}, {5,0}};

  Types::PointList list2(std::move(points));
  auto a = adaptive.FindCandidatePoints(list2, 5);
  Types::PointList testlist = {
      {0, 1}, {1, 1}, {2, 1}, {3, 0}, {4, 0}, {5, 0}, {6, 1}, {7, 1}, {8, 1}
  };
  std::vector<int> expected = {3, 4, 5};
  std::vector<int> nlist = adaptive.FindSalientPoints(testlist, 0);*/
  return 0;
}
