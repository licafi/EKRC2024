#!/usr/bin/env python
# coding=utf-8
import rospy
import Jetson.GPIO as GPIO
import time


get_pin = 23
put_pin = 7

GPIO.setmode(GPIO.BOARD)
GPIO.setup(get_pin, GPIO.OUT)
GPIO.setup(put_pin, GPIO.OUT)

def getball():
    print("Getting ball")
    GPIO.output(get_pin, GPIO.HIGH)
    time.sleep(2)
    GPIO.output(get_pin, GPIO.LOW)

def putball():
    print("Putting ball")
    GPIO.output(put_pin, GPIO.HIGH)
    time.sleep(2)
    GPIO.output(put_pin, GPIO.LOW)



def main():
    rospy.init_node('putball', anonymous=True)
    try:
        while not rospy.is_shutdown():
            getball()
            time.sleep(2)
            putball()
            time.sleep(2)
            if rospy.is_shutdown() == 1:
                break
    except rospy.ROSInterruptException:
        pass
    finally:
        GPIO.output(get_pin, GPIO.LOW)
        GPIO.output(put_pin, GPIO.LOW)
        GPIO.cleanup()

if __name__ == '__main__':
    main()
