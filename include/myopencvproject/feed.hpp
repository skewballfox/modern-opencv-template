#ifndef MYOPENCVPROJECT_FEED_H_
#define MYOPENCVPROJECT_FEED_H_

#include <atomic>
#include <string>
#include <thread>

#include "boost/lockfree/spsc_queue.hpp"
#include "opencv2/opencv.hpp"

//#TODO: finish moving functions out of main and encapsulate them in this class

// template<typename Func>
// struct Func;
class Feed
{
 private:
  int feed_id;
  std::string feed_name;
  bool display;

  boost::lockfree::spsc_queue<cv::Mat, boost::lockfree::capacity<100>> frameBuffer;
  std::atomic<bool> grabbing, processing;
  std::thread producer, consumer;

  std::function<void(cv::Mat&)> frameDataProcessor;

  void feedProducer();

  template<typename Func>
  void feedConsumer();

 public:
  Feed(int, std::string);
  ~Feed();

  void setDisplay(bool display);

  template<typename Func>
  void setFrameDataProcessor(Func&& f);

  template<typename Func>
  void loop(Func&& f);

  void start();
  void stop();
};

#endif  // MYOPENCVPROJECT_FEED_H_
