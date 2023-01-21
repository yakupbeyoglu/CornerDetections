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

TEST (FindDirectionTest, BasicAssertions) {
    Types::Point p1 = {0, 0};
    Types::Point p2 = {1, 0};

    EXPECT_EQ(adaptive.FindDirection(p1, p2), Types::Direction::E);
    EXPECT_EQ(adaptive.FindDirection(p2, p1), Types::Direction::W);

    p2 = {0, 1};
    EXPECT_EQ(adaptive.FindDirection(p1, p2), Types::Direction::S);
    EXPECT_EQ(adaptive.FindDirection(p2, p1), Types::Direction::N);

    p2 = {1, 1};
    EXPECT_EQ(adaptive.FindDirection(p1, p2), Types::Direction::SE);
    EXPECT_EQ(adaptive.FindDirection(p2, p1), Types::Direction::NW);

    p2 = {-1, 1};
    EXPECT_EQ(adaptive.FindDirection(p1, p2), Types::Direction::SW);
    EXPECT_EQ(adaptive.FindDirection(p2, p1), Types::Direction::NE);
    
    EXPECT_EQ(adaptive.FindDirection(p2, p2), Types::Direction::SAME);

}

TEST(FindSalientPoints, BasicAssertions) {
    Types::PointList testlist = {
        {0, 1}, {1, 1}, {2, 1} , {3, 0}, {4, 0}, {5, 0}, {6, 1}, {7, 1}, {8, 1} 
    };
    std::vector<int> expected = {3, 4, 5};
    std::vector<int> list = adaptive.FindSalientPoints(testlist, 0);
    
    EXPECT_EQ(expected.size(), list.size());
    for(int i = 0; i < list.size(); i++)
        EXPECT_EQ(expected[i], list[i]);
    
    Types::PointList testlist2 = {
        {0, 1}, {1, 1}, {2, 1}, {3, 2}, {4, 2}, {5, 2}, {6, 1}, {7, 1}, {8, 1},{ 9, 0},{10, 1}
    };
    
    std::vector<int> expected2 = {3, 4, 5, 9};
    std::vector<int> list2 = adaptive.FindSalientPoints(testlist2, 0);
    EXPECT_EQ(expected2.size(), list2.size());
    for(int i = 0; i < list.size(); i++)
        EXPECT_EQ(expected2[i], list2[i]);
    
}
