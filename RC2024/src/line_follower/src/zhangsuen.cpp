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
// vector<int> neighbours(int x, int y, Mat& img) {
//     return {
//         img.at<uchar>(y-1, x), img.at<uchar>(y-1, x+1), img.at<uchar>(y, x+1), img.at<uchar>(y+1, x+1),
//         img.at<uchar>(y+1, x), img.at<uchar>(y+1, x-1), img.at<uchar>(y, x-1), img.at<uchar>(y-1, x-1)
//     };
// }

// int transitions(vector<int> n) {
//     int count = 0;
//     for (int i = 0; i < n.size(); i++) {
//         if (n[i] == 0 && n[(i + 1) % n.size()] == 1) count++;
//     }
//     return count;
// }

// Mat zhangSuenThinning(Mat& img) {
//     bool hasChanged;
//     do {
//         hasChanged = false;
//         vector<Point> toBeRemoved;
//         for (int y = 1; y < img.rows - 1; y++) {
//             for (int x = 1; x < img.cols - 1; x++) {
//                 if (img.at<uchar>(y, x) == 255) {
//                     auto n = neighbours(x, y, img);
//                     int sum = 0;
//                     for (int val : n) sum += val == 255 ? 1 : 0;
//                     if (sum >= 2 && sum <= 6 && transitions(n) == 1 &&
//                         (n[0] * n[2] * n[4] == 0) && (n[2] * n[4] * n[6] == 0)) {
//                         toBeRemoved.push_back(Point(x, y));
//                         hasChanged = true;
//                     }
//                 }
//             }
//         }
//         for (Point p : toBeRemoved) img.at<uchar>(p.y, p.x) = 0;
        
//         toBeRemoved.clear();
//         for (int y = 1; y < img.rows - 1; y++) {
//             for (int x = 1; x < img.cols - 1; x++) {
//                 if (img.at<uchar>(y, x) == 255) {
//                     auto n = neighbours(x, y, img);
//                     int sum = 0;
//                     for (int val : n) sum += val == 255 ? 1 : 0;
//                     if (sum >= 2 && sum <= 6 && transitions(n) == 1 &&
//                         (n[0] * n[2] * n[6] == 0) && (n[0] * n[4] * n[6] == 0)) {
//                         toBeRemoved.push_back(Point(x, y));
//                         hasChanged = true;
//                     }
//                 }
//             }
//         }
//         for (Point p : toBeRemoved) img.at<uchar>(p.y, p.x) = 0;
//     } while (hasChanged);

// }
void thinImage(Mat & srcImg) {
    vector<Point> deleteList;
    int neighbourhood[9];
    int nl = srcImg.rows;
    int nc = srcImg.cols;
    bool inOddIterations = true;
    while (true) {
        for (int j = 1; j < (nl - 1); j++) {
            uchar* data_last = srcImg.ptr<uchar>(j - 1);
            uchar* data = srcImg.ptr<uchar>(j);
            uchar* data_next = srcImg.ptr<uchar>(j + 1);
            for (int i = 1; i < (nc - 1); i++) {
                if (data[i] == 255) {
                    int whitePointCount = 0;
                    neighbourhood[0] = 1;
                    if (data_last[i] == 255) neighbourhood[1] = 1;
                    else  neighbourhood[1] = 0;
                    if (data_last[i + 1] == 255) neighbourhood[2] = 1;
                    else  neighbourhood[2] = 0;
                    if (data[i + 1] == 255) neighbourhood[3] = 1;
                    else  neighbourhood[3] = 0;
                    if (data_next[i + 1] == 255) neighbourhood[4] = 1;
                    else  neighbourhood[4] = 0;
                    if (data_next[i] == 255) neighbourhood[5] = 1;
                    else  neighbourhood[5] = 0;
                    if (data_next[i - 1] == 255) neighbourhood[6] = 1;
                    else  neighbourhood[6] = 0;
                    if (data[i - 1] == 255) neighbourhood[7] = 1;
                    else  neighbourhood[7] = 0;
                    if (data_last[i - 1] == 255) neighbourhood[8] = 1;
                    else  neighbourhood[8] = 0;
                    for (int k = 1; k < 9; k++) {
                        whitePointCount += neighbourhood[k];
                    }
                    if ((whitePointCount >= 2) && (whitePointCount <= 6)) {
                        int ap = 0;
                        if ((neighbourhood[1] == 0) && (neighbourhood[2] == 1)) ap++;
                        if ((neighbourhood[2] == 0) && (neighbourhood[3] == 1)) ap++;
                        if ((neighbourhood[3] == 0) && (neighbourhood[4] == 1)) ap++;
                        if ((neighbourhood[4] == 0) && (neighbourhood[5] == 1)) ap++;
                        if ((neighbourhood[5] == 0) && (neighbourhood[6] == 1)) ap++;
                        if ((neighbourhood[6] == 0) && (neighbourhood[7] == 1)) ap++;
                        if ((neighbourhood[7] == 0) && (neighbourhood[8] == 1)) ap++;
                        if ((neighbourhood[8] == 0) && (neighbourhood[1] == 1)) ap++;
                        if (ap == 1) {
                            if (inOddIterations && (neighbourhood[3] * neighbourhood[5] * neighbourhood[7] == 0)
                                && (neighbourhood[1] * neighbourhood[3] * neighbourhood[5] == 0)) {
                                deleteList.push_back(Point(i, j));
                            }
                            else if (!inOddIterations && (neighbourhood[1] * neighbourhood[5] * neighbourhood[7] == 0)
                                && (neighbourhood[1] * neighbourhood[3] * neighbourhood[7] == 0)) {
                                deleteList.push_back(Point(i, j));
                            }
                        }
                    }
                }
            }
        }
        if (deleteList.size() == 0)
            break;
        for (size_t i = 0; i < deleteList.size(); i++) {
            Point tem;
            tem = deleteList[i];
            uchar* data = srcImg.ptr<uchar>(tem.y);
            data[tem.x] = 0;
        }
        deleteList.clear();
 
        inOddIterations = !inOddIterations;
    }
}


// Define the Hough transform parameters
      double rho = 1; // distance resolution in pixels of the Hough grid
      double theta = CV_PI/180; // angular resolution in radians of the Hough grid
      int threshold = 50; // minimum number of votes (intersections in Hough grid cell)
      double minLineLength = 100; // minimum number of pixels making up a line
      double maxLineGap = 10; // maximum gap in pixels between connectable line segments

      // Create a vector to store the lines
      vector<Vec4i> lines;

      // Run Hough on the binary image
      HoughLinesP(binary, lines, rho, theta, threshold, minLineLength, maxLineGap);

      // Draw the lines on the original image
      for( size_t i = 0; i < lines.size(); i++ )
      {
          Vec4i l = lines[i];
          line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
      }

      // Show the result
      imshow("Result", frame);
      waitKey(1);