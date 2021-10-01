#ifndef MYOPENCVPROJECT_FEED_H_
#define MYOPENCVPROJECT_FEED_H_

#include <atomic>
#include <string>
#include <thread>

#include "boost/lockfree/spsc_queue.hpp"
#include "opencv2/opencv.hpp"

//#TODO: finish moving functions out of main and encapsulate them in this class
class Feed
{
 private:
  int feed_id;
  std::string feed_name;
  bool display;

  boost::lockfree::spsc_queue<cv::Mat, boost::lockfree::capacity<100>> feed;
  std::atomic<bool> grabbing, processing;
  std::thread producer, consumer;

  void feedProducer();

  template<typename F>
  void feedConsumer(F func(cv::Mat&));

 public:
  Feed(int, std::string);
  ~Feed();

  void setDisplay(bool display);

  template<typename F>
  void loop(F&& func(cv::Mat&));
};

#endif  // MYOPENCVPROJECT_FEED_H_
