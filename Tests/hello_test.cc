#include <gtest/gtest.h>

#undef ASSERT_FALSE
#undef ASSERT_ALL

#include "../Source/Processing/AdaptiveDetection.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
  EXPECT_EQ(8 * 6, 48);

}

// Demonstrate some basic assertions.
TEST(HelloTest2, BasicAssertions) {
  std::string  s = CornerDetections::AdaptiveDetection::Test();
  std::string s2 = "test";
  EXPECT_TRUE(s != s2);
  Types::Point p;
  p = {3,5};
  EXPECT_TRUE(p.X == 3 && p.Y == 5);
  EXPECT_FALSE(p.X == 5);
}
