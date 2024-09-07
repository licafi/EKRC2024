# !/usr/bin/env python3
# coding=utf-8


import rospy
import Jetson.GPIO as GPIO
import time


get_pin = 40
put_pin = 38

GPIO.setmode(GPIO.BOARD)
GPIO.setup(get_pin, GPIO.OUT)
GPIO.setup(put_pin, GPIO.OUT)

def getball():
    print("Getting ball")
    GPIO.output(get_pin, GPIO.HIGH)
    time.sleep(5)
    GPIO.output(get_pin, GPIO.LOW)

def putball():
    print("Putting ball")
    GPIO.output(put_pin, GPIO.HIGH)
    time.sleep(5)
    GPIO.output(put_pin, GPIO.LOW)

    # try:
    #     while not rospy.is_shutdown():
    #         getball()
    #         time.sleep(5)
    #         putball()
    #         time.sleep(5)
    # except rospy.ROSInterruptException:
    #     pass

# def main():
#     rospy.init_node('putball', anonymous=True)
#     try:
#         while not rospy.is_shutdown():
#             getball()
#             time.sleep(5)
#             putball()
#             time.sleep(5)
#     except rospy.ROSInterruptException:
#         pass
#     finally:
#         GPIO.output(get_pin, GPIO.LOW)
#         GPIO.output(put_pin, GPIO.LOW)
#         GPIO.cleanup()

try:
    while not rospy.is_shutdown():
        getball()
        time.sleep(5)
        #putball()
        #time.sleep(5)
except KeyboardInterrupt:
    pass


# try:
#     while 1:
#         getball()

# except KeyboardInterrupt:
#     GPIO.output(led_pin,GPIO.LOW)
#     GPIO.cleanup()
# print("DONE")

# if __name__ == '__main__':
#     main()