#include <ros/ros.h>
#include <image_transport/image_transport.h> //goruntuleri yayinlamak ve abone olmak icin
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h> //format donusturme islemi icin
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW = "Kamera Goruntusu";

class Kamera
{
  ros::NodeHandle nh;

  image_transport::ImageTransport imtr;
  image_transport::Subscriber image_sub;
  image_transport::Publisher image_pub;

public:
  Kamera() : imtr(nh)
  
  {

    image_sub = imtr.subscribe("/usb_cam/image_raw", 1, &Kamera::func, this);
    image_pub = imtr.advertise("/kamera_cpp", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~Kamera()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
  
  void func(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }

    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge hatasi: %s", e.what());
      return;
    }

    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);

    image_pub.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "kamera_goruntusu");
  
  Kamera obje;
  ros::spin();
  
  return 0;
}