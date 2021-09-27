#include "myopencvproject/fps_utils.hpp"

#include <chrono>

double fps_utils::getFPS()
{
  static std::chrono::time_point oldTime = std::chrono::high_resolution_clock::now();
  static const std::chrono::seconds wait_duration(5);
  static unsigned int frameCount = 0;
  static double fps = 0.0;
  frameCount++;

  if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= wait_duration)
  {
    oldTime = std::chrono::high_resolution_clock::now();
    fps = frameCount / 5.0;
    //*10.0;
    frameCount = 0;
  }
  return fps;
}