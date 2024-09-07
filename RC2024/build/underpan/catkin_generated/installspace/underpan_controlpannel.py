#!/usr/bin/env python3
# 2024/6/2 zfy


import rospy
from geometry_msgs.msg import Twist
import sys, select, termios, tty
import time

#print(msg)
msg = """
1234
"""
Omni = 0 #全向移动模式

#获取键值函数
def getKey():
    tty.setraw(sys.stdin.fileno())
    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)  #update speed == 0.1
    if rlist:
        key = sys.stdin.read(1)
    else:
        key = ''

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

def getTwist():
    gettwist = [0,0,0]


    return gettwist

#主函数
if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin) #获取键值初始化，读取终端相关属性
    
    rospy.init_node('under_vel_pan') #创建ROS节点
    pub = rospy.Publisher('~cmd_vel', Twist, queue_size=5) #创建速度话题发布者，'~cmd_vel'='节点名/cmd_vel'
    # termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)

    # zhe ge shi chuan ru shu zu update twist de zhi
    # while(1):
    #     speed = 0
    #     twist = Twist()
    #     rate =rospy.Rate(0.1)
    #     buffer = getTwist()
    #     twist.linear.x  = buffer[0]; twist.linear.y = buffer[1]; twist.linear.z = buffer[2]
    #     twist.angular.x = buffer[3];             twist.angular.y = buffer[4];                  twist.angular.z = buffer[5]
    #     pub.publish(twist) #ROS发布速度话题
    #     rate.sleep()

    
    while not rospy.is_shutdown():
        speed = 0
        twist = Twist() #创建ROS速度话题变量
        key = getKey()
        # if key == "1": #forward
        #     speed = int(key)
        #     # print("yidong")
        #     twist.linear.x  = speed; twist.linear.y = 0; twist.linear.z = 0
        #     twist.angular.x = 0;             twist.angular.y = 0;                  twist.angular.z = 0
        #     pub.publish(twist) #ROS发布速度话题
        #     time.sleep(1)
        if key == "w":
            twist.linear.x  = 0.3; twist.linear.y = 0; twist.linear.z = 0
            twist.angular.x = 0;             twist.angular.y = 0;                  twist.angular.z = 0
            pub.publish(twist) #ROS发布速度话题
            time.sleep(0.5)
        elif key == "s":
            twist.linear.x  = -1; twist.linear.y = 0; twist.linear.z = 0
            twist.angular.x = 0;             twist.angular.y = 0;                  twist.angular.z = 0
            pub.publish(twist) #ROS发布速度话题
            time.sleep(0.01)

        elif key == "i":
            twist.linear.x  = 0.5; twist.linear.y = 0; twist.linear.z = 0
            twist.angular.x = 0;             twist.angular.y = 1;                  twist.angular.z = 0.5
            pub.publish(twist) #ROS发布速度话题
            time.sleep(0.01)

        elif key == "a":
            twist.linear.x  = 0; twist.linear.y = 1; twist.linear.z = 0
            twist.angular.x = 0;             twist.angular.y = 0;                  twist.angular.z = 1
            pub.publish(twist) #ROS发布速度话题
            time.sleep(1)

        elif key == "d":
            twist.linear.x  = 0; twist.linear.y = -1; twist.linear.z = 0
            twist.angular.x = 0;             twist.angular.y = 0;                  twist.angular.z = -1
            pub.publish(twist) #ROS发布速度话题
            time.sleep(1)
        
        elif key == "k":
            rospy.signal_shutdown("kill the node")

        else:
            # print("mei you yi dong")
            control_speed = 0
        # print("fa bu le ")
            twist.linear.x  = control_speed; twist.linear.y = 0; twist.linear.z = 0
            twist.angular.x = 0;             twist.angular.y = 0;                  twist.angular.z = 0

            pub.publish(twist) #ROS发布速度话题

        print("shui da jiao")
    
        # rospy.spin()
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)