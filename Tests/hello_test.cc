#include <gtest/gtest.h>

#undef ASSERT_FALSE
#undef ASSERT_ALL

#include "../Source/Processing/AdaptiveDetection.h"

// Test for CyclicngClamp
TEST(CyclingClamTest, BasicAssertions) {

    std::vector<Types::PointList> list = {};
    CornerDetections::AdaptiveDetection adaptive(std::move(list));
    EXPECT_TRUE(adaptive.Clamp(5,3,2) == 0);
    EXPECT_TRUE(adaptive.Clamp(5,1,-2) == 4);
    EXPECT_TRUE(adaptive.Clamp(5,2,-5) == 2);
    EXPECT_TRUE(adaptive.Clamp(5,3,-1) == 2);
    EXPECT_TRUE(adaptive.Clamp(5,1,4) == 0);
    EXPECT_TRUE(adaptive.Clamp(5,1,6) == 2);
    EXPECT_TRUE(adaptive.Clamp(5,1,8) == 4);
    EXPECT_TRUE(adaptive.Clamp(5,1,-8) == 3);

}
