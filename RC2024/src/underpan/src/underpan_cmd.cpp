#include <ros/ros.h>
#include <std_msgs/String.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float64MultiArray.h>
#include <geometry_msgs/Twist.h>
#include "nav_msgs/Odometry.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <iostream>

bool turn_right_flag = 0;
bool turn_left_flag = 0;
bool turn_right_allow = 1;
bool turn_left_allow = 1;
bool line_follower_flag = 0;
bool forward_flag = 0;
bool turn=0;
double tp = 0;
double enn = 0;
bool en_odom = 1;
//double odomm[];  //mode , x, y, z mode = 1lock mode = 0 open
double target_x = 0.0;  // 目标x位置
double target_y = 0.0;  // 目标y位置
double target_z = 0.0;  // 目标角度，单位为弧度

using namespace std;
ros::Publisher underpan_control;
geometry_msgs::Twist twist;
void lineFollowerTurn();



int myMap[] = {1, 1, 2, 2, 2, 4};
int readmap() {
    int myMapIndex ;
    myMapIndex = myMap[0]; 
    int value = myMap[myMapIndex];
    myMapIndex++;
    if(myMapIndex > 5)
    {
        myMapIndex = 1;
    }

    myMap[0] = myMapIndex;
    ROS_INFO("myMapIndex: %d", myMapIndex);
    ROS_INFO("value: %d", value);
    return value;
}

void op_opom(){
    
}
void speedsend(const std_msgs::Float64MultiArray::ConstPtr& speed_array)
{
    twist.linear.x = speed_array->data[0];
    twist.linear.y = speed_array->data[1];
    twist.linear.z = speed_array->data[2];
    twist.angular.x = speed_array->data[3];
    twist.angular.y = speed_array->data[4];
    twist.angular.z = speed_array->data[5];
    underpan_control.publish(twist);
}

void BallarrayCallback(const std_msgs::Float64MultiArray::ConstPtr& array){
    //ROS_INFO("NODE_BALL");
    
    if (array->data.size() >= 3 && tp == 0) 
    {   
        op_opom();
        double z = array->data[0];
        double y = array->data[1];
        double c = array->data[2];
        double last_erro, erro, d_erro;
        ROS_INFO("z: %f", z);
        if (z > 350 && enn == 0)
        {
            if(y>50){
            twist.linear.x = 0.28;
            twist.angular.z= -0.1;
            }
            else if (y<-50){
            twist.linear.x = 0.28;
            twist.angular.z= 0.1;
            }
            else{
            twist.linear.x = 0.28;
            twist.angular.z = 0;
            twist.linear.y = 0;
            }
            // erro = 0.1*y;
            // d_erro = erro - last_erro;
            // twist.linear.x = 0.18;
            // if (erro != 0) {
            //     twist.linear.y = -erro * 0.005 - d_erro * 0.000;
            // } else {
            //     twist.linear.y= 0;
            //     }
            // last_erro = erro;
            // twist.linear.x = 0.18;
        }
        else if (z<350 && enn == 0)
        {
            twist.linear.x = 1;
            twist.linear.y= 0;
            twist.angular.z = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            enn = 1;
        }
        else{
            twist.linear.x = 0;
            twist.linear.y= 0;
            twist.angular.z = 0;
        }
        underpan_control.publish(twist);
    }
}

