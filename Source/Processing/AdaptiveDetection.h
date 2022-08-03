#ifndef AdaptiveDetection_h
#define AdaptiveDetection_h
#include "CornerDetectors.h"
#include <vector>
namespace CornerDetections{
    class AdaptiveDetection: public CornerDetectors {
    public:
        AdaptiveDetection(const std::vector<Types::PointList> &list);
        
        /*
         *  Delete copy constructors 
        */
        AdaptiveDetection(const AdaptiveDetection &cpy) = delete;
        AdaptiveDetection &operator=(const AdaptiveDetection &cpy) = delete;
        
        /*
         *  Delete Move constructors 
         */ 
        AdaptiveDetection(const AdaptiveDetection &&target) = delete;
        AdaptiveDetection &operator=(const AdaptiveDetection &&target) = delete;
        
        ~AdaptiveDetection();
        virtual void PreProcess() override;
        virtual Types::PointList DetectCorners(const Types::PointList &list) override;
    private:
        std::vector<Types::PointList> pointlist;
 };
    
}

#endif
