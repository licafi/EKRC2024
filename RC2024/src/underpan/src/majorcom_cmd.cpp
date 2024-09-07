#include <ros/ros.h>
#include <std_msgs/String.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <tf/transform_datatypes.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>
/**************************************
Date: 2024.06.28
ROBOCON2024_MAJORCONTROL_GLOBLEPRAMA
@AUTHOR: CATFISH QDXC
@VERSION: 1.0
@CARNAME: CATFISH_CAR
@LICENSE: CENTRAL SOUTH UNIVERSITY EK_TEAM
***************************************/
#define MAINLOOP_RATE 10
using namespace std;
ros::Publisher underpan_control;
ros::Publisher GPIO_control;
geometry_msgs::Twist ball_twist;
geometry_msgs::Twist line_twist;
geometry_msgs::Twist odom_twist;
geometry_msgs::Twist imu_twist;
/**************************************
函数声明区域
***************************************/
void writeOdom(double rel_x, double rel_y, double rel_z);
void odomBACK();
void odomMoveHorizontal();
void odomMoveStraight();
void odomMoveRotate();
double Rotate2Range(double input);
void lineturn(const std::string& turnmode);
int readlinemap(); //line condition mode

int writeOdomflag = 1;  // 里程记写入标志 1直接目标 2队列目标
double waitToOdom[3] = {0, 0, 0}; // waiting to be fucked
double nowOdom[3] = {0, 0, 0};  // 用里程记更新当前的位置参数 0->x, 1->y, 2->z 
double targetOdom[3] = {0, 0, 0}, errorOdom[3] = {0, 0, 0};  // 目标位置参数 x, y, z 单位: m
double lastOdom[3] = {0, 0, 0};  // 上一次的位置参数 x, y, z
/**************************************
Date: 2024.06.29 
功能: for cmd test
***************************************/
char getKey()
{
    struct termios oldSettings, newSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    char key = '\0';
    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000; // Update speed in microseconds (0.1 seconds)

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0)
    {
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            read(STDIN_FILENO, &key, 1);
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    ROS_INFO("key: %c", key);
    return key-48;
}
/**************************************
Date: 2024.06.29
功能: GPIO信息发送 1吐球 2吸球 4全部关闭
***************************************/
void GPIO_send(int value)
{
    std_msgs::Int8 msg;
    msg.data = value;
    GPIO_control.publish(msg);
}
/**************************************
Date: 2024.06.28
功能: 找球然后撞过去
// ifsprintOnce:有没有接近球之后的冲刺1次，每次只能冲一次哦 1则是冲刺过了
***************************************/
bool ifsprintOnce = 0;          
void BallarrayCallback(const std_msgs::Float64MultiArray::ConstPtr& array)
{   
    geometry_msgs::Twist twist;
    // std_msgs::Int8 msg;
    // msg.data = 2;
    // GPIO_control.publish(msg);
    double z = array->data[0];
    double y = array->data[1];
    double c = array->data[2];
    double last_erro, erro, d_erro;
    ROS_INFO("z: %f, y: %f, color: %f", z, y, c);
    
    if (z>350 && ifsprintOnce==0)   // 在找球，还没冲刺
    {   
        // twist.linear.x = -0.58;
        // twist.angular.z = ((-y>0)?y:-y)*0.001;
        if(y>50)
        {
            twist.linear.x = -0.28;
            twist.linear.y = 0;
            twist.angular.z= -0.1;
        }
        else if (y<-50)
        {
            twist.linear.x = -0.28;
            twist.angular.z= 0.1;
            twist.linear.y = 0;
        }
        else
        {
            twist.linear.x = -0.28;
            twist.angular.z = 0;
            twist.linear.y = 0;
        }
        ROS_INFO("twist1: x: %f,  y: %f, z: %f", twist.linear.x, twist.linear.y, twist.angular.z);
    }
    else if ( z<400 && z!=-1 && ifsprintOnce==0 )  // 正在冲刺
    {   
        ROS_INFO("sprint");
        std_msgs::Int8 GPIO_msg;
        GPIO_msg.data = 2;
        GPIO_control.publish(GPIO_msg);
        twist.linear.x = -0.547;
        twist.linear.y= 0;
        twist.angular.z = 0;
        underpan_control.publish(twist);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        twist.linear.x = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        ifsprintOnce = 1;
        //ROS_INFO("twist2: x: %f,  y: %f, z: %f", twist.linear.x, twist.linear.y, twist.angular.z);
    }

    //ROS_INFO("ifsprintOnce: %d", ifsprintOnce);
    if(ifsprintOnce == 1)   // 冲过了不许再冲
    {
        twist.linear.x = 0;
        twist.linear.y= 0;
        twist.angular.z = 0;
        //ROS_INFO("twist3: x: %f,  y: %f, z: %f", twist.linear.x, twist.linear.y, twist.angular.z);
    }

    if( z == -1 && y == -1 && c == -1)  // 没找到球，停止
    {
        twist.linear.x = 0;
        twist.linear.y= 0;
        twist.angular.z = 0;
        //ROS_INFO("twist4: x: %f,  y: %f, z: %f", twist.linear.x, twist.linear.y, twist.angular.z);
    }
    //ROS_INFO("twist5: x: %f,  y: %f, z: %f", twist.linear.x, twist.linear.y, twist.angular.z);
    ball_twist = twist;
}

