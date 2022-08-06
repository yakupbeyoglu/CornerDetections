#include "AdaptiveDetection.h"
namespace CornerDetections {
    AdaptiveDetection::AdaptiveDetection(std::vector<Types::PointList> list) {
            pointlist = std::move(list);
    }
    
    void AdaptiveDetection::PreProcess() {
        // TO DO : Pre process 
    }
    
    Types::PointList AdaptiveDetection::FindCandidatePoints(const Types::PointList &list, const int &step) {
        Types::PointList candidatepoints;
        size_t listsize = list.GetSize();
        std::vector<float> sharpness;
        
        for(size_t point = 0; point < listsize; point++) {
            int nextindex = Clamp(listsize, point, step);
            int previousindex = Clamp(listsize, point, -1 * step);
            sharpness.push_back(GetSharpness(list.Get(point), list.Get(nextindex), list.Get(previousindex)));
        }
        
        float threshold = GetAdaptiveTh(sharpness);
        
        for(size_t point = 0; point < sharpness.size(); point++) {
                if(sharpness[point] >= threshold)
                    candidatepoints.Push(list.Get(point));
        }
        
        return candidatepoints;
    }
    
    int AdaptiveDetection::Clamp(int size, int current, int k)const {
        if(current - k < 0)
            current = size - k + current;
        else if (current + k > size)
            current =  current - size + k;
        return current;
    }
    
    float AdaptiveDetection::GetSharpness(const Types::Point &p1, const Types::Point &p2, 
                                          const Types::Point &p3)const {

        return 1 - ( GetDistance(p2, p3) / (GetDistance(p1,p3) + GetDistance(p1, p2)));
    }
    
    float AdaptiveDetection::GetAdaptiveTh(const std::vector<float> &sharpness)const {
        // filter nonzero elements 
        std::vector<float> nonzero;
        std::copy_if(sharpness.begin(), sharpness.end(), std::back_inserter(nonzero), [](float sharp){
            return sharp != 0;
        }
        );
        
        // return mean of nonzero
        return std::accumulate(nonzero.begin(), nonzero.end(), 0) / nonzero.size();
    }
    
    
    float AdaptiveDetection::GetDistance(const Types::Point &p1, 
                                         const Types::Point &p2)const { 
                                             
            float xdiff = p1.X - p2.X;
            float ydiff = p1.Y - p2.Y;
            return std::sqrt(std::pow(xdiff,2) + std::pow(ydiff, 2));
    }

}
