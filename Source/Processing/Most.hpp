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
        cv::Point p;
    };
    using CandidateList = std::vector<CandidatePoint>;
    Most(const std::size_t &l =5, const double &th = 0.8):l(l),th(th){
        assert(th  > 0);
    }

    ~Most(){}

    Most(const Most &) = delete;
    Most &operator = (const Most &) = delete;

    Most(Most &&) =  delete;
    Most &operator = (Most &&) = delete;

    Types::CvPointList CornerDetection(const Types::CvPointList &contour) {
        CandidateList list = GetCandidates(contour);
        return NonMaximumSuppression(std::move(list), th);
    }


private:

    // Return list of the candidates
    CandidateList GetCandidates(const Types::CvPointList &contour) {
        CandidateList candidates;
        // Find curvatures
        for(std::size_t i=0; i < contour.size(); ++i) {
            // Formulate (7) to receive t value to find subpixels
            std::size_t t = GetT(contour, i);

            // Cycle indexes if out of bounds
            std::size_t bw_index = (i - t + contour.size()) % contour.size();
            std::size_t bw_index_1 = (i - t - 1 + contour.size()) % contour.size();
            std::size_t fw_index = (i + t + contour.size()) % contour.size();
            std::size_t fw_index_1 = (i + t + 1 + contour.size()) % contour.size();



            double arc_len_1 = GetArcLength(contour, i, fw_index);
            double arc_len_2 = GetArcLength(contour, i, fw_index_1) ;
            double lf = (l - arc_len_1) / ((arc_len_2 - l) == 0 ? 1 : (arc_len_2 - l));

            arc_len_1 = GetArcLength(contour, bw_index, i);
            arc_len_2 = GetArcLength(contour, bw_index_1, i);
            double lb = (l - arc_len_1) / ((arc_len_2 - l) == 0 ? 1 : (arc_len_2 - l));

            int division = (1+lf) == 0 ? 1 : 1 + lf;
            cv::Point forward_point = {
                static_cast<int>(std::round(((contour[fw_index].x + lf * contour[fw_index_1].x) / division))),
                static_cast<int>(std::round(((contour[fw_index].y + lf * contour[fw_index_1].y) / division)))
            };

            division = (1 + lb) == 0 ? 1 : 1 + lb;
            cv::Point backward_point = {
                static_cast<int>(std::round(((contour[bw_index].x + lb * contour[bw_index_1].x) / division))),
                static_cast<int>(std::round(((contour[fw_index].y + lb * contour[fw_index_1].y) / division)))
            };

            cv::Point current_point = contour[i];
            double curvature_value = DiscreteCurvatureComputation(current_point, backward_point, forward_point);
            candidates.push_back({curvature_value, i, current_point});
        }

        return candidates;
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

    Types::CvPointList NonMaximumSuppression(const CandidateList& candidates, double threshold, int n = 3) {
        Types::CvPointList result;


    for (std::size_t i = 0; i < candidates.size(); ++i) {
        bool isLocalMaxima = true;

        // Check against neighbors
        for (std::size_t j = i - n; j <= i + n; ++j) {
            if (j != i && j < candidates.size() && candidates[i].curvature_value <= candidates[j].curvature_value && candidates[i].curvature_value > threshold) {
                isLocalMaxima = false;
                break;
            }
        }

        if (isLocalMaxima) {
            result.push_back(candidates[i].p);
        }
    }



        return result;
    }

    std::size_t l;
    double th;

};
}


#endif
