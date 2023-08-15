#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>


using namespace std;
//using namespace cv;

cv::Point2f dotes[] = { cv::Point2f(138,268),cv::Point2f(365,268),cv::Point2f(408,334),cv::Point2f(97,329) }; // car1.mp4
cv::Point2f changing[] = { cv::Point2f(0,0),cv::Point2f(480,0),cv::Point2f(480,500),cv::Point2f(0,500) };
vector<int> lines;
cv::Mat intrst;
cv::Mat sonuc;
int leftLoc;
int rightLoc;
int pid;

static const std::string OPENCV_WINDOW = "LANE DETECTION";

class find_Line
{
  
  ros::NodeHandle nh;

  image_transport::ImageTransport imtr;
  image_transport::Subscriber image_sub;
  image_transport::Publisher image_pub;

  public:

  find_Line() : imtr(nh)
  {

    image_sub = imtr.subscribe("/usb_cam/image_raw", 1, &find_Line::func, this);
    image_pub = imtr.advertise("/lane_detection_cpp", 1);

  }

  ~find_Line(){}

  
void find_lineRIO() {

	lines.resize(480);
	lines.clear();

	for (int i = 0; i < 480; i++) {

		intrst = sonuc(cv::Rect(i, 100, 1, 150)); /////
		cv::divide(255, intrst, intrst);
		lines.push_back((int)(sum(intrst)[0]));

	}
}

void line_show(cv::Mat son) {

	vector<int>::iterator leftLine;
	vector<int>::iterator rightLine;

	leftLine = max_element(lines.begin(), lines.begin() + 250);
	leftLoc = distance(lines.begin(), leftLine);

	rightLine = max_element(lines.begin() + 345, lines.end());
	rightLoc = distance(lines.begin(), rightLine);

	cv::line(son, cv::Point(leftLoc, 0), cv::Point(leftLoc, 500), cv::Scalar(0, 0, 256), 3);
	cv::line(son, cv::Point(rightLoc, 0), cv::Point(rightLoc, 500), cv::Scalar(0, 0, 256), 3);

	//cout << "sag serit mesafe : " << rightLoc << endl;
	//cout << "sol serit mesafe : " << leftLoc << endl;

	//imshow("serit tespiti", son);



}

void findLinesCenter(cv::Mat son) {

	int linesCenter = ((rightLoc - leftLoc) / 2) + leftLoc;
	int lineCalibre = (son.cols / 2) - 9;
	pid = linesCenter - lineCalibre;


	cv::line(son, cv::Point(linesCenter, 0), cv::Point(linesCenter, 500), cv::Scalar(0, 256, 0), 3); // sağ ve sol şeritlerin ortası
	line(son, cv::Point(lineCalibre, 0), cv::Point(lineCalibre, 500), cv::Scalar(255, 0, 0), 3); // ilgi alanının ortası
	//imshow("serit merkez", son);

	stringstream yazi;
	yazi.str(" ");
	yazi.clear();
	yazi << "PID Values :" << pid << endl;
	putText(son, yazi.str(), cv::Point(50, 50), CV_FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 0), 2);
	imshow("pid", son);



}


  void bianrytr(cv::Mat giris) {

	cv::Mat cikis, bilat;
	cv::imshow("giris", giris);

	cv::bilateralFilter(giris, bilat, 15, 15, 15);
	cv::cvtColor(bilat, bilat, CV_BGR2GRAY);

	cv::threshold(bilat, cikis, 150, 255, CV_THRESH_BINARY);
	cv::dilate(cikis, cikis, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));

	cv::morphologyEx(cikis, cikis, cv::MORPH_OPEN, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 1); // önce yayma sonra aşındırma dilate,erode
	cv::morphologyEx(cikis, cikis, cv::MORPH_CLOSE, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)), cv::Point(-1, -1), 1); // önce aşındırma sonra aşındırma

	cv::Canny(bilat, bilat, 150, 255);
	cv::add(bilat, cikis, sonuc);

	cv::imshow("Kenar eklenmiş form", sonuc);

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

      cv::Mat pers = getPerspectiveTransform(dotes, changing);
			cv::Mat son(480, 500, CV_8UC3);
			cv::warpPerspective(cv_ptr->image, son, pers, son.size());
			cv::resize(cv_ptr->image, cv_ptr->image, cv::Size(480, 500));

      bianrytr(son);
      find_lineRIO();
			line_show(son);

			findLinesCenter(son);
      cv::imshow("son", son);
          
    cv::waitKey(3);

    image_pub.publish(cv_ptr->toImageMsg());
    
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "cember_tespiti");
  
  find_Line obje;
  ros::spin();
  
  return 0;
}