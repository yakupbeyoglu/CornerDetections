#ifndef COMMON_H_
#define COMMON_H_
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include "../Types.h"

namespace CornerDetections::Common {

/*
 *  Basically doing Canny Edge Detection then change it to PointList
 *
 */
std::tuple<Types::CvPointList, cv::Mat> GetContour(cv::Mat &image, std::size_t threshold = 0) {
    // Convert image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(image, grayscale, cv::COLOR_RGB2GRAY);
    // Do canny Edge Detection
    cv::Mat edges;
    cv::Canny(grayscale, edges, 128, 255);

    // Collect coordinatest to point list
    Types::CvPointList list;
    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x < image.cols; x++) {
            if(image.at<uchar>(y, x) > 0) {
                list.push_back(cv::Point(x, y));
            }
        }
    }
    return {list, edges};
}

std::string StreamPointList(const Types::CvPointList &list) {
    std::size_t size = list.size();
    std::stringstream stream;
    for(int i= 0; i< size; ++i) {
        cv::Point point = list[i];
        stream << "{" << point.x << "," << point.y << "}";
        if(i + 1 != size)
            stream << ",";
    }
    return stream.str();
}

cv::Mat DrawPoints(const cv::Size &size, const Types::CvPointList &list) {
    cv::Mat output(size, CV_8UC1);
    cv::polylines(output, list, false, cv::Scalar(0, 255, 0), 2);
    return output;
}

/*
 *  Draw circles to given coordinates on the image
 */
cv::Mat PointOnImage(const cv::Mat &image, const Types::CvPointList &list, std::size_t radius = 10, std::size_t thickness = 2) {
    assert(!image.empty());
    cv::Mat output = image;

    cv::Scalar color (255, 0, 0);
    for(auto &point : list) {
        cv::circle(output, point, radius, color, thickness);
    }

    return output;
}

Types::CvPointList FindTJunctions(const Types::CvPointList &list)
{
    auto isequal= [](cv::Point p1, cv::Point p2) {
        return p1.x == p2.x && p1.y == p2.y;
    };

    // sure that its ordered
    Types::CvPointList tjunctions;

    Types::CvPointList endpoints;

    // create start and end point for each
    for (std::size_t point = 0; point < list.size() - 1; ++point) {
        endpoints.push_back(list[point]);
        endpoints.push_back(list[point + 1]);
    }

    for(const auto &point:endpoints) {
        std::size_t count = 0;
        for(std::size_t i = 0; i < list.size() - 1; ++i) {
            if(isequal(list[i], point) || isequal(list[i+1], point))
                ++count;
        }

        if(count > 1) {
            int segmentcounter = 0;
            // check if point cross at least three line segment
            for(std::size_t i = 0; i < list.size() - 1; ++i) {
                if(isequal(list[i], point) || isequal(list[i+1], point))
                {
                    if(++segmentcounter > 2) {
                        if(std::find(std::begin(tjunctions), std::end(tjunctions), point) == std::end(tjunctions))
                            tjunctions.push_back(point);
                        break;
                    }
                }
            }
        }
    }
    return tjunctions;
}

}

#endif