void linearrayCallback(const std_msgs::Float64MultiArray::ConstPtr& array)
{
    ROS_INFO("--------------------");
    ROS_INFO("NODE");
    
    if (array->data.size() >= 4 && tp == 0 ) 
    {
        double cx = array->data[0];
        double cy = array->data[1];
        double w = array->data[2];
        double cr = array->data[3];
        double en = array->data[4];
        double last_erro, erro, d_erro;
        if (cr == 0)
        {
            if (en != 0)
            {
                // erro = y;
                // d_erro = erro - last_erro;
                // twist.linear.x = 0.48;
                // if (erro != 0) {
                //     twist.angular.z = -erro * 0.005 - d_erro * 0.000;
                // } else {
                //     twist.angular.z = 0;
                //     }
                // last_erro = erro;
                erro = cx - w/2-15;
                d_erro=erro-last_erro;
                twist.linear.x = 0.35;
                if (erro!=0){
                    twist.angular.z = -float(erro)*0.005-float(d_erro)*0.000;
                }    
                else{
                    twist.angular.z = 0;
                    last_erro=erro;
                }
            }
            else
            { 
                twist.linear.x = 0;
                twist.angular.z = 0;
                ROS_INFO("erro: %f", erro);
            }
        underpan_control.publish(twist);
    }
        else
        {
            int flag;
            flag = readmap();
            ROS_INFO("flag: %d", flag);
            if (flag == 1)
            {
                turn_right_flag = 1;
                turn_left_flag = 0;
                forward_flag = 0;
                twist.linear.x = 0.2;
                underpan_control.publish(twist);
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                lineFollowerTurn();
                turn_right_flag = 0;
                ROS_INFO("TURN RIGHT");
            }
            else if (flag == 2)
            {
                turn_right_flag = 0;
                turn_left_flag = 1;
                forward_flag = 0;
                twist.linear.x = 0.2;
                underpan_control.publish(twist);
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                lineFollowerTurn();
                turn_left_flag = 0;
                ROS_INFO("TURN LEFT");
            }
            else if (flag == 3)
            {
                turn_right_flag = 0;
                turn_left_flag = 0;
                forward_flag = 1;
                lineFollowerTurn();
                forward_flag = 0;
                ROS_INFO("STRIGHT");
            }
            else if (flag == 4)
            {   
                turn = 1;
                turn_right_flag = 0;
                turn_left_flag = 0;
                forward_flag = 0;
                tp = 1;
                lineFollowerTurn();
                // int turncount = 15;
                // for(int i=0; i<turncount; i++)
                // {
                //     twist.linear.x = 0;
                //     twist.linear.y = 0.0;
                //     twist.linear.z = 0.0;
                //     twist.angular.x = 0.0;
                //     twist.angular.y = 0.0;
                //     twist.angular.z = 1.8;
                //     underpan_control.publish(twist);
                //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
                // }
                ROS_INFO("TURN TURN");
            }
        }
    
    // for(std::vector<double>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
    // {
    //     ROS_INFO("Received: %f", *it);
    // }
    }
}

