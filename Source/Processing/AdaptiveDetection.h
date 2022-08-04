#ifndef AdaptiveDetection_h
#define AdaptiveDetection_h
#include "CornerDetectors.h"
#include <vector>
namespace CornerDetections{
    
    /*
     *  This class aims to detect corner with
     *  Adaptive algortihm for corner detecting based on 
     *  degree of sharpness of the contour
     *  You can find reference at below :
     *  Xiao, J., Xiang, Z., Wang, B., & Liu, Y. (2011). Adaptive algorithm for corner detecting based on the degree of sharpness of the 
     *  contour. Optical Engineering, 50(4), 047008.
     */
    class AdaptiveDetection: public CornerDetectors {
    public:
        AdaptiveDetection(std::vector<Types::PointList> list);
        
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
        /*
         *  Find the candidate points with the sharpness degreee 
         */
        Types::PointList FindCandidatePoints(const Types::PointList &list);
        
        /*
         *  Clamp index to cycle Pi + k and Pi - k
         */
        int Clamp(int size, int current, int k) const;
        
        /*
         *  Get Distnace Between two points
         */
        virtual float GetDistance(const Types::Point &p1, const Types::Point &p2) const override;
 };
    
}

#endif
