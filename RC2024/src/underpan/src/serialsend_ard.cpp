#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int8.h>
#include <iostream>
#include <chrono>
#include <thread>

// 创建串口对象
serial::Serial ser;

void fakeGPIO(const std_msgs::Int8::ConstPtr& msg)
{
    //int mode = msg->data;
    int mode = 0;
    if (mode == 1)
    {
        ser.write("a");
    }
    else if (mode == 2)
    {
        ser.write("b");
    }
    else if (mode == 3)
    {
        ser.write("c");
    }
    else if (mode == 4)
    {
        ser.write("d");
    }
    ROS_INFO("cnmb");
}

int main(int argc, char** argv) 
{
    // 初始化ROS节点
    ros::init(argc, argv, "serialsend_node");
    ros::NodeHandle nh;
    // 创建sub
    ros::Subscriber sub = nh.subscribe<std_msgs::Int8>("GPIO_cmd", 1, fakeGPIO);     
    //打开串口设备
    try
    {
        ser.setPort("/dev/ttyUSB0");
        ser.setBaudrate(9600);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }
    
    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port open");
    }else{
       return -1;
    }
    // ser.write("a");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    int i=0;
    ros::Rate loop_rate(50);
    while(ros::ok())
    {
        i++;
        ser.write("a");
        ros::spinOnce();
        loop_rate.sleep();
        //ROS_INFO("%s" , ser.read());
        //ser.flush();
        //serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //ROS_INFO("%d" , i);
    }

    
    // 设置运行频率
    

    ros::spin();
    return 0;
}