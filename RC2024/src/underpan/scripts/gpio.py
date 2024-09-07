# !/usr/bin/env python3
# coding=utf-8


import rospy
import Jetson.GPIO as GPIO
import time
import sys, select, termios, tty

# get_pin = 40
# put_pin = 35

# GPIO.setmode(GPIO.BOARD)
# GPIO.setup(get_pin, GPIO.OUT)
# GPIO.setup(put_pin, GPIO.OUT)
# GPIO.output(get_pin, GPIO.HIGH)
# GPIO.output(put_pin, GPIO.HIGH)

def getKey():
    tty.setraw(sys.stdin.fileno())
    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)  #update speed == 0.1
    if rlist:
        key = sys.stdin.read(1)
    else:
        key = ''

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

def getball():
    print("Getting ball")
    GPIO.output(get_pin, GPIO.HIGH)
    time.sleep(5)
    GPIO.output(get_pin, GPIO.LOW)

# def putball():
#     print("Putting ball")
#     GPIO.output(put_pin, GPIO.HIGH)
#     time.sleep(5)
#     GPIO.output(put_pin, GPIO.LOW)

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
if __name__=="__main__":
    get_pin = 40
    put_pin = 35

    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(get_pin, GPIO.OUT)
    GPIO.setup(put_pin, GPIO.OUT)
    GPIO.output(get_pin, GPIO.HIGH)
    GPIO.output(put_pin, GPIO.HIGH)
    print("ok")
    settings = termios.tcgetattr(sys.stdin)
    try:
        while not rospy.is_shutdown():
            print("wtmd")
            key = getKey()
            GPIO.output(get_pin, GPIO.LOW)
            GPIO.output(put_pin, GPIO.LOW)
            time.sleep(5)
            print("cnmb")
            GPIO.output(get_pin, GPIO.HIGH)
            GPIO.output(put_pin, GPIO.HIGH)
            if key == "k":
                rospy.signal_shutdown("kill the node")
            #putball()
            #time.sleep(5)
    except KeyboardInterrupt:
        pass
    finally:
    # 恢复原来的终端属性
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)


# try:
#     while 1:
#         getball()

# except KeyboardInterrupt:
#     GPIO.output(led_pin,GPIO.LOW)
#     GPIO.cleanup()
# print("DONE")

# if __name__ == '__main__':
#     main()