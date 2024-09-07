import Jetson.GPIO as GPIO
import time
import rospy

led_pin = 7
GPIO.setmode(GPIO.BOARD)
GPIO.setup(led_pin,GPIO.OUT)


def getball():
    a = 1
    




try:
    while 1:
        print("on")
        GPIO.output(led_pin,GPIO.HIGH)
        time.sleep(2)
        print("off")
        GPIO.output(led_pin,GPIO.LOW)
        time.sleep(2)
except KeyboardInterrupt:
    GPIO.output(led_pin,GPIO.LOW)
    GPIO.cleanup()
print("DONE")

