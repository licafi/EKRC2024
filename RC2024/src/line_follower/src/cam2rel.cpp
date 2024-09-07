#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <std_msgs/Float64MultiArray.h>

using namespace cv;
using namespace std;

ros::Publisher pub;

void linecal(Mat binary) //没用的函数
{
  int h = binary.rows;
  int w = binary.cols;
  int d = binary.channels();
  double last_erro, erro, d_erro;
  
  int search_top = h - 40;
  int search_bot = h;
  //选取合适长宽
  binary(Range(0, search_top), Range(0, w)) = 0;
  binary(Range(search_bot, h), Range(0, w)) = 0;
  Moments M = moments(binary);
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

  std_msgs::Float64MultiArray position_array;
  position_array.data.push_back(last_erro);
  position_array.data.push_back(erro);
  position_array.data.push_back(d_erro);
  pub.publish(position_array); // 发布
}


int main(int argc, char** argv)
{ 
  int crossmode = 0;
  ros::init(argc, argv, "img_publisher");
  ros::NodeHandle nh;
  // image_transport::ImageTransport it(nh);
  // image_transport::Publisher pub = it.advertise("camera/image", 1);
  pub = nh.advertise<std_msgs::Float64MultiArray> ("sendlinefollow", 1);


  VideoCapture cap(0,CAP_V4L2);
  Mat frame;
  int deviceID=0;
  if(argc>1)
    deviceID=argv[1][0]-'0';
  int apiID=CAP_ANY;
  cap.open("/dev/came", CAP_V4L2);
  //cap.open(deviceID+apiID);
  if(!cap.isOpened())
  {
    std::cerr<<"ERROR! Unable to open camera"<<std::endl;
    return -1;
  }
  ros::Rate loop_rate(30);
  
  while (ros::ok()) 
  {
    cap.read(frame);
    if(!frame.empty())
    {
      flip(frame, frame, -1);
      Mat hsv;

      // Define range for white color in HSV
      //Scalar lower_white = Scalar(0, 10, 210);
      Scalar lower_white = Scalar(36, 15, 180);
      Scalar upper_white = Scalar(180, 59, 255);
      Scalar lower_black = Scalar(0, 0, 0);
      Scalar upper_black = Scalar(180, 255, 70);

      // Threshold the HSV image to get only white colors
      Mat top_half = frame(Rect(0, 0, frame.cols, frame.rows-320));
      // imshow("color",frame);
      // imshow("color",top_half);
      cvtColor(top_half, hsv, COLOR_BGR2HSV);

      Mat mask;
      inRange(hsv, lower_white, upper_white, mask);
      // imshow("hsv",hsv);
      
      // Convert the mask to a binary image
      Mat binary;
      threshold(mask, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
      imshow("binary",binary);
      Mat eroded_binary;
      int erosion_size = 1;  
      Mat element_eroded = getStructuringElement(MORPH_RECT, 
            Size(2 * erosion_size + 1, 2 * erosion_size + 1), 
            Point(erosion_size, erosion_size));
      erode(binary, eroded_binary, element_eroded);
      // imshow("Eroded Binary", eroded_binary);
      // Mat dilated_binary;
      // int dilation_size = 1;  
      // Mat element_dilated = getStructuringElement(MORPH_RECT, 
      //       Size(2 * dilation_size + 1, 2 * dilation_size + 1), 
      //       Point(dilation_size, dilation_size));
      // dilate(eroded_binary, dilated_binary, element_dilated);
      // imshow("Dilated Binary", dilated_binary);
      binary = eroded_binary;

      int gridSize = 5;
      int cellWidth = binary.cols / gridSize;
      int cellHeight = binary.rows / gridSize;
      vector<vector<int>> areas(gridSize, vector<int>(gridSize, 0.0));
      vector<int> sum_width(gridSize);
      // Loop over the grid
      for (int i = 0; i < gridSize; i++) {
          for (int j = 0; j < gridSize; j++) {
              // Define the region of interest (ROI) for the current cell
              Rect roi(j * cellWidth, i * cellHeight, cellWidth, cellHeight);
              // Calculate the moments of the ROI
              Moments m = moments(binary(roi), true);
              // Calculate the area of the ROI and store it in the array
              if(m.m00>800) areas[i][j] = 1;   //改这里
              else areas[i][j] = 0;
          }
      }
      // Print the areas
      crossmode = 0;
      for (int i = 0; i < gridSize; i++) {
          for (int j = 0; j < gridSize; j++) {
              cout << areas[i][j] << " ";
              sum_width[i]+=areas[i][j];
              if (sum_width[i]>=3) crossmode=1;
          }
          cout<< "       "<<sum_width[i];
          cout << endl;
      }
      
      // if(sum_width[2]>=4 &&(areas[2][1]==areas[2][3]==1)) crossmode=1&&cout<<"mod: cross";
      // else if(sum_width[2]==3 && areas[2][3]==areas[2][4]==1 && areas[1][2]==0) crossmode=2&&cout<<"mod: only right"<< endl;
      // else if(sum_width[2]==3 && areas[2][1]==areas[2][0]==1 && areas[1][2]==0) crossmode=3&&cout<<"mod: only left"<< endl;
      // else if(sum_width[2]==3 && areas[2][3]==areas[2][4]==1 && areas[1][2]==1) crossmode=4&&cout<<"mod: maybe right"<< endl;
      // else if(sum_width[2]==3 && areas[2][1]==areas[2][0]==1 && areas[1][2]==1) crossmode=5&&cout<<"mod: maybe left"<< endl;
      // else crossmode=0&&cout<<"mod: line"<< endl;
      cout<<"-------------" << endl;


      int h = binary.rows;
      int w = binary.cols;
      int d = binary.channels();
      double last_erro, erro, d_erro, en;
      std_msgs::Float64MultiArray position_array;

      int search_top = 0;
      int search_bot = 40;
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
          if (circle) 
          {
          // erro = cx - w / 2.0 - 15;
          // d_erro = erro - last_erro;
          // last_erro = erro;
            ROS_INFO("cx %d",cx);
            if (crossmode==1) ROS_INFO("READ MAP");
            en = 1; // en=1时get line
            position_array.data.push_back(cx);
            position_array.data.push_back(cy);
            position_array.data.push_back(w);
            position_array.data.push_back(crossmode);
            position_array.data.push_back(en);
            pub.publish(position_array); // 发布  
          }
              
      }
      else
      {
        en = 0;
        position_array.data.push_back(0);
        position_array.data.push_back(0);
        position_array.data.push_back(0);
        position_array.data.push_back(0);
        position_array.data.push_back(en);  
        ROS_INFO("EN %f",en);
        pub.publish(position_array); // 发布
        en = 0;
        position_array.data.clear();
      }
      // std_msgs::Float64MultiArray position_array;
      // position_array.data.push_back(last_erro);
      // position_array.data.push_back(erro);
      // position_array.data.push_back(d_erro);
      // position_array.data.push_back(crossmode);
      // pub.publish(position_array); // 发布
      
      // ROS_INFO("last_erro: %f", last_erro);
      // ROS_INFO("erro: %f", erro);
      // ROS_INFO("d_erro: %f", d_erro);
      // ROS_INFO("----------");
      
      // if cv2.circle:
      //       # 计算图像中心线和目标指示线中心的距离
      //           erro = cx - w/2-15
      //           d_erro=erro-last_erro
      //           self.twist.linear.x = 0.18
      //           if erro!=0:
      //               self.twist.angular.z = -float(erro)*0.005-float(d_erro)*0.000
      //           else :
      //               self.twist.angular.z = 0
      //           last_erro=erro
      //   else:
      //       self.twist.linear.x = 0
      //       self.twist.angular.z = 0
      //   self.cmd_vel_pub.publish(self.twist)

      // imshow("color",top_half);
      // imshow("hsv",hsv);
      // imshow("binary",binary);

      waitKey(1);
      // sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", hsv).toImageMsg();
      // pub.publish(msg);
    }


    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}


