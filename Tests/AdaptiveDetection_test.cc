#include <gtest/gtest.h>

#undef ASSERT_FALSE
#undef ASSERT_ALL

#include "../Source/Processing/AdaptiveDetection.h"

std::vector<Types::PointList> list = {};
CornerDetections::AdaptiveDetection adaptive(std::move(list));

// Test for CyclicngClamp
TEST(CyclingClampTest, BasicAssertions) {

    std::vector<Types::PointList> list = {};
    EXPECT_TRUE(adaptive.Clamp(5,3,2) == 0);
    EXPECT_TRUE(adaptive.Clamp(5,1,-2) == 4);
    EXPECT_TRUE(adaptive.Clamp(5,2,-5) == 2);
    EXPECT_TRUE(adaptive.Clamp(5,3,-1) == 2);
    EXPECT_TRUE(adaptive.Clamp(5,1,4) == 0);
    EXPECT_TRUE(adaptive.Clamp(5,1,6) == 2);
    EXPECT_TRUE(adaptive.Clamp(5,1,8) == 4);
    EXPECT_TRUE(adaptive.Clamp(5,1,-8) == 3);

}


TEST(AdaptiveTHTest, BasicAssertions) {
    std::vector<float> values = {0,1.2,5,2.0,0,0,3};
    float value = adaptive.GetAdaptiveTh(values);
    float expected = 11.2 / 4;
    EXPECT_TRUE(value ==  expected);
}


TEST (DistanceTEST, BasicAssertions) {
    EXPECT_TRUE(std::round(adaptive.GetDistance({2, 3}, {4, 5})) == 3);
}

TEST (SharpnessTEST, BasicAssertions) {
    Types::Point p2 = {2, 3};
    Types::Point p1 = {1, 2};
    Types::Point p3  = {4, 2};

    float distanceup = adaptive.GetDistance(p1,p3);
    float distancep2p1 = adaptive.GetDistance(p1,p2);
    float distancep2p3 = adaptive.GetDistance(p2, p3);

    float real = adaptive.GetSharpness(p2,p1,p3);
    float expected = 1 - (distanceup / (distancep2p1 + distancep2p3));
    EXPECT_TRUE(real == expected);

}
