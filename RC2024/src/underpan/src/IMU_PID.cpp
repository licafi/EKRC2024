#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <termios.h>
#include <stdio.h>
#include <sensor_msgs/Imu.h>
class PIDController {
public:
    PIDController(double kp, double ki, double kd)
        : kp(kp), ki(ki), kd(kd), previous_error(0), integral(0) {}

    double update(double error, double dt) {
        integral += error * dt;
        double derivative = (error - previous_error) / dt;
        double output = kp * error + ki * integral + kd * derivative;
        previous_error = error;
        return output;
    }

private:
    double kp, ki, kd;
    double previous_error;
    double integral;
};
void getimucallback(const sensor_msgs::Imu::ConstPtr& msg)
{
    ROS_INFO("IMU: [%f]", msg->linear_acceleration.x);
}

// 获取键值函数
char getKey() {
    struct termios oldt, newt;
    char ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

int main(int argc, char** argv) {
    PIDController pid(1.0, 0.0, 0.0);  // 这里的参数是PID控制器的参数，你可能需要根据你的具体需求来调整它们
    //double imu_data = getIMUData();  // 这里假设你有一个函数可以获取IMU的数据
    // double error = target_speed - imu_data;  // 这里假设你有一个目标速度
    // double dt = 0.1;  // 这里假设你的主循环的时间间隔是0.1秒
    // double control_output = pid.update(error, dt);
    ros::init(argc, argv, "under_vel_pan");
    ros::NodeHandle nh;

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5);
    ros::Subscriber cmdimu = nh.subscribe("/imu", 5, getimucallback);
    while (ros::ok()) {
        geometry_msgs::Twist twist;
        char key = getKey();

        if (key == 'w') {
            twist.linear.x = 0.5;
        } else if (key == 's') {
            twist.linear.x = -0.5;
        } else if (key == 'l') {
            twist.linear.x = 0.2;
            twist.angular.z = 1.3;
        } else if (key == 'j') {
            twist.linear.x = 0.2;
            twist.angular.z = -1.3;
        } else if (key == 'a') {
            twist.linear.y = 1;
        } else if (key == 'd') {
            twist.linear.y = -1;
        } else if (key == 'k') {
            ros::shutdown();
        } else {
            twist.linear.x = 0;
        }

        pub.publish(twist);
        ros::spinOnce();
    }

    return 0;
}