#include "myopencvproject/feed.hpp"

Feed::Feed(int feed_ident, std::string feed_name)
{
  feed_id = feed_ident;
  this->feed_name = feed_name;
  display = false;
  // Configure
  // cv::setNumThreads(processor_count);
  grabbing.store(true);    // set the grabbing control variable
  processing.store(true);  // ensure the processing will start
}

void Feed::loop(std::function<void()> func(cv::Mat&))
{
  grabbing.store(true);    // set the grabbing control variable
  processing.store(true);  // ensure the processing will start
  // Capture Input
  producer = std::thread(&Feed::feedProducer, this);
  consumer = std::thread(&Feed::feedConsumer, this, std::ref(func));
}

void Feed::feedProducer()
{
  cv::Mat frame;
  cv::VideoCapture camera;
  camera.open(0);

  if (!camera.isOpened())
  {
    std::cout << "ERROR opening camera..." << std::endl;
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

void Feed::feedConsumer(std::function<void()> func(cv::Mat&))
{
  cv::Mat frame;

  while (this->processing.load() == true)
  {
    feed.pop(frame);

    if (frame.empty())
    {
      std::cout << "ERROR: empty frame" << std::endl;
      continue;
    }
    func(frame);
    if (display)
      cv::imshow(feed_name, frame);
    cv::waitKey(1);
  }
}

void Feed::setDisplay(bool display)
{
  this->display = display;
}
Feed::~Feed()
{
  grabbing.store(false);
  processing.store(false);
}