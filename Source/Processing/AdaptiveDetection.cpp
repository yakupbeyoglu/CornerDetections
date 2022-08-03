#include "AdaptiveDetection.h"
namespace CornerDetections {
    AdaptiveDetection::AdaptiveDetection(std::vector<Types::PointList> list) {
            pointlist = std::move(list);
    }
    
    void AdaptiveDetection::PreProcess() {
        // TO DO : Pre process 
    }
    
    Types::PointList FindCandidatePoints(const Types::PointList &list) {
        Types::PointList candidatepoints;
        
        return candidatepoints;
    }
    
    
    
}