/**************************************
Date: 2024.07.03
功能: 寻线地图
// 首位必须填写1。 其他位填写转弯模式： 1，左转；2，右转；3，直行；4，停止 6寻线退出，7篮时找到十字路口。 多写个4保证安全
***************************************/
bool slowLineFollowFlag = 0;
//int linemap[] = {1 ,6, 6, 6, 2, 4, 6, 4, 4};
int linemap[] = {1, 7,7, 7, 7,7,7,7,7,7,7} ;
int readlinemap() //line condition mode
{
    int linemapIndex;
    linemapIndex = linemap[0]; 
    int value = linemap[linemapIndex];
    if(value == 3) slowLineFollowFlag = 1; // 直行区要减速哦
        else slowLineFollowFlag = 0;
    linemapIndex++;
    if(linemapIndex > 8)   // 用来保证在3区收集球之后接触到十字路口一直是“6”进入投篮
    {
        linemapIndex = 8;
    }
    linemap[0] = linemapIndex;
    ROS_INFO("linemapIndex: %d", linemapIndex);
    ROS_INFO("value: %d", value);
    return value;
}
/**************************************
Date: 2024.06.28
功能: 寻线控制和转向控制(开环转向)
***************************************/
string turnmode;
void lineturn(const std::string& turnmode)
{
    geometry_msgs::Twist twist;
    for(int i=0; i<6; i++)
    {
        if (turnmode == "left")
        {
            twist.angular.z = 0.948;
            twist.linear.x = 0;
            twist.linear.y = 0;
        }
        else if (turnmode == "right")
        {
            twist.angular.z = -0.948;
            twist.linear.x = 0;
            twist.linear.y = 0;
        }
        else if (turnmode == "straight")//减速
        {
            twist.angular.z = 0;
            twist.linear.x = 0.35;
            twist.linear.y = 0;
        }
        else if(turnmode == "stop")
        {
            twist.linear.x = 0;
            twist.linear.y = 0;
            twist.angular.z = 0;
            underpan_control.publish(twist);
            ROS_INFO("MAMBA OUT!!");
            ros::shutdown();
        }
        else if (turnmode == "continue")
        {
            twist.angular.z = 0;
            twist.linear.x = 0.64;
            twist.linear.y = 0;
        }
        else if(turnmode == "crossover")
        {
            twist.angular.z = -0.948;
            twist.linear.x = 0;
            twist.linear.y = 0;
        }
        else
        {
            twist.linear.x = 0;
            twist.linear.y = 0;
            twist.angular.z = 0;
        }
        underpan_control.publish(twist);
        ROS_INFO("turn_mode %s turn_COUNT %d", turnmode.c_str(), i);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}
/**************************************
Date: 2024.06.29
功能: 闭环控制linetracker
// islinecatchable 1为寻线到3区； 2为寻线到投篮处 0在循环中取消寻线标志
***************************************/
int islinecatchable = 1;
double line_last_erro, line_erro, line_d_erro;
void linearrayCallback(const std_msgs::Float64MultiArray::ConstPtr& array)
{
    geometry_msgs::Twist twist;
    double cx_line = array->data[0];
    double cy_line = array->data[1];
    double w_line = array->data[2];
    double crossmode = array->data[3];
    double en = array->data[4];
    
    // ROS_INFO("cx_line: %f, cy_line: %f, w_line: %f, crossmode: %f, en: %f", cx_line, cy_line, w_line, crossmode, en);
    ROS_INFO("last_erro: %f, erro: %f, d_erro: %f", line_last_erro, line_erro, line_d_erro);
    ROS_INFO("islinecatchable, %d",islinecatchable);
    if (crossmode == 0)
    {
        if (en == 1)
        {   
            islinecatchable = 1;
            line_erro = cx_line - w_line/2-15;
            line_d_erro = line_erro-line_last_erro;
            if (slowLineFollowFlag == 0) twist.linear.x = 0.64;  // 正常
                else twist.linear.x = 0.35; // 减速
            if (line_erro!=0)
            {
                twist.angular.z = -float(line_erro)*0.0006-float(line_d_erro)*0.000;
                twist.linear.y = -float(line_erro)*0.0002-float(line_d_erro)*0.000;
            }    
            else
            {
                twist.angular.z = 0;
                line_last_erro = line_erro;
            }
        }
        else
        { 
            twist.linear.x = 0;
            twist.angular.z = 0;
        }
        line_twist = twist;
    }
    else if (crossmode != 0 && islinecatchable==1)
    {
        ROS_INFO("crossmode: %f", crossmode);
        string turnmode;
        int line_condition;
        line_condition = readlinemap();
        switch (line_condition)
        {
            case 1:
                {   
                    turnmode = "left";
                    lineturn(turnmode);
                    break;
                }
            case 2:
                {
                    turnmode = "right";
                    lineturn(turnmode);
                }
            case 3:
                {   
                    turnmode = "straight";
                    lineturn(turnmode);
                    break;
                }
            case 4:
                {
                    turnmode = "stop";
                    lineturn(turnmode);
                    break;
                }
            case 5:
                {
                    turnmode = "continue";
                    lineturn(turnmode);
                    break;
                }
            // 退出主循环
            case 6:
                {
                    islinecatchable = -1;
                    turnmode = "waiting";
                    lineturn(turnmode);
                    break;
                }
            case 7:
                {
                    islinecatchable = 2;
                    turnmode = "waiting";
                    twist.linear.x = 0;
                    twist.linear.y = 0;
                    twist.angular.z = 0;
                    underpan_control.publish(twist);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    //lineturn(turnmode);
                    break;
                }
            case 8:
                {
                    turnmode = "crossover";
                    lineturn(turnmode);
                    islinecatchable = -1;
                    break;
                }
        }
    }
    if(en == 0)
    {
        islinecatchable = 0;
    }
}
/**************************************
Date: 2024.06.29
功能: 校准车头位置，可以在启动寻线前修正用
***************************************/
void lineAssist()
{
    geometry_msgs::Twist twist;
    twist.linear.x = 0;
    //twist.angular.z = -float(line_erro)*0.0008 - float(line_d_erro)*0.000;
    twist.linear.y = -float(line_erro)*0.001 - float(line_d_erro)*0.000;
    if(abs(twist.linear.y) < 0.07)
    {
        twist.linear.y = (twist.linear.y > 0) ? 0.07 : -0.07;
    }
    // if (abs(line_erro) > 200)
    //     twist.linear.y = (line_erro < 0) ? 0.1888 : -0.1888;  // Movement in y
    // else if (100<abs(line_erro) < 200)
    //     twist.linear.y = (line_erro< 0) ? 0.108 : -0.108;  // Movement in y
    // else if (abs(line_erro) < 100)
    //     twist.linear.y = (line_erro < 0) ? 0.108 : -0.108;  // Movement in y
    // ROS_INFO("line_erro: %f", line_erro);
    underpan_control.publish(twist);
}

/**************************************
Date: 2024.06.29 没有测试,有bug，需要看imu的方法，不能写while循环，否则不能去处理订阅
功能: 里程计控制
***************************************/
// 映射关系
double Rotate2Range(double input)
{
    double x = input;
    double y;
    if (x>90/0.468)
        y = -0.468/90 * x + 2;
    else if (x<-90/0.468)
        y =  -0.468/90 * x - 2;
    else
        y = 0.468/90 * x;
    return y;
}
//bool writeOdomflag = 1;  // 里程记写入标志
double Kp = 1.0;
double Ki = 0.0;
double Kd = 0.0;
// Path storage
std::vector<geometry_msgs::Pose> path;
// Error integral
double error_integral_x = 0.0;
double error_integral_y = 0.0;
// Last error for derivative calculation
double last_error_x = 0.0;
double last_error_y = 0.0;
bool pushFlag = 0;
//更新targetOdom为当前位置
void UpdateTargetOdom()
{
    ROS_INFO("UpdateTargetOdom");
    targetOdom[0] = nowOdom[0];
    targetOdom[1] = nowOdom[1];
    targetOdom[2] = nowOdom[2];
}
void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    
    lastOdom[0] = nowOdom[0]; lastOdom[1] = nowOdom[1]; lastOdom[2] = nowOdom[2]; // 记录上一次的位置参数
    double x = msg->pose.pose.position.x;
    double y = msg->pose.pose.position.y;
    double z = msg->pose.pose.orientation.z;

    nowOdom[0] = x; nowOdom[1] = y; nowOdom[2] = z;// 更新当前的位置参数
    // ROS_INFO("123");
    // ROS_INFO("x: %f, y: %f, z: %f", x, y, z);
    for(int i = 0; i < 3; i++)
    {   if(i < 2)
            errorOdom[i] = targetOdom[i] - nowOdom[i];
        else
            errorOdom[i] = targetOdom[i] - nowOdom[i];
    }
    if (pushFlag == 1)
    {
        geometry_msgs::Pose temp;
        temp.position.x = x;
        temp.position.y = y;
        temp.position.z = z;
        path.push_back(temp);
        ROS_INFO("PUSH PATH");
    }
}
bool startodom = 1;
void odomBACK()
{
    geometry_msgs::Twist control;
    double fix_speed = 0.68;
    double length = sqrt(pow((nowOdom[0] - targetOdom[0]), 2) + pow((nowOdom[1] - targetOdom[1]), 2));
    double sin = (targetOdom[1] - nowOdom[1]) / length;
    double cos = (targetOdom[0] - nowOdom[0]) / length;
    double tan = (sin/cos);
    control.linear.x = fix_speed * cos;
    //control.angular.z = fix_speed * tan;
    control.linear.y = fix_speed * sin + 0.18*sin/abs(sin);
    ROS_INFO("length: %f", length);
    ROS_INFO("targetOdom[0]: %f, targetOdom[1]: %f", targetOdom[0], targetOdom[1]);
    ROS_INFO("errorOdom[0]: %f, errorOdom[1]: %f", errorOdom[0], errorOdom[1]);
    if (length < 0.03)
    {
        control.linear.x = 0;
        control.linear.y = 0;
    }
    else if (length < 0.5)
    {
        control.linear.x = 0.48*cos * length;
        control.linear.y = 0.48*sin * length + 0.08*sin/abs(sin);
        //control.angular.z = (errorOdom[2] > 0) ? 0.35 : -0.35;
        if (length < 0.1)
        {
            control.angular.z = (errorOdom[2] > 0) ? 0.45 : -0.45;

        }
    }
    underpan_control.publish(control);
}
void odomMoveStraight()
{
    ROS_INFO("error_x: %f", errorOdom[0]);
    geometry_msgs::Twist twist;
    odomCallback;
    ROS_INFO("error_x: %f", errorOdom[0]);
    ROS_INFO("error_z: %f", errorOdom[2]);
    ROS_INFO("x: %f", nowOdom[0]);
    ROS_INFO("z: %f", nowOdom[2]);
    twist.linear.y = 0;  // No movement in y
    twist.linear.x = (errorOdom[0] > 0) ? 0.78 : -0.78;  // Movement in x
    ROS_INFO("twist.linear.x: %f", twist.linear.x);
    // if (abs(errorOdom[0]) < 0.04)
    // {   
    //     twist.linear.x = 0;
    // }
    //twist.angular.z = -errorOdom[2]*0.1;
    twist.angular.z = -(nowOdom[2] - lastOdom[2]);
    underpan_control.publish(twist);
}
void odomMoveHorizontal()
{
    geometry_msgs::Twist twist;
    odomCallback;
    ROS_INFO("error_y: %f", errorOdom[1]);
    ROS_INFO("error_z: %f", errorOdom[2]);
    ROS_INFO("y: %f", nowOdom[1]);
    ROS_INFO("z: %f", nowOdom[2]);
    //twist.linear.x = (errorOdom[0] > 0) ? 0.18 : -0.18;  // No movement in x
    twist.linear.y = (errorOdom[1] > 0) ? 0.78 : -0.78;  // Movement in y
    ROS_INFO("twist.linear.y: %f", twist.linear.y);
    // if (abs(errorOdom[1]) < 0.04)
    // {   
    //     twist.linear.y = 0;
    // }
    twist.angular.z = -errorOdom[2]*0.13;
    //twist.angular.z = -(nowOdom[2] - lastOdom[2])*1.9;
    underpan_control.publish(twist);
}
void odomMoveRotate()
{
    // 直接转吧
    geometry_msgs::Twist twist;
    ROS_INFO("error_z: %f", errorOdom[2]);
    ROS_INFO("z: %f", nowOdom[2]);
    //twist.linear.x = 0.1;
    //twist.angular.z = (errorOdom[2] > 0) ? 0.168 : -0.168;  // Movement in z
    twist.angular.z = -float(line_erro)*0.002 - float(line_d_erro)*0.000;
    if(abs(twist.angular.z) < 0.07)
    {
        twist.angular.z = (twist.angular.z > 0) ? 0.07 : -0.07;
    }
    ROS_INFO("twist.angular.z: %f", twist.angular.z);
    underpan_control.publish(twist);
}
void writeOdom(double rel_x = 0.0, double rel_y = 0.0, double rel_z = 0.0)
{
    if(writeOdomflag ==1)
    {
        targetOdom[0] = nowOdom[0] + rel_x;
        targetOdom[1] = nowOdom[1] + rel_y;
        targetOdom[2] = nowOdom[2] + Rotate2Range(rel_z);
        writeOdomflag = 0;
    }
    if(writeOdomflag ==2)
    {
        waitToOdom[0] = nowOdom[0] + rel_x;
        waitToOdom[1] = nowOdom[1] + rel_y;
        waitToOdom[2] = nowOdom[2] + Rotate2Range(rel_z);
        writeOdomflag = 0;
    }
}
void odomAlignlineFollow()
{   
    geometry_msgs::Twist twist;
    odomCallback;
    twist.linear.x = (errorOdom[0] > 0) ? 0.4566 : -0.4566;  // Movement in x
    if(abs(errorOdom[1])>0.05)
    {   
        //twist.linear.y = (errorOdom[1] > 0) ? 0.185 : -0.185; 
        twist.linear.y = -float(line_erro)*0.002 - float(line_d_erro)*0.000;
        //twist.angular.z = -float(line_erro)*0.0006-float(line_d_erro)*0.000;
    }
    //twist.linear.y = (errorOdom[1] > 0) ? 0.115 : -0.115;  // Movement in y
    //twist.linear.y = -float(line_erro)*0.001 - float(line_d_erro)*0.000;
    // twist.angular.z = (errorOdom[2] > 0) ? 0.106 : -0.106;  // Movement in z
    ROS_INFO("error_x: %f", errorOdom[0]);
    ROS_INFO("error_y: %f", errorOdom[1]);
    //ROS_INFO("error_z: %f", errorOdom[2]);
    ROS_INFO("x: %f", nowOdom[0]);
    ROS_INFO("y: %f", nowOdom[1]);
    //ROS_INFO("z: %f", nowOdom[2]);
    ROS_INFO("twist.linear.x: %f", twist.linear.x);
    ROS_INFO("twist.linear.y: %f", twist.linear.y);
    underpan_control.publish(twist);
}
//ros::Time start_time;
void reverseLine()
{
    geometry_msgs::Twist twist;
    odomCallback;
    twist.linear.x = (errorOdom[0] > 0) ? 0.4566 : -0.4566;  // Movement in x
    if(abs(errorOdom[1])>0.05)
    {   
        //twist.linear.y = (errorOdom[1] > 0) ? 0.185 : -0.185; 
        twist.linear.y = float(line_erro)*0.002 - float(line_d_erro)*0.000;
        //twist.angular.z = -float(line_erro)*0.0006-float(line_d_erro)*0.000;
    }
    //twist.linear.y = (errorOdom[1] > 0) ? 0.115 : -0.115;  // Movement in y
    //twist.linear.y = -float(line_erro)*0.001 - float(line_d_erro)*0.000;
    // twist.angular.z = (errorOdom[2] > 0) ? 0.106 : -0.106;  // Movement in z
    ROS_INFO("error_x: %f", errorOdom[0]);
    ROS_INFO("error_y: %f", errorOdom[1]);
    //ROS_INFO("error_z: %f", errorOdom[2]);
    ROS_INFO("x: %f", nowOdom[0]);
    ROS_INFO("y: %f", nowOdom[1]);
    //ROS_INFO("z: %f", nowOdom[2]);
    ROS_INFO("twist.linear.x: %f", twist.linear.x);
    ROS_INFO("twist.linear.y: %f", twist.linear.y);
    underpan_control.publish(twist);
}
/**************************************
Date: 2024.06.29 没有测试
功能: IMU控制 
//很难用，尽量不用吧
***************************************/
double temp_yaw;
double target_yaw, yaw_control_signal;
double yaw_erro, yaw_last_erro, yaw_d_erro, yaw_integral;
double imu_dt = 1 / MAINLOOP_RATE;
void imuCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
    // Convert the Quaternion to a yaw angle
    double read_yaw;
    read_yaw = msg->orientation.z;
    read_yaw = read_yaw * 180 / 1;  // Convert to degrees
    temp_yaw = read_yaw;
    
    yaw_erro = target_yaw - temp_yaw;
    yaw_integral += yaw_erro;
    if (fabs(yaw_erro-yaw_last_erro) < 0.01)  // 0.01 is the tolerance
    {
        yaw_integral = 0;
    }
    yaw_d_erro = yaw_erro - yaw_last_erro;

    yaw_control_signal = 0.008 * yaw_erro + 0.000 * yaw_integral + 0.000 * yaw_d_erro; // 不能用积分
    if ( (fabs(yaw_erro-yaw_last_erro) < 0.03) && abs(yaw_erro) < 2 )  // 零点漂移
    {
        yaw_control_signal = 0;
    }
    // ROS_INFO("yaw: %f", temp_yaw);
    // ROS_INFO("target_yaw: %f", target_yaw);
    // ROS_INFO("yaw_control_signal: %f", yaw_control_signal);
    // ROS_INFO("yaw_erro: %f, yaw_last_erro: %f, yaw_d_erro: %f, yaw_integral: %f", yaw_erro, yaw_last_erro, yaw_d_erro, yaw_integral);

    yaw_last_erro = yaw_erro;
}
void imuMoveStraight(double x_distance)
{
    
}
void imuMoveHorizontal(double y_distance)
{

}
bool enableImuturnFlag = 1;
bool imuCountFlag = 1;
void imumMoveRotate(string angle)
{
    ROS_INFO("IMUSTARTING");
    if(angle == "left90")
    {
        // 直接转吧
        geometry_msgs::Twist twist;
        if(imuCountFlag == 1)       // 只设置一次目标角度，这里需要考虑target>180的情况
        {
            target_yaw = temp_yaw + 125;
            imuCountFlag = 0;
            yaw_control_signal = 0.01;
        }
        ROS_INFO("target_yaw: %f", target_yaw);
        if(enableImuturnFlag == 1)
        {
            twist.angular.z = yaw_control_signal;
            underpan_control.publish(twist);
            if(yaw_control_signal == 0)
            {
                enableImuturnFlag = 0;
            }
            ROS_INFO("yaw_control_signal: %f, temp_yaw: %f, yaw_erro: %f", yaw_control_signal, yaw_erro, yaw_erro);
        }
        else if (enableImuturnFlag == 0)
        {
            twist.angular.z = 0;
            underpan_control.publish(twist); 
            enableImuturnFlag = 0;
            ROS_INFO("TURN ANGLE: %s IS DONE", angle.c_str());
        }
        // ROS_INFO("twist.angular.z: %f", twist.angular.z);
    }

    else if(angle == "right90")
    {
        geometry_msgs::Twist twist;
        if(imuCountFlag == 1)       // 只设置一次目标角度，这里需要考虑target>180的情况
        {
            target_yaw = temp_yaw - 125;
            imuCountFlag = 0;
            yaw_control_signal = 0.01;
        }
        ROS_INFO("target_yaw: %f", target_yaw);
        if(enableImuturnFlag == 1)
        {
            twist.angular.z = yaw_control_signal;
            underpan_control.publish(twist);
            if(yaw_control_signal == 0)
            {
                enableImuturnFlag = 0;
            }
            ROS_INFO("yaw_control_signal: %f, temp_yaw: %f, yaw_erro: %f", yaw_control_signal, yaw_erro, yaw_erro);
        }
        else if (enableImuturnFlag == 0)
        {
            twist.angular.z = 0;
            underpan_control.publish(twist); 
            enableImuturnFlag = 0;
            ROS_INFO("TURN ANGLE: %s IS DONE", angle.c_str());
        }
    }
    else if(angle == "turn180")
    {
         geometry_msgs::Twist twist;
        if(imuCountFlag == 1)       // 只设置一次目标角度，这里需要考虑target>180的情况
        {
            target_yaw = temp_yaw + 180;
            imuCountFlag = 0;
            yaw_control_signal = 0.01;
        }
        ROS_INFO("target_yaw: %f", target_yaw);
        if(enableImuturnFlag == 1)
        {
            twist.angular.z = yaw_control_signal;
            underpan_control.publish(twist);
            if(yaw_control_signal == 0)
            {
                enableImuturnFlag = 0;
            }
            ROS_INFO("yaw_control_signal: %f, temp_yaw: %f, yaw_erro: %f", yaw_control_signal, yaw_erro, yaw_erro);
        }
        else if (enableImuturnFlag == 0)
        {
            twist.angular.z = 0;
            underpan_control.publish(twist); 
            enableImuturnFlag = 0;
            ROS_INFO("TURN ANGLE: %s IS DONE", angle.c_str());
        }
    }
}
/**************************************
Date: 2024.07.03
功能:功能循环地图  
// 首位一定为1 （最好不用0,1作为case哦）
// 寻线到指定位置： 1
// 在指定位置找球： 2
// 找好球之后投篮： 10～～18
***************************************/
//int myMap[] = {1, 1, 255, 12, 103 ,102, 13, 255,14, 255, 255, 255};
int myMap[] = {1,101, 0,105,10, 254, 11, 12, 13, 254, 253, 14,  253, 15, 16, 17, 21, 14, 254, 255, 255, 255}; //测试放球
// int myMap[] = {1, 2, 255, 3, 0, 255, 255, 255, 255, 255};
int readmap() //condition mode
{
    int myMapIndex ;
    myMapIndex = myMap[0]; 
    int value = myMap[myMapIndex];
    myMapIndex++;
    // if(myMapIndex > 11)
    // {
    //     myMapIndex = 11;
    // }
    myMap[0] = myMapIndex;
    ROS_INFO("myMapIndex: %d", myMapIndex);
    ROS_INFO("value: %d", value);
    return value;
}
/**************************************
Date: 2024.06.29
功能: ROS节点和主循环
***************************************/
int main(int argc, char **argv)
{   
    ros::init(argc, argv, "majorcom_control");
    ros::NodeHandle nh("~");
    ros::Subscriber sub_ball = nh.subscribe("/send3dposition", 1, BallarrayCallback);
    ros::Subscriber sub_line = nh.subscribe("/sendlinefollow", 1, linearrayCallback);
    ros::Subscriber sub_odom = nh.subscribe("/odom", 1, odomCallback); 
    ros::Subscriber sub_imu = nh.subscribe("/imu", 1, imuCallback);    
    ROS_INFO("MAJOR_CONTROL_NODE IS OPEN");
    underpan_control = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5);
    GPIO_control = nh.advertise<std_msgs::Int8> ("cmd_GPIO", 5);

    // int condition_temp = getKey();
    // int condition = static_cast<int>(condition_temp);
    // ROS_INFO("orin_condition: %d", condition);
    int linecathablecount = 0;
    int condition = -1;
    bool ending_three = 0;
    int i = 0;
    ros::Rate rate(MAINLOOP_RATE);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    while (ros::ok())
    {
        geometry_msgs::Twist cmd_send;
        switch (condition)
        {   case -1:
                {
                    condition = readmap();
                    ROS_INFO("condition: %d", condition);
                    break;
                }
            case 0:
                {   
                    pushFlag = 1;
                    cmd_send = ball_twist;
                    ROS_INFO("Case 0: ball_twist");
                    if(ifsprintOnce == 1)
                    {
                        ROS_INFO("get ball");
                        pushFlag = 0;
                        condition = -1;
                    }
                    break;
                }
            case 1:
                {
                    cmd_send = line_twist;
                    ROS_INFO("Case 1: line_twist");
                    if(islinecatchable == 0)
                    {   
                        // if(linecathablecount < 3)
                        // {
                        //     cmd_send.angular.z=0.5;
                        //     underpan_control.publish(cmd_send);
                        //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                        //     linecathablecount++;
                        //     condition = 1;
                        // }
                        // else /乱寻线万恶之源
                            condition = 255;
                    }
                    else if(islinecatchable == -1)
                    {
                        condition = -1;

                    }
                    else
                    {
                        linecathablecount = 0;
                    }
                    break;
                }
            case 2:
                {
                    ROS_INFO("Case 2: Straighten the car's head");
                    // lineAssist();
                    // ROS_INFO("line_erro: %f",line_erro);
                    // if (abs(line_erro) < 0.03)
                    // {
                    //     reverseLine(2);
                    //     condition = -1;
                    // }
                    reverseLine();
                    condition = -1;
                    break;
                }
            case 3:
                {   
                    ROS_INFO("Case 2: readying to the Reverse patrol");
                    writeOdomflag = 1;
                    writeOdom(-1.5, 0, 0);
                    condition = -1;
                    geometry_msgs::Twist twist;
                    twist.linear.x = 0;
                    twist.linear.y = 0;
                    twist.angular.z = 0;
                    underpan_control.publish(twist);
                    this_thread::sleep_for(chrono::milliseconds(1000));
                    break;  
                }
            case 4:
                {
                    ROS_INFO("Case 3: Reverse patrol -1.5m");
                    ROS_INFO("targetOdom[0]: %f", targetOdom[0]);
                    ROS_INFO("targetOdom[1]: %f", targetOdom[1]);
                    odomAlignlineFollow();
                    if(abs(errorOdom[0]) < 0.03)
                        condition = -1;
                    break;
                }


            // 放球
            case 10:
                {
                    ROS_INFO("Case 10: line_back");
                    cmd_send = line_twist;
                    if(islinecatchable == 2) // 寻好线了,这里要和前面对应上
                    {   
                        condition = -1;
                    }
                    break;
                }
            case 11:
                {
                    // 简单测了五个框框的位置
                    ROS_INFO("Case 11: write the aimming box");
                    writeOdomflag = 1;
                    // writeOdom(1.1, 1.0, 0);
                    writeOdom(1.07, -0.28, 0);
                    // writeOdom(1, 0, 0);
                    // writeOdom(0.96, -0.3, 0);
                    // writeOdom(0.9, -1.18, 0);
                    ROS_INFO("targetOdom: %f", targetOdom[0]);
                    condition = -1;
                    break;
                }
            case 12:
                {
                    ROS_INFO("Case 12: write the crossing");
                    writeOdomflag = 2;
                    writeOdom(-0.5, 0, 0);  // 故意地靠前一点方便寻线
                    ROS_INFO("waitToOdom: %f", waitToOdom[0]);
                    condition = -1;
                    this_thread::sleep_for(chrono::milliseconds(100));
                    break;
                }
            case 13:
                {   
                    ROS_INFO("Case 13: moving horizontal");
                    ROS_INFO("targetOdom[0]: %f", targetOdom[0]);
                    ROS_INFO("targetOdom[1]: %f", targetOdom[1]);
                    odomMoveHorizontal();
                    if(abs(errorOdom[1]) < 0.03)
                    {    
                        std::this_thread::sleep_for(std::chrono::milliseconds(200));
                        condition = -1;
                    }
                    break;
                }
            case 14:
                {
                    ROS_INFO("Case 14: moving odomAlignlineFollow to boxes");
                    ROS_INFO("targetOdom[0]: %f", targetOdom[0]);
                    ROS_INFO("targetOdom[1]: %f", targetOdom[1]);
                    odomAlignlineFollow();
                    // if(abs(errorOdom[0]) < 0.6 && ending_three == 0)
                    // {
                    //     condition = 254;
                    // }
                    if(abs(errorOdom[0]) < 0.03)
                        condition = -1;
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    break;
                }
            case 15:
                {
                    ROS_INFO("Case 15: put ball get score");
                    geometry_msgs::Twist twist;
                    twist.linear.x = 0;
                    twist.linear.y = 0;
                    underpan_control.publish(twist);
                    int i =0;
                    for(i = 0; i < 10; i++)
                    {
                        GPIO_send(3);
                        std::this_thread::sleep_for(std::chrono::milliseconds(200));  
                    }
                    // GPIO_msg.data = 1;
                    // GPIO_control.publish(GPIO_msg);
                    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
                    condition = -1;
                    break;
                }
            case 16:
                {
                    ROS_INFO("Case 16: turn back to the cross");
                    ROS_INFO("16- targetodom: %f", targetOdom[0]);
                    targetOdom[0] = waitToOdom[0];
                    targetOdom[1] = waitToOdom[1];
                    targetOdom[2] = waitToOdom[2];
                    ROS_INFO("16+ targetodom: %f", targetOdom[0]);
                    geometry_msgs::Twist temp;
                    temp.linear.x = -0.28;
                    underpan_control.publish(temp);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200)); 

                    condition = -1;
                    break;
                }
            case 17:
                {
                    ROS_INFO("Case 17:back to a point ahead of crossing_point");
                    odomBACK();
                    if(abs(errorOdom[0]) < 0.07 && abs(errorOdom[1])<0.05)
                        {
                            condition = -1;
                            GPIO_send(4);
                        }
                    break;
                }
            case 18:
                {
                    i = 0;
                    condition = -1;
                    break;
                }
            case 19:
                {
                    ROS_INFO("Case 18: Reverse patrol -2.5s");
                    ROS_INFO("targetOdom[0]: %f", targetOdom[0]);
                    ROS_INFO("targetOdom[1]: %f", targetOdom[1]);
                    reverseLine();
                    ending_three = 0;
                    i++;
                    ROS_INFO("i: %d", i);
                    if(i>25)
                    {
                        i = 0;
                        condition = -1;
                    }

                    break;
                }



            case 20:
                {
                    ROS_INFO("20 testing turnning");
                    geometry_msgs::Twist twist;
                    for(int i=0; i<=3; i++)
                    {
                        twist.angular.z = 0.788;
                        twist.linear.x = 0;
                        underpan_control.publish(twist);
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                    condition = -1;
                    break;
                }
            case 21:
                {
                    ROS_INFO("21");
                    writeOdomflag = 1;
                    writeOdom(-0.5, 0, 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    condition = -1;
                    break;
                }
            case 98:
                {
                    ROS_INFO("Case 98: testing+++++++++");
                    imumMoveRotate(string("left90"));
                    ROS_INFO("Case 98: testing---------");
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    condition = 99;
                    break;
                }
            case 99:
                {
                    ROS_INFO("Case 99: testing+++++++++");
                    enableImuturnFlag = 1;
                    imuCountFlag = 1;
                    imumMoveRotate(string("right90"));
                    ROS_INFO("Case 99: testing---------");
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    condition = 98;
                    break;
                }
            case 100:
                {   

                    ROS_INFO("TAGET-ODOM X: %f, Y: %f, Z: %f", targetOdom[0], targetOdom[1], targetOdom[2]);
                    writeOdomflag = 1;
                    writeOdom(1.5, 0.75, 90);
                    ROS_INFO("TAGET-ODOM X: %f, Y: %f, Z: %f", targetOdom[0], targetOdom[1], targetOdom[2]);
                    condition = -1;
                    break;
                }
            case 101:
                {
                    ROS_INFO("*********101***********");
                    writeOdomflag = 1;
                    writeOdom(0, 0, 0);
                    condition = -1;
                    break;
                }
            case 102:
                {
                    ROS_INFO("Case 102: moving straight");
                    ROS_INFO("targetOdom[0]: %f", targetOdom[0]);
                    ROS_INFO("targetOdom[1]: %f", targetOdom[1]);
                    odomMoveStraight();
                    if(abs(errorOdom[0]) < 0.03)
                        condition = -1;
                    break;
                }
            case 103:
                {   
                    ROS_INFO("Case 103: moving horizontal");
                    ROS_INFO("targetOdom[0]: %f", targetOdom[0]);
                    ROS_INFO("targetOdom[1]: %f", targetOdom[1]);
                    odomMoveHorizontal();
                    if(abs(errorOdom[1]) < 0.03)
                        condition = -1;
                    break;
                }
            case 104:
                {   // 带islinecatchable别乱用
                    ROS_INFO("Case 103: moving rotate");
                    odomMoveRotate();
                    ROS_INFO("targetOdom[2]: %f", targetOdom[2]);
                    if(abs(errorOdom[2]) < 0.03)
                    {
                        islinecatchable = 1;
                        condition = -1;
                    }
                    break;
                }
            case 105:
                {
                    ROS_INFO("105");
                    odomBACK();
                    if(abs(errorOdom[0]) < 0.07 && abs(errorOdom[1])<0.05)
                        {
                            condition = -1;
                            GPIO_send(4);
                            geometry_msgs::Twist twist;                            
                            twist.linear.x = 0;
                            twist.linear.y = 0;
                            twist.angular.z = 0;
                            underpan_control.publish(twist);
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        }
                    break;
                }
            case 200:
                {
                    for(i = 0; i < 100; i++)
                    {   

                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        GPIO_send(3);
                    }
                    //GPIO_send(3);
                    condition = -1;
                    break;
                }
            case 253:
                {
                    ROS_INFO("253");
                    odomMoveRotate();
                    if(abs(errorOdom[2]) < 0.03)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        condition = -1;
                    }
                    //condition = -1;
                    break;
                }
            case 254:
                {
                    ROS_INFO("254");
                    // if(abs(errorOdom[0]) < 0.7)
                    // {
                    //     ending_three = 1;
                    // }
                    lineAssist();
                    if (abs(line_erro) < 10)
                    {   
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        condition = -1;
                    }
                    break;
                }
            case 255:
                {
                    cmd_send.linear.x=0; cmd_send.linear.y=0; cmd_send.angular.z=0;
                    underpan_control.publish(cmd_send);
                    ROS_INFO("WHAT CAN I SAY, MANBA OUT!");
                    ros::shutdown();
                    break;
                }
            default:
                {
                    cmd_send.linear.x=0; cmd_send.linear.y=0; cmd_send.angular.z=0;
                    underpan_control.publish(cmd_send);
                    ROS_INFO("WHAT CAN I SAY, MANBA OUT!");
                    ros::shutdown();
                    break;
                }
        }
        if (condition==1 || condition==0 || condition==10)
            {
                underpan_control.publish(cmd_send); //否则其他case会强制输出0
                ROS_INFO("SPEED: x: %f,  y: %f, z: %f", cmd_send.linear.x, cmd_send.linear.y, cmd_send.angular.z);
            }
        ROS_INFO("                          ");
        ROS_INFO("                          ");
        ROS_INFO("==========================");
        ros::spinOnce();
        rate.sleep();
    }
    ros::spin();
    return 0;
}