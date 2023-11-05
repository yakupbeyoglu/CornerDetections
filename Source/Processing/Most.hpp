#ifndef MOST_H_
#define MOST_H_
#include <algorithm>
#include <vector>

#include "CornerDetectors.h"
#include "Common.h"
namespace CornerDetections {
class Most: public CornerDetectors<Most, Types::CvPointList> {
public:
    struct CandidatePoint{
        double curvature_value;
        std::size_t index;
    }
    using CandidateList = std::vector<CandidatePoint>;
    Most(const std::size_t &l =4, const double &th = 0.8):l(l),th(th){
        assert(th  > 0);
    }

    ~Most(){}

    Most(const Most &) = delete;
    Most &operator = (const Most &) = delete;

    Most(Most &&) =  delete;
    Most &operator = (Most &&) = delete;

    Types::CvPointList CornerDetection(const Types::CvPointList &contour) {
        Types::CvPointList corners;

    }


private:

    // Return list of the candidates
    CandidateList GetCandidates(const Types::CvPointList &contour) {
        CandidateList candidates;
        // Find curvatures
        for(int i=1; i < contour.size() - 1; ++i) {
            // Formulate (7) to receive t value to find subpixels
            std::size_t t = GetT(contour, i);

            // Cycle indexes if out of bounds
            int bw_index = (i - t + contour.size()) % contour.size();
            int bw_index_1 = (i - t - 1 + contour.size()) % contour.size();
            int fw_index = (i + t + contour.size()) % contour.size();
            int fw_index_1 = (i + t + 1 + contour.size()) % contour.size();


            // Forward point for subpixelized triangle
            const cv::Point &pi = contour[i];
            const cv::Point &p_B = contour[bw_index];
            const cv::Point &p_f = contour[fw_index];

            double arc_len_1 = GetArcLength(contour, i, fw_index);
            double arc_len_2 = GetArcLength(contour, i, fw_index_1) ;
            double lf = (l - arc_len_1) / ((arc_len_2 - l) == 0 ? 1 : (arc_len_2 - l));

            arc_len_1 = arcLength(contour, bw_index, index);
            arc_len_2 = arcLength(contour, bw_index_1, index);
            double lb = (l - arc_len_1) / ((arc_len_2 - l) == 0 ? 1 : (arc_len_2 - l));

            int division = (1+lf) == 0 ? 1 : 1 + lf;
            cv::Point forward_point = {
                std::round(std::abs((contour[fw_index].x + lf * contour[fw_index_1].x) / division)),
                std::round(std::abs((contour[fw_index].y + lf * contour[fw_index_1].y) / division))
            }

            division = (1 + lb) == 0 ? 1 : 1 + lb;
            cv::Point backward_point = {
                std::round(std::abs((contour[bw_index].x + lb * contour[bw_index_1].x) / division)),
                std::round(std::abs((contour[fw_index].y + lb * contour[fw_index_1].y) / division))
            }

            cv::Point current_point = contour[index];
            double curvature_value = DiscreteCurvatureComputation(current_point, backward_point, forward_point);
            candidates.emplace_back({curvature_value, index});
        }


    }

    /*
     *  Get Arc Length, Formula(6)
     */
    float GetArcLength(const Types::CvPointList &contour, const std::size_t &start, const std::size_t  &end)  {
        float length = 0.0f;
        for(std::size_t i = start + 1; i <= end; ++i) {
            length += Common::EucledianDistance(contour[i -1], contour[i]);
        }

        return length;
    }

    /*
     * Get T to obant subpixel position of pi+f or pi-b
     *  Formula (7)
     */
    std::size_t GetT(const Types::CvPointList &contour, std::size_t current_index){
        std::size_t t = current_index + 1;
        for(; t < contour.size(); ++t) {
            if(GetArcLength(contour, current_index, t) <= l &&
                GetArcLength(contour, current_index, t+1) > l)
                break;
        }
        return t - current_index;
    }

    double DiscreteCurvatureComputation(const cv::Point &pi, const cv::Point &pb, const cv::Point &pf) {
        cv::Point pici = pi - ((pb + pf)/2);
        return cv::norm(pici);
    }

    CandidateList NonMaximumSuppression(const CandidateList& candidates, double threshold) {
        CandidateList result;

        if (candidates.size() < 3) {
            // If there are fewer than 3 points, non-maximum suppression is not meaningful
            return candidates;
        }

        // Handle the first point separately
        if (candidates.front().curvature_value >= threshold &&candidates.front().curvature_value > candidates[1].curvature_value) {
            result.push_back(candidates.front());
        }

        // Iterate through the middle points
        for (size_t i = 1; i < candidates.size() - 1; ++i) {
            // Check if the current curvature value is greater than both neighbors
            if (candidates[i].curvature_value >= threshold && candidates[i].curvature_value > candidates[i - 1].curvature_value &&
                candidates[i].curvature_value > candidates[i + 1].curvature_value) {
                result.push_back(candidates[i]);
            }
        }

        // Handle the last point separately
        if (candidates.back().curvature_value >= threshold && candidates.back().curvature_value > candidates[candidates.size() - 2].curvature_value && candidates.back().curvature_value > threshold) {
            result.push_back(candidates.back());
        } else if (candidates.back().curvature_value > candidates[candidates.size() - 2].curvature_value) {
            // Correct the condition to check if the last point is a local maximum
            result.push_back(candidates.back());
        }

        return result;
    }

    std::size_t l;
    double th;

};


#endif
