#include "ros/ros.h"
#include "nav_msgs/Odometry.h"

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  double x = msg->pose.pose.position.x;
  double y = msg->pose.pose.position.y;
  double z = msg->pose.pose.position.z;
  ROS_INFO("Received odom message: x: [%f], y: [%f], z: [%f]", x, y, z);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "odom_listener");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("/odom", 1000, odomCallback);

  ros::spin();
  return 0;
}