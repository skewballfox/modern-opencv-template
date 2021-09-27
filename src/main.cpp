//#include "functions.h"
#include <fmt/format.h>

#include <atomic>
#include <thread>

#include "boost/lockfree/spsc_queue.hpp"
#include "myopencvproject/fps_utils.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

// useful consts
const auto processor_count = std::thread::hardware_concurrency();
boost::lockfree::spsc_queue<cv::Mat, boost::lockfree::capacity<100>> feed;
atomic<bool> grabbing, processing;

void feedProducer()
{
  cv::Mat frame;
  cv::VideoCapture camera;
  camera.open(0);

  if (!camera.isOpened())
  {
    cout << "ERROR opening camera..." << endl;
    return;
  }
  while (grabbing.load() == true)
  {
    camera >> frame;
    if (frame.empty())
      continue;
    feed.push(frame);
  }
  processing.store(false);  //!!!!!!stop processing here
}

void feedConsumer()
{
  cv::Mat frame;

  double fps = 0.0;
  string fps_text = "";
  while (processing.load() == true)
  {
    feed.pop(frame);

    if (frame.empty())
    {
      continue;
    }
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
        frame,                          // target image
        fps_text,                       // text
        cv::Point(10, frame.rows / 2),  // top-left position
        cv::FONT_HERSHEY_DUPLEX,
        1.0,
        CV_RGB(118, 185, 0),  // font color
        2);
    imshow("feed", neg);
    waitKey(1);
  }
}

int main()
{
  // Configure
  cv::setNumThreads(processor_count);
  grabbing.store(true);    // set the grabbing control variable
  processing.store(true);  // ensure the processing will start

  // Capture Input
  std::thread producer(feedProducer);
  std::thread consumer(feedConsumer);
  // Initialize camera

  // your own GUI
  cout << endl << "Press Enter to stop grabbing...";
  cin.get();

  // terminate all
  grabbing.store(false);    // stop the grab loop
  processing.store(false);  // ensure to stop the processing
  producer.join();          // wait for the grab thread
  consumer.join();          // wait for the process thread

  return 0;
}