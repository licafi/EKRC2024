#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "img_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("camera/image", 1);
  VideoCapture cap(0,CAP_V4L2);
  Mat frame;
  int deviceID=0;
  if(argc>1)
    deviceID=argv[1][0]-'0';
  int apiID=CAP_ANY;
  cap.open(deviceID+apiID);
  if(!cap.isOpened()){
    std::cerr<<"ERROR! Unable to open camera"<<std::endl;
    return -1;
  }

  ros::Rate loop_rate(30);
  while (ros::ok()) 
  {
    cap.read(frame);
    if(!frame.empty())
    {
      // Convert the image from BGR to HSV
      Mat hsv;
      cvtColor(frame, hsv, COLOR_BGR2HSV);
      // Define range for white color in HSV
      Scalar lower_white = Scalar(0, 0, 200);
      Scalar upper_white = Scalar(180, 255, 255);
      // Threshold the HSV image to get only white colors
      Mat mask;
      inRange(hsv, lower_white, upper_white, mask);
      // Convert the mask to a binary image
      Mat binary;
      threshold(mask, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
      
      //Mat hough;
      

      int h = binary.rows;
      int w = binary.cols;
      int d = binary.channels();
      double last_erro, erro, d_erro;
      
      int search_top = h - 40;
      int search_bot = h;
      //选取合适长宽
      mask(Range(0, search_top), Range(0, w)) = 0;
      mask(Range(search_bot, h), Range(0, w)) = 0;
      Moments M = moments(mask);
      if (M.m00 > 0) 
      {
          int cx = static_cast<int>(M.m10 / M.m00);
          int cy = static_cast<int>(M.m01 / M.m00);
          // ROS_INFO("M.m10: %f", M.m10);
          // ROS_INFO("M.m01: %f", M.m01);
          // ROS_INFO("M.m00: %f", M.m00);
          // ROS_INFO("----------");
          circle(binary, Point(cx, cy), 10, Scalar(0, 0, 255), -1);

          erro = cx - w / 2.0 - 15;
          d_erro = erro - last_erro;
          last_erro = erro;
      }
      
      // ROS_INFO("last_erro: %f", last_erro);
      // ROS_INFO("erro: %f", erro);
      // ROS_INFO("d_erro: %f", d_erro);
      // ROS_INFO("----------");
      imshow("binary",binary);
      //imshow("hough",hough);
      waitKey(1);
      sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", mask).toImageMsg();
      pub.publish(msg);
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}