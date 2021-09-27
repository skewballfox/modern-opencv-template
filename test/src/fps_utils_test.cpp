#include "myopencvproject/fps_utils.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <cmath>

TEST(FPSZEROTEST, CheckValues)
{
  ASSERT_LT(std::abs(fps_utils::getFPS() - 0.0), .00001);
  EXPECT_TRUE(true);
}

TEST(FPSNONZEROTEST, CheckValues)
{
  double tmp = fps_utils::getFPS();
  std::chrono::time_point start = std::chrono::high_resolution_clock::now();
  do
  {
    tmp = fps_utils::getFPS();
  } while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() <
           1000);
  ASSERT_LT(std::abs(fps_utils::getFPS() - 0.0), .00001);
  EXPECT_TRUE(true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
