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
    double square;
};
std::vector<Box> temp_boxes;

int yolo_count;

ros::Publisher position2d;

void positionpub()
{
    ROS_INFO("positionpub");
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
                real_z.push_back(0);
                real_x.push_back(temp_boxes[i].cx);
                real_y.push_back(temp_boxes[i].cy);

                if(temp_boxes[i].name=="red")
                {
                    real_color.push_back(1);
                    if(abs(temp_boxes[i].cx) < min_x)
                    {
                        min_x = abs(temp_boxes[i].cx);
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
                    position2d.publish(position_array); // 发布
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
        temp_box.square = (box.xmax - box.xmin) * (box.ymax - box.ymin);
    }
    temp_boxes.push_back(temp_box);
    //void positionpub();

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
                real_z.push_back(temp_boxes[i].square);
                real_x.push_back(temp_boxes[i].cx);
                real_y.push_back(temp_boxes[i].cy);

                if(temp_boxes[i].name=="red")
                {
                    real_color.push_back(1);
                    if(abs(temp_boxes[i].cx) < min_x)
                    {
                        min_x = abs(temp_boxes[i].cx);
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
                //ROS_INFO("position_array.data[0]: %f", position_array.data[1]);
                if((position_array.data[1]!= 0))
                    position2d.publish(position_array); // 发布
        }
    temp_boxes.clear();
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
            position2d.publish(position_array); // 发布
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "get_position");
    ros::NodeHandle nh;
    ROS_INFO("get_image node started");

    ros::Subscriber yolo_bounding_boxes_sub = nh.subscribe("/darknet_ros/bounding_boxes", 1, block_callback);
    ros::Subscriber yolo_boxes_count_sub = nh.subscribe("/darknet_ros/found_object", 1, count_callback);

    position2d = nh.advertise<std_msgs::Float64MultiArray> ("send2dposition", 1);


    ros::spin();

    return 0;
}