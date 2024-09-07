#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <ros/ros.h>
//#include <Jeston_gpio.h>

using namespace std;

class JetsonGPIO {
public:
    JetsonGPIO(string pin) : pinNumber(pin) {
        exportGPIO();
        usleep(100000); // 等待100毫秒
    }

    ~JetsonGPIO() {
        unexportGPIO();
    }

    void setDirection(string direction) {
        ofstream directionFile("/sys/class/gpio/gpio" + pinNumber + "/direction");
        if (directionFile.is_open()) 
        {
            directionFile << direction;
            directionFile.close();
        }
    }

    void setValue(string value) {
        ofstream valueFile("/sys/class/gpio/gpio" + pinNumber + "/value");
        if (valueFile.is_open()) {
            valueFile << value;
            valueFile.close();
        }
    }

private:
    string pinNumber;

    void exportGPIO() {
        ofstream exportFile("/sys/class/gpio/export");
        if (exportFile.is_open()) {
            exportFile << pinNumber;
            exportFile.close();
        }
    }

    void unexportGPIO() {
        ofstream unexportFile("/sys/class/gpio/unexport");
        if (unexportFile.is_open()) {
            unexportFile << pinNumber;
            unexportFile.close();
        }
    }
};

void getball() {
    JetsonGPIO gpio40("40"); // 假设40引脚的编号是40，根据实际情况修改
    gpio40.setDirection("out");
    gpio40.setValue("1");
    sleep(5); // 等待5秒
    gpio40.setValue("0");
}

void putball() {
    JetsonGPIO gpio38("38"); // 假设38引脚的编号是38，根据实际情况修改
    gpio38.setDirection("out");
    gpio38.setValue("1");
    sleep(5); // 等待5秒
    gpio38.setValue("0");
    ROS_INFO("0");

}

int main(int argc, char **argv) 
{
    // 测试getball和putball函数
    ros::init(argc, argv, "putball");
    ros::NodeHandle nh;
    //ros::Subscriber sub_ball = nh.subscribe("/send3dposition", 1, BallarrayCallback);
    getball();

    putball();

    return 0;
}
