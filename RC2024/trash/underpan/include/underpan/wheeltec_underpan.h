#ifndef __WHEELTEC_UNDERPAN_H_
#define __WHEELTEC_UNDERPAN_H_

#include "ros/ros.h"
#include <iostream>
#include <string.h>
#include <string> 
#include <iostream>
#include <math.h>
#include <stdlib.h>    
#include <unistd.h>      
#include <sys/types.h>
#include <sys/stat.h>
#include <serial/serial.h>
#include <fcntl.h>          
#include <stdbool.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <sensor_msgs/Imu.h>
#include <turn_on_wheeltec_robot/supersonic.h>
using namespace std;

//cout相关，用于打印带颜色的信息
#define RESET   string("\033[0m")
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define CYAN    "\033[36m"

//宏定义
#define SEND_DATA_CHECK   1          //Send data check flag bits //发送数据校验标志位
#define READ_DATA_CHECK   0          //Receive data to check flag bits //接收数据校验标志位
#define FRAME_HEADER      0X7B       //Frame head //帧头
#define FRAME_TAIL        0X7D       //Frame tail //帧尾
#define RECEIVE_DATA_SIZE 24         //The length of the data sent by the lower computer //下位机发送过来的数据的长度
#define SEND_DATA_SIZE    11         //The length of data sent by ROS to the lower machine //ROS向下位机发送的数据的长度
#define PI 				  3.1415926f //PI //圆周率

//自动回充相关
#define AutoCharge_HEADER      0X7C //Frame_header //自动回充数据帧头
#define AutoCharge_TAIL        0X7F //Frame_tail   //自动回充数据帧尾
#define AutoCharge_DATA_SIZE    8   //下位机发送过来的自动回充数据的长度


//Data structure for speed and position
//速度、位置数据结构体
typedef struct __Vel_Pos_Data_
{
	float X;
	float Y;
	float Z;
}Vel_Pos_Data;

//The structure of the ROS to send data to the down machine
//ROS向下位机发送数据的结构体
typedef struct _SEND_DATA_  
{
	    uint8_t tx[SEND_DATA_SIZE];
		float X_speed;	       
		float Y_speed;           
		float Z_speed;         
		unsigned char Frame_Tail; 
}SEND_DATA;

//The structure in which the lower computer sends data to the ROS
//下位机向ROS发送数据的结构体
typedef struct _RECEIVE_DATA_     
{
	    uint8_t rx[RECEIVE_DATA_SIZE];
	    uint8_t Flag_Stop;
		unsigned char Frame_Header;
		float X_speed;  
		float Y_speed;  
		float Z_speed;  
		float Power_Voltage;	
		unsigned char Frame_Tail;
}RECEIVE_DATA;

typedef struct _Distance_     
{
	float A;  
	float B;  
	float C;  
	float D;
}Supersonic_data;




//The robot chassis class uses constructors to initialize data, publish topics, etc
//机器人底盘类，使用构造函数初始化数据和发布话题等
class turn_on_underpan
{
	public:
		turn_on_underpan();  //Constructor //构造函数
		~turn_on_underpan(); //Destructor //析构函数
		void Control();   //Loop control code //循环控制代码
		serial::Serial Stm32_Serial; //Declare a serial object //声明串口对象 
	
	private:
		ros::NodeHandle wup;         //Create a ROS node handle //创建ROS节点句柄
		ros::Time _Now, _Last_Time;  //Time dependent, used for integration to find displacement (mileage) //时间相关，用于积分求位移(里程)
		float Sampling_Time;         //Sampling time, used for integration to find displacement (mileage) //采样时间，用于积分求位移(里程)

		ros::Subscriber Cmd_Vel_Sub; //Initialize the topic subscriber //初始化话题订阅者
		void Cmd_Vel_Callback(const geometry_msgs::Twist &twist_aux);  //The speed topic subscribes to the callback function //速度话题订阅回调函数

        unsigned char Check_Sum(unsigned char Count_Number,unsigned char mode); //BBC check function //BBC校验函数
      
		
        //Define the related variables 
        //定义相关变量
        string usart_port_name, robot_frame_id, gyro_frame_id, odom_frame_id,robot_type; 
        int serial_baud_rate;      //Serial communication baud rate //串口通信波特率
        SEND_DATA Send_Data;       //The serial port sends the data structure //串口发送数据结构体
        RECEIVE_DATA Receive_Data; //The serial port receives the data structure //串口接收数据结构体
        Vel_Pos_Data Robot_Pos;    //The position of the robot //机器人的位置
        Vel_Pos_Data Robot_Vel;    //The speed of the robot //机器人的速度
};
#endif