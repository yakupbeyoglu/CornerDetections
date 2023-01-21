#include "AdaptiveDetection.h"
namespace CornerDetections {
CornerDetections::AdaptiveDetection::AdaptiveDetection(std::vector<Types::PointList> list)
{
    pointlist = std::move(list);
}


void AdaptiveDetection::PreProcess() {
    // TO DO : Pre process
}

Types::PointList AdaptiveDetection:: FindCandidatePoints(const Types::PointList &list, const int &step) {
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
    return ((current + k) % size + size) % size;
}

float AdaptiveDetection::GetSharpness(const Types::Point &p1, const Types::Point &p2,
                                      const Types::Point &p3)const {

    return 1 - ( GetDistance(p2, p3) / (GetDistance(p1,p3) + GetDistance(p1, p2)));
}


float AdaptiveDetection::GetAdaptiveTh(const std::vector<float> &sharpness)const {
    // filter nonzero elements
    std::vector<float> nonzero;
    std::copy_if(sharpness.begin(), sharpness.end(), std::back_inserter(nonzero), [](float sharp) {
        return sharp != 0;
        \
    }
                );

    if(nonzero.size() == 0)
        return 0;
    // return mean of nonzero
    return std::accumulate(nonzero.begin(), nonzero.end(), 0.0) / nonzero.size();
}


float AdaptiveDetection::GetDistance(const Types::Point &p1,
                                     const Types::Point &p2)const {

    float xdiff = p1.X - p2.X;
    float ydiff = p1.Y - p2.Y;
    return std::sqrt(std::pow(xdiff,2) + std::pow(ydiff, 2));
}

std::vector<int>  AdaptiveDetection::FindSalientPoints(const Types::PointList &list, const int startindex)const {
    std::vector<int> indexes;
    const int listlength = list.GetSize();
    if(listlength - 1 <= startindex)
        throw std::runtime_error("List size is less than or equal to start index");
    if(listlength < 2)
        throw std::runtime_error("Point list must include more than 2 points");
    int i = startindex;
    while( i < listlength - 2) {
        int step = 0;
        do {
            i++;
            if(list[i].Y < list[i+1].Y) {
                step += 1;
            }
            else if(list[i].Y > list[i + 1].Y) {
                step -=1;
                
            }
            if(step !=0)
                indexes.push_back(i+1);

        } while(step != 0 && i < listlength - 2);
    }
    return indexes;
}


Types::PointList AdaptiveDetection::RemoveSalientPoints(const Types::PointList &list)const {
    Types::PointList filteredlist;
    size_t length = list.GetSize();
    if(length < 2)
        throw std::runtime_error("Size of point list must be equal or greater than 2");

    int i = 1;
    Types::Direction lastdirection = FindDirection(list[0], list[1]);
    Types::Direction direction;
    while(i <= length - 2) {
        std::vector<int> sailentlist;
        direction = FindDirection(list[i], list[i+1]);
        int step = 0;
        do {
            if(list[i].Y < list[i+1].Y)
                step += 1;
            else if(list[i].Y > list[i + 1].Y)
                step -=1;

            direction = FindDirection(list[i], list[i+1]);
            sailentlist.push_back(i);
            i++;
        } while(step != 0 && i <= length - 2 && lastdirection !=  direction);

        if(sailentlist.size() > 1) {

        }
    }
    return filteredlist;
}

/*
 *   Find direction between two points
 */
Types::Direction AdaptiveDetection::FindDirection(const Types::Point &p1, const Types::Point &p2) const {
    Types::Direction direction;
    if(p1.Y == p2.Y) {
        if(p1.X > p2.X)
            direction = Types::Direction::W;
        else if (p1.X < p2.X)
            direction = Types::Direction::E;
        else
            direction = Types::Direction::SAME;
    }
    else if (p1.Y > p2.Y) {
        if (p1.X == p2.X)
            direction = Types::Direction::N;
        else if(p1.X > p2.X)
            direction = Types::Direction::NW;
        else
            direction = Types::Direction::NE;
    }
    else {
        if(p1.X == p2.X)
            direction = Types::Direction::S;
        else if (p1.X > p2.X)
            direction = Types::Direction::SW;
        else
            direction = Types::Direction::SE;
    }
    return direction;
}
}
