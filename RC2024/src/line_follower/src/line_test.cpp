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

void on_trackbar(int, void*)
{
    int h_min = getTrackbarPos("Hmin", "Adjust_hsv");
    int s_min = getTrackbarPos("Smin", "Adjust_hsv");
    int v_min = getTrackbarPos("Vmin", "Adjust_hsv");
    int h_max = getTrackbarPos("Hmax", "Adjust_hsv");
    int s_max = getTrackbarPos("Smax", "Adjust_hsv");
    int v_max = getTrackbarPos("Vmax", "Adjust_hsv");

    // 现在你可以使用这些新值来调整你的HSV蒙板
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "img_publisher");
    ros::NodeHandle nh;
    // image_transport::ImageTransport it(nh);
    // image_transport::Publisher pub = it.advertise("camera/image", 1);
    pub = nh.advertise<std_msgs::Float64MultiArray> ("sendlinefollow", 1);


    VideoCapture cap(0,CAP_V4L2);
    Mat frame;
    cap.open("/dev/came", CAP_V4L2);
    
    if(!cap.isOpened())
    {
        std::cerr<<"ERROR! Unable to open camera"<<std::endl;
        return -1;
    }
    ros::Rate loop_rate(30);

    namedWindow("Adjust_hsv", WINDOW_NORMAL);

    // 创建六个滑动条，分别用于调整HSV的色相（H）、饱和度（S）和明度（V）的最小值和最大值
    // Scalar lower_white = Scalar(0, 0, 208);
    // Scalar upper_white = Scalar(180, 50, 255);
    // Scalar lower_black = Scalar(0, 0, 0);
    // Scalar upper_black = Scalar(180, 255, 46);
    createTrackbar("Hmin", "Adjust_hsv", NULL, 255, on_trackbar);
    createTrackbar("Hmax", "Adjust_hsv", NULL, 255, on_trackbar);
    createTrackbar("Smin", "Adjust_hsv", NULL, 255, on_trackbar);
    createTrackbar("Smax", "Adjust_hsv", NULL, 255, on_trackbar);
    createTrackbar("Vmin", "Adjust_hsv", NULL, 255, on_trackbar);
    createTrackbar("Vmax", "Adjust_hsv", NULL, 255, on_trackbar);

    // // 黑
    // setTrackbarPos("Hmin", "Adjust_hsv", 0);
    // setTrackbarPos("Hmax", "Adjust_hsv", 180);
    // setTrackbarPos("Smin", "Adjust_hsv", 0);
    // setTrackbarPos("Smax", "Adjust_hsv", 255);
    // setTrackbarPos("Vmin", "Adjust_hsv", 0);
    // setTrackbarPos("Vmax", "Adjust_hsv", 46);
    // 白
    setTrackbarPos("Hmin", "Adjust_hsv", 0);
    setTrackbarPos("Hmax", "Adjust_hsv", 180);
    setTrackbarPos("Smin", "Adjust_hsv", 0);
    setTrackbarPos("Smax", "Adjust_hsv", 50);
    setTrackbarPos("Vmin", "Adjust_hsv", 208);
    setTrackbarPos("Vmax", "Adjust_hsv", 255);

    // 创建一个无限循环，用于更新滑动条的值
    while (true) 
    {
        Mat frame;
        cap >> frame;

        if (frame.empty()) {
            cout << "Frame is empty. Please check your image source." << endl;
            break;
        }

        // 转换到HSV色彩空间
        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 获取滑动条的值
        int h_min = getTrackbarPos("Hmin", "Adjust_hsv");
        int s_min = getTrackbarPos("Smin", "Adjust_hsv");
        int v_min = getTrackbarPos("Vmin", "Adjust_hsv");
        int h_max = getTrackbarPos("Hmax", "Adjust_hsv");
        int s_max = getTrackbarPos("Smax", "Adjust_hsv");
        int v_max = getTrackbarPos("Vmax", "Adjust_hsv");
        // 根据滑动条的值来调整HSV蒙板
        Scalar lower_hsv = Scalar(h_min, s_min, v_min);
        Scalar upper_hsv = Scalar(h_max, s_max, v_max);
        Mat mask;
        inRange(hsv, lower_hsv, upper_hsv, mask);
        Mat half_mask = mask(Rect(0, 0, mask.cols, mask.rows-320));
        Mat binary;
        threshold(mask, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
        // imshow("binary",binary);
        // 显示图像
        imshow("Adjust_hsv", half_mask);
        imshow("color",frame);
        imshow("hsv",hsv);

        // 等待用户按键，如果用户按下'q'键，就退出循环
        if (waitKey(1) == 'q') {
            break;
        }
    }
  return 0;
}