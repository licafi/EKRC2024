#!/usr/bin/env python
# 2024/6/5 shi jue zu

import rospy
from geometry_msgs.msg import Twist
import sys, select, termios, tty
import time
from std_msgs.msg import Float32MultiArray
import cv2, cv_bridge


# data.data[0] forward

# def image

def line_callback(turn,data):
    m0 = data.data[0]
    m1 = data.data[1]
    m2 = data.data[2]
    cx = data.data[3]
    cy = data.data[4]
    w = 1111#width
    if m0 < 10000:
        erro = cx - w/2-15
        d_erro=erro-last_erro
        twist.linear.x = 0.18
        if erro!=0:
            twist.angular.z = -float(erro)*0.005-float(d_erro)*0.000
        else :
            twist.angular.z = 0
            last_erro=erro
    else:
        with open('map', 'r') as file:
            for i, line in enumerate(file, 1):
                if i == turn:
                    twist.linear.x = line[1]
                    twist.angular.z = line[2]
                    turn = turn+1
                    break
    # twist = Twist() #创建ROS速度话题变量
    # while(data.data[0] == 1):

    #     rel = data.data[1]
    #     if rel > 0:
    #         twist.linear.x = 0.3
    #         twist.angular.z = 0.3
    #     else:
    #         twist.linear.x = 0.3
    #         twist.angular.z = -0.3

    #     twist.linear.x  = data.data[0] 
    #     twist.linear.y = data.data[1] 
    #     twist.linear.z = data.data[2]
    #     twist.angular.x = data.data[3]             
    #     twist.angular.y = data.data[4]                  
    #     twist.angular.z = data.data[5]
    rospy.INFO(data.data)    
    # return data.data

if __name__=="__main__":
    
    rospy.init_node('under_vel_pan') #创建ROS节点
    pub = rospy.Publisher('~cmd_vel', Twist, queue_size=5) #创建速度话题发布者，'~cmd_vel'='节点名/cmd_vel'
    twist = Twist()
    rospy.Subscriber('camera/image', Float32MultiArray, line_callback)
    # while not rospy.is_shutdown():
