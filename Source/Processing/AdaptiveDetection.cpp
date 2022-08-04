#include "AdaptiveDetection.h"
namespace CornerDetections {
    AdaptiveDetection::AdaptiveDetection(std::vector<Types::PointList> list) {
            pointlist = std::move(list);
    }
    
    void AdaptiveDetection::PreProcess() {
        // TO DO : Pre process 
    }
    
    Types::PointList AdaptiveDetection::FindCandidatePoints(const Types::PointList &list) {
        Types::PointList candidatepoints;
        
        
        return candidatepoints;
    }
    
    int AdaptiveDetection::Clamp(int size, int current, int k) const {
        if(current - k < 0)
            current = size - k + current;
        else if (current + k > size)
            current =  current - size + k;
        return current;
    }
    
    float AdaptiveDetection::GetDistance(const Types::Point &p1, 
                                         const Types::Point &p2) const { 
                                             
            float xdiff = p1.X - p2.X;
            float ydiff = p1.Y - p2.Y;
            return std::sqrt(std::pow(xdiff,2) + std::pow(ydiff, 2));
    }

}
