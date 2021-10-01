//#include "functions.h"
#include <fmt/format.h>

#include <atomic>
#include <thread>

#include "boost/lockfree/spsc_queue.hpp"
#include "myopencvproject/feed.hpp"
#include "myopencvproject/fps_utils.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

// useful consts
const auto processor_count = std::thread::hardware_concurrency();

int main()
{
  // Configure
  cv::setNumThreads(processor_count);
  // cv::setUseOptimized(true);

  Feed feed(0, "feed");
  feed.setDisplay(true);

  double fps = 0.0;
  string fps_text = "";
  feed.loop(
      [&](Mat& frame)
      {
        // Compute negative image, i.e., 255 - pixel_val
        Mat neg = Mat(frame.size(), frame.type());
        neg = Scalar(255, 255, 255) - frame;

        // Resize input frame
        Mat frame_rz;
        resize(frame, frame_rz, Size(), 0.2, 0.2);

        // Set image region of interest (ROI) to resized frame
        Rect roi = Rect(10, 10, frame_rz.cols, frame_rz.rows);
        frame_rz.copyTo(neg(roi));

        // Draw frame around ROI
        rectangle(neg, roi, Scalar(200, 200, 200), 3);

        // Display FPS
        fps = fps_utils::getFPS();
        fps_text = cv::format("%f", fps);
        cv::putText(
            neg,                            // target image
            fps_text,                       // text
            cv::Point(10, frame.rows / 2),  // top-left position
            cv::FONT_HERSHEY_DUPLEX,
            1.0,
            CV_RGB(118, 185, 0),  // font color
            2);
      });

  return 0;
}