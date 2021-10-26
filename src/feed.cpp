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

template<typename Func>
void Feed::loop(Func&& f)
{
  this->start();
  this->setFrameDataProcessor(std::forward<Func>(f));

  // Capture Input
  producer = std::thread(&Feed::feedProducer, this);
  consumer = std::thread(&Feed::feedConsumer, this);
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
    frameBuffer.push(frame);
  }
  processing.store(false);  //!!!!!!stop processing here
}

// template<typename Func>
void Feed::feedConsumer()
{
  cv::Mat frame;

  while (this->processing.load() == true)
  {
    frameBuffer.pop(frame);

    if (frame.empty())
    {
      // qstd::cout << "ERROR: empty frame" << std::endl;
      continue;
    }
    this->frameDataProcessor(frame);
    if (display)
      cv::imshow(feed_name, frame);
    cv::waitKey(1);
  }
}
void Feed::start()
{
  grabbing.store(true);    // set the grabbing control variable
  processing.store(true);  // ensure the processing will start
}

void Feed::stop()
{
  grabbing.store(false);  // set the grabbing control variable
  processing.store(false);
}
template<typename Func>
void Feed::setFrameDataProcessor(Func&& f)
{
  frameDataProcessor(std::forward<Func>(f));
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