#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <std_msgs/Int8.h>
#include <std_msgs/Float64MultiArray.h>
// #include <depthcam/position3d.h>
#include <depthcam/BoundingBoxes.h>
#include <depthcam/BoundingBox.h>

#define fx 606.3499755859375
#define fy 605.1943359375
#define ppx 328.8806457519531
#define ppy 249.58609008789062

using namespace std;

struct Box {
    std::string name;
    double prob;
    int xmin;
    int xmax;   
    int ymin;
    int ymax;
    double cx;
    double cy;
};
std::vector<Box> temp_boxes;

struct POS {
    int x3d;
    int y3d;
    int z3d;
};

int yolo_count;

ros::Publisher position3d_pub;

void callback(const sensor_msgs::ImageConstPtr& data1, const sensor_msgs::ImageConstPtr& data2)
{
    cv_bridge::CvImagePtr color_img = cv_bridge::toCvCopy(data1, "bgr8");
    cv_bridge::CvImagePtr depth_img = cv_bridge::toCvCopy(data2, "16UC1");

    if(temp_boxes.size() != 0)
        {
            double pos_forward,pos_trans,find_color,find_hsv=0;
            vector<double> real_z;
            vector<double> real_x;
            vector<double> real_y;
            vector<int> real_color;

            int min_index = -1;
            int min_x = INT_MAX;  
            for(size_t i = 0; i < temp_boxes.size(); ++i)
            {
                int temp_z,temp_x,temp_y;
                temp_z =  depth_img->image.at<uint16_t>(temp_boxes[i].cy, temp_boxes[i].cx);
                temp_x = (temp_boxes[i].cx - ppx) / fx * temp_z;
                temp_y = (temp_boxes[i].cy - ppy) / fy * temp_z;
                real_z.push_back(temp_z);
                real_x.push_back(temp_x);
                real_y.push_back(temp_y);
                if(temp_boxes[i].name=="red")
                {
                    real_color.push_back(1);
                    if(abs(temp_x) < min_x)
                    {
                        min_x = abs(temp_x);
                        min_index = i;
                    }
                }
                else if(temp_boxes[i].name=="blue")
                {
                    real_color.push_back(2);
                }
                else if(temp_boxes[i].name=="purple")
                {
                    real_color.push_back(3);
                }
                
                if(min_index == -1)
                {
                    min_index = 0;  // 保证去找一个球
                }
                pos_forward = real_z[min_index];
                pos_trans = real_x[min_index];
                find_color = real_color[min_index];
            }
                std_msgs::Float64MultiArray position_array;
                position_array.data.push_back(pos_forward);
                position_array.data.push_back(pos_trans);
                position_array.data.push_back(find_color);
                if((position_array.data[0]!=0) && (position_array.data[1]!= 0))
                    position3d_pub.publish(position_array); // 发布
        }
    temp_boxes.clear();
}

void block_callback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg)
{
    Box temp_box;
    for(const auto& box : msg->bounding_boxes)
    {   
        temp_box.name = box.Class;
        temp_box.prob = box.probability;
        temp_box.xmin = box.xmin;
        temp_box.xmax = box.xmax;
        temp_box.ymin = box.ymin;
        temp_box.ymax = box.ymax;
        temp_box.cx = (box.xmin + box.xmax) / 2;
        temp_box.cy = (box.ymin + box.ymax) / 2;
    }
    // ROS_INFO_STREAM("Finished processing " << msg->bounding_boxes.size() << " bounding boxes");
    temp_boxes.push_back(temp_box);
    // temp_box.clear();
}

void count_callback(const std_msgs::Int8::ConstPtr& msg)
{
    yolo_count = msg->data;
    if (yolo_count == 0)
    {
        std_msgs::Float64MultiArray position_array;
            position_array.data.push_back(-1);
            position_array.data.push_back(-1);
            position_array.data.push_back(-1);
            position3d_pub.publish(position_array); // 发布
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "get_position");
    ros::NodeHandle nh;
    ROS_INFO("get_image node started");
    message_filters::Subscriber<sensor_msgs::Image> color_sub(nh, "/camera/color/image_raw", 1);
    message_filters::Subscriber<sensor_msgs::Image> depth_sub(nh, "/camera/aligned_depth_to_color/image_raw", 1);
    

    ros::Subscriber yolo_bounding_boxes_sub = nh.subscribe("/darknet_ros/bounding_boxes", 1, block_callback);
    ros::Subscriber yolo_boxes_count_sub = nh.subscribe("/darknet_ros/found_object", 1, count_callback);

    position3d_pub = nh.advertise<std_msgs::Float64MultiArray> ("send3dposition", 1);

    message_filters::TimeSynchronizer<sensor_msgs::Image, sensor_msgs::Image> sync(color_sub, depth_sub, 10);
    sync.registerCallback(boost::bind(&callback, _1, _2));

    ros::spin();

    return 0;
}




// string getColor(cv::Vec3b bgrPixel)
// {
//     cv::Mat3b bgr;
//     bgr.push_back(bgrPixel);

//     cv::Mat3b hsv;
//     cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);

//     cv::Vec3b hsvPixel = hsv.at<cv::Vec3b>(0,0);

//     int hue = hsvPixel[0];
//     int saturation = hsvPixel[1];
//     int value = hsvPixel[2];

//     if ((hue < 10 || hue > 160) && saturation > 100 && value > 100)
//     {
//         return "Red";
//     }
//     else
//     {
//         return "Not Red";
//     }
// }


// int radius = 5; // The radius of the area around the center point to sample
//             std::vector<cv::Vec3b> rgb_values;
//             for (int dx = -radius; dx <= radius; ++dx)
//             {
//                 for (int dy = -radius; dy <= radius; ++dy)
//                 {
//                     int x = temp_boxes[i].cx + dx;
//                     int y = temp_boxes[i].cy + dy;
//                     if (x >= 0 && x < color_img->image.cols && y >= 0 && y < color_img->image.rows)
//                     {
//                         rgb_values.push_back(color_img->image.at<cv::Vec3b>(y, x));
//                     }
//                 }
//             }
//             cv::Vec3b avg_rgb(0, 0, 0);
//             for (const cv::Vec3b& rgb : rgb_values)
//             {
//                 avg_rgb[0] += rgb[0];
//                 avg_rgb[1] += rgb[1];
//                 avg_rgb[2] += rgb[2];
//             }
//             avg_rgb[0] /= rgb_values.size();
//             avg_rgb[1] /= rgb_values.size();
//             avg_rgb[2] /= rgb_values.size();
//             cv::Mat3b avg_rgb_mat(1, 1, avg_rgb);
//             cv::Mat3b avg_hsv_mat;
//             cv::cvtColor(avg_rgb_mat, avg_hsv_mat, cv::COLOR_BGR2HSV);

//             cv::Vec3b avg_hsv = avg_hsv_mat.at<cv::Vec3b>(0, 0);
//             int hue = avg_hsv[0];
//             int color_category;
//             if ((hue < 10) || (hue > 160)) // Red color range in HSV
//             {
//                 color_category = 1; // Red
//             }
//             else if ((hue > 60) && (hue < 130)) // Blue color range in HSV
//             {
//                 color_category = 2; // Blue
//             }
//             else if ((hue > 130) && (hue < 160)) // Purple color range in HSV
//             {
//                 color_category = 3; // Purple
//             }
//             else
//             {
//                 color_category = 0; // Unknown color
//             }
//             hsv_color.push_back(color_category);