void lineFollowerTurn()
{
    int turn_publish_count = 18;
    if(turn_left_flag==1 && turn_left_allow==1)
    {
        for(int i=0; i<turn_publish_count; i++)
        {
            twist.linear.x = 0.11;
            twist.linear.y = 0.0;
            twist.linear.z = 0.0;
            twist.angular.x = 0.0;
            twist.angular.y = 0.0;
            twist.angular.z = 1.1;
            underpan_control.publish(twist);
            ROS_INFO("turn_COUNT %d", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    else if(turn_right_flag==1 && turn_right_allow==1)
    {
        for(int i=0; i<turn_publish_count; i++)
        {
            twist.linear.x = 0.11;
            twist.linear.y = 0.0;
            twist.linear.z = 0.0;
            twist.angular.x = 0.0;
            twist.angular.y = 0.0;
            twist.angular.z = -1.1;
            underpan_control.publish(twist);
            ROS_INFO("turn_COUNT %d", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    else if(forward_flag==1)
    {
        for(int i=0; i<turn_publish_count; i++)
        {
            twist.linear.x = 0.20;
            twist.linear.y = 0.0;
            twist.linear.z = 0.0;
            twist.angular.x = 0.0;
            twist.angular.y = 0.0;
            twist.angular.z = 0;
            underpan_control.publish(twist);
            ROS_INFO("forward_flag %d", i);
            // ros::Duration(0.1).sleep();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    if(turn==1)
    {
        for(int i=0; i<30; i++)
        {
            twist.linear.x = 0.1;
            twist.linear.y = 0.0;
            twist.linear.z = 0.0;
            twist.angular.x = 0.0;
            twist.angular.y = 0.0;
            twist.angular.z = -1.5;
            underpan_control.publish(twist);
            ROS_INFO("turnturn %d", i);
            // ros::Duration(0.1).sleep();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
       tp = 1;
    }
    twist.linear.x = 0.0;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;
    twist.angular.x = 0.0;
    twist.angular.y = 0.0;
    twist.angular.z = 0.0;
    underpan_control.publish(twist);
}

void setOdomAim()
{

}
// void op_opom(){
//      // ... your existing code ...

//     // Read the values from odomm array
//     int mode = odomm[0];
//     double tx = odomm[1];
//     double ty = odomm[2];
//     double tz = odomm[3];

//     // Check if odomm[0] is 0 or 1
//     if (mode == 0)
//     {
        
//     }
//     else if (mode == 1)
//     {
//         // odomm[0] is 1, it means odom is read-only
//         // Add your code here to handle read-only odom
//     }

//     // ... your existing code ...
// }

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg){
    ROS_INFO("NODE_ODOM");
    double x = msg->pose.pose.position.x;
    double y = msg->pose.pose.position.y;
    double z = msg->pose.pose.position.z;
    ROS_INFO("Received odom message: x: [%f], y: [%f], z: [%f]", x, y, z);
    
    if(en_odom==1)
    {
        double error_x = target_x - x;
        double error_y = target_y - y;
        double error_z = target_z - z;

        if(error_x > 0.03)
        {
            twist.linear.x = 0.18;
            twist.linear.y = 0;
            twist.linear.z = 0;
            twist.angular.x = 0;
            twist.angular.y = 0;
            twist.angular.z = 0;
            underpan_control.publish(twist);
        }
        else if(error_x < -0.03)
        {
            twist.linear.x = -0.18;
            twist.linear.y = 0;
            twist.linear.z = 0;
            twist.angular.x = 0;
            twist.angular.y = 0;
            twist.angular.z = 0;
            underpan_control.publish(twist);
        }
        else if(error_y > 0.03)
        {
            twist.linear.x = 0;
            twist.linear.y = -0.18;
            twist.linear.z = 0;
            twist.angular.x = 0;
            twist.angular.y = 0;
            twist.angular.z = 0;
            underpan_control.publish(twist);
        }
        else if(error_y < -0.03)
        {
            twist.linear.x = 0;
            twist.linear.y = -0.18;
            twist.linear.z = 0;
            twist.angular.x = 0;
            twist.angular.y = 0;
            twist.angular.z = 0;
            underpan_control.publish(twist);
        }
        else if(error_z > 0.2)
        {
            twist.linear.x = 0;
            twist.linear.y = 0;
            twist.linear.z = 0;
            twist.angular.x = 0;
            twist.angular.y = 0;
            twist.angular.z = 0.18;
            underpan_control.publish(twist);
        }
        else if(error_z < -0.2)
        {
            twist.linear.x = 0;
            twist.linear.y = 0;
            twist.linear.z = 0;
            twist.angular.x = 0;
            twist.angular.y = 0;
            twist.angular.z = -0.18;
            underpan_control.publish(twist);
        }
        else
        {
            twist.linear.x = 0;
            twist.linear.y = 0;
            twist.linear.z = 0;
            twist.angular.x = 0;
            twist.angular.y = 0;
            twist.angular.z = 0;
            underpan_control.publish(twist);
            en_odom = 0;
        }
    }    
}


int main(int argc, char **argv)
{   
    ros::init(argc, argv, "underpan_control");
    ros::NodeHandle nh("~");
    ros::Subscriber sub_ball = nh.subscribe("/send3dposition", 1, BallarrayCallback);
    ros::Subscriber sub_line = nh.subscribe("/sendlinefollow", 1, linearrayCallback);
    ros::Subscriber sub = nh.subscribe("/odom", 1000, odomCallback);

    // ros::Publisher underpan_control = nh.advertise<geometry_msgs::Twist>("topic_name", 1);
    
    ROS_INFO("NODE IS OPEN");
    
    underpan_control = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5);
    
    //turn_right_flag = 1;
    //turn_left_flag = 1;
    //forward_flag = 1;
    //lineFollowerTurn();
    

    // ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5); // 创建速度话题发布者
    // lineFollowerTurn();
    
    ros::spin();
    return 0;
}



 // while (y < ctrl_odom[1]-0.3 || y>ctrl_odom[1]+0.3 )
        // {   
        //     double error_x = ctrl_odom[0] - x;
        //     double error_y = ctrl_odom[1] - y;
        //     double error_z = ctrl_odom[2] - z;
        //     ROS_INFO("ctrl_odom[1]: %f", y);
        //     if(error_y > 0.03)
        //     {
        //         if(error_z != 0)
        //         {
        //             twist.linear.x = 0;  twist.linear.y = 0.18;
        //             twist.angular.z = float(error_z)*0.05;
        //             underpan_control.publish(twist);
        //         }
        //         else
        //         {
        //             twist.linear.x = 0;  twist.linear.y = 0.18;
        //             twist.angular.z = 0;
        //             underpan_control.publish(twist);
        //         }
        //     }
        //     else if(error_y < -0.03)
        //     {
        //         if(error_z != 0)
        //         {
        //             twist.linear.x = 0;  twist.linear.y = -0.18;
        //             twist.angular.z = -float(error_z)*0.05;
        //             underpan_control.publish(twist);
        //         }
        //         else
        //         {
        //             twist.linear.x = 0;  twist.linear.y = -0.18;
        //             twist.angular.z = 0;
        //             underpan_control.publish(twist);
        //         }
        //     }
        //     // else
        //     // {
        //     // twist.linear.x = 0;  twist.linear.y = 0;  twist.angular.z = 0;
        //     // twist.linear.z = 0;  twist.angular.x = 0;  twist.angular.y = 0;
        //     // underpan_control.publish(twist);
        //     // en_odom = 0;
        //     // en_line = 0;
        //     // }
        //}
    //     if(abs(error_y) > 0.03)
    //     {   
    //         ROS_INFO("error_y: %f", error_y);
    //         ROS_INFO("error_z: %f", error_z);
    //         ROS_INFO("y: %f", y);
    //         ROS_INFO("z: %f", z);
    //         twist.linear.x = 0;  // No movement in x
    //         twist.linear.y = (error_y > 0) ? 0.18 : -0.18;  // Movement in y
    //         ROS_INFO("twist.linear.y: %f", twist.linear.y);
    //         twist.angular.z = -error_z;  // Correction in z
    //         //underpan_control.publish(twist);
    //     }
    //     // underpan_control.publish(twist);
    //     else if(error_x > 0.03)
    //     {
    //         if(error_z != 0)
    //         {
    //             twist.linear.x = 0;  twist.linear.y = 0.18;
    //             twist.angular.z = +float(error_z)*0.05;
    //             //underpan_control.publish(twist);
    //         }
    //         else
    //         {
    //             twist.linear.x = 0;  twist.linear.y = 0.18;
    //             twist.angular.z = 0;
    //             //underpan_control.publish(twist);
    //         }
    //     }
    //     else if(error_x < -0.03)
    //     {
    //         if(error_z != 0)
    //         {
    //             twist.linear.x = 0;  twist.linear.y = -0.18;
    //             twist.angular.z = -float(error_z)*0.05;
    //             //underpan_control.publish(twist);
    //         }
    //         else
    //         {
    //             twist.linear.x = 0;  twist.linear.y = 0.18;
    //             twist.angular.z = 0;
    //             //underpan_control.publish(twist);
    //         }
    //     }
    //     else
    //     {
    //         twist.linear.x = 0;  twist.linear.y = 0;  twist.angular.z = 0;
    //         twist.linear.z = 0;  twist.angular.x = 0;  twist.angular.y = 0;
    //         //underpan_control.publish(twist);
    //         en_odom = 0;
    //         en_line = 0;
    //     }
    //     underpan_control.publish(twist);
    // }