#include "myopencvproject/feed.hpp"

Feed::Feed(int feed_ident, string feed_name)
{
  this->feed_ident = feed_ident;
  this->feed_name = feed_name;
  // Configure
  // cv::setNumThreads(processor_count);
  grabbing.store(true);    // set the grabbing control variable
  processing.store(true);  // ensure the processing will start
}

void Feed::loop(void (*func)(cv::Mat&))
{
  grabbing.store(true);    // set the grabbing control variable
  processing.store(true);  // ensure the processing will start
  // Capture Input
  producer(feedProducer);
  consumer(feedConsumer, std::ref(func));
}

void Feed::feedProducer()
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

void Feed::feedConsumer(void (*func)(cv::Mat&))
{
  cv::Mat frame;

  double fps = 0.0;
  string fps_text = "";
  while (processing.load() == true)
  {
    feed.pop(frame);

    if (frame.empty())
    {
      std::cout << "ERROR: empty frame" << endl;
      continue;
    }
    func(frame);
    imshow(feed_name, neg);
    waitKey(1);
  }
}

Feed::~Feed()
{
  grabbing.store(false);
  processing.store(false);
}