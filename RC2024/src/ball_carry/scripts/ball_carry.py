#!/usr/bin/env python
# coding=utf-8
import rospy
import Jetson.GPIO as GPIO
import time
from std_msgs.msg import Int8
from std_msgs.msg import String

get_pin = 23
put_pin = 7
GPIO.setmode(GPIO.BOARD)
GPIO.setup(get_pin, GPIO.OUT)
GPIO.setup(put_pin, GPIO.OUT)
#GPIO.output(get_pin, GPIO.HIGH)
#GPIO.output(put_pin, GPIO.HIGH)
GPIO.output(get_pin, GPIO.LOW)
GPIO.output(put_pin, GPIO.LOW)

def callback(msg):
    data = msg.data
    rospy.loginfo("data: %d", data)
    if data == 1:
        # rospy.loginfo("wolile")
        GPIO.output(get_pin, GPIO.LOW)
        GPIO.output(put_pin, GPIO.HIGH)
    elif data == 2:
        GPIO.output(get_pin, GPIO.HIGH)
        GPIO.output(put_pin, GPIO.LOW)
    elif data == 3:
        GPIO.output(get_pin, GPIO.HIGH)
        GPIO.output(put_pin, GPIO.HIGH)
    else:
        GPIO.output(get_pin, GPIO.LOW)
        GPIO.output(put_pin, GPIO.LOW)


if __name__ == '__main__':
    rospy.init_node('ball_carry', anonymous=True)
    

    rospy.Subscriber("/cmd_GPIO", Int8, callback)
    rospy.spin()
