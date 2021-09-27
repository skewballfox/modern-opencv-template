#ifndef MYOPENCVPROJECT_FEED_H_
#define MYOPENCVPROJECT_FEED_H_

#include <atomic>
#include <thread>

#include "boost/lockfree/spsc_queue.hpp"
#include "opencv2/opencv.hpp"

//#TODO: finish moving functions out of main and encapsulate them in this class
class Feed
{
 private:
  int feed_id;
  string feed_name;

  boost::lockfree::spsc_queue<cv::Mat, boost::lockfree::capacity<100>> feed;
  atomic<bool> grabbing, processing;
  std::thread producer, consumer;

  void feedProducer();
  void feedConsumer();

 public:
  Feed(int feed_id);
  ~Feed();

  loop(void (*func)(cv::Mat&));
}

#endif  // MYOPENCVPROJECT_FEED_H_
