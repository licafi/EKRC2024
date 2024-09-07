# !/usr/bin/env python
# coding=utf-8

import rospy
from sensor_msgs.msg import Image
import cv2, cv_bridge
import numpy
from geometry_msgs.msg import Twist
last_erro=0
def nothing(s):
    pass
col_black = (0,0,0,180,255,46)# black
col_white = (0,180,0,30,221,255)# white
col_red = (0,100,80,10,255,255)# red
col_blue = (100,40,40,130,255,255)# blue
col_green= (65,70,70,85,255,255)# green
col_yellow = (26,43,46,34,255,255)# yellow

cv2.namedWindow('Adjust_hsv',cv2.WINDOW_NORMAL)
Switch = '0:Red\n1:Green\n2:Blue\n3:Yellow\n4:Black'
cv2.createTrackbar(Switch,'Adjust_hsv',0,4,nothing)

mask=None

class Follower:
    def __init__(self):
        self.bridge = cv_bridge.CvBridge()
        #cv2.namedWindow("window", 1)
        # 订阅usb摄像头
        self.image_sub = rospy.Subscriber("/usb_cam/image_raw", Image, self.image_callback)
        self.image_sub = rospy.Subscriber("cv_bridge_image", Image, self.image_callback)
        # 订阅深度相机
        # self.image_sub = rospy.Subscriber("/camera/rgb/image_raw", Image, self.image_callback)
        # self.image_sub = rospy.Subscriber("/camera/depth/image_raw", Image,self.image_callback)
        self.cmd_vel_pub = rospy.Publisher("cmd_vel_ori", Twist, queue_size=1)
        self.twist = Twist()


    def image_callback(self, msg):
        global last_erro
        global mask
        image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        image = cv2.resize(image, (320,240), interpolation=cv2.INTER_AREA)#提高帧率
        # hsv将RGB图像分解成色调H，饱和度S，明度V
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        # 颜色的范围        
        # 第二个参数：lower指的是图像中低于这个lower的值，图像值变为0
        # 第三个参数：upper指的是图像中高于这个upper的值，图像值变为0
        # 而在lower～upper之间的值变成255
        kernel = numpy.ones((5,5),numpy.uint8)
        hsv_erode = cv2.erode(hsv,kernel,iterations=1)
        hsv_dilate = cv2.dilate(hsv_erode,kernel,iterations=1)
        m=cv2.getTrackbarPos(Switch,'Adjust_hsv')
        if m == 0:
            lowerbH=col_red[0]
            lowerbS=col_red[1]
            lowerbV=col_red[2]
            upperbH=col_red[3]
            upperbS=col_red[4]
            upperbV=col_red[5]
        elif m == 1:
            lowerbH=col_green[0]
            lowerbS=col_green[1]
            lowerbV=col_green[2]
            upperbH=col_green[3]
            upperbS=col_green[4]
            upperbV=col_green[5]
        elif m == 2:
            lowerbH=col_blue[0]
            lowerbS=col_blue[1]
            lowerbV=col_blue[2]
            upperbH=col_blue[3]
            upperbS=col_blue[4]
            upperbV=col_blue[5]
        elif m == 3:
            lowerbH=col_yellow[0]
            lowerbS=col_yellow[1]
            lowerbV=col_yellow[2]
            upperbH=col_yellow[3]
            upperbS=col_yellow[4]
            upperbV=col_yellow[5]
        elif m == 4:
            lowerbH=col_black[0]
            lowerbS=col_black[1]
            lowerbV=col_black[2]
            upperbH=col_black[3]
            upperbS=col_black[4]
            upperbV=col_black[5]
        else:
            lowerbH=0
            lowerbS=0
            lowerbV=0
            upperbH=255
            upperbS=255
            upperbV=255

        mask=cv2.inRange(hsv_dilate,(lowerbH,lowerbS,lowerbV),(upperbH,upperbS,upperbV))
        masked = cv2.bitwise_and(image, image, mask=mask)

        # 在图像某处绘制一个指示，因为只考虑20行宽的图像，所以使用numpy切片将以外的空间区域清空
        h, w, d = image.shape
        search_top = h-40
        search_bot = h
        mask[0:search_top, 0:w] = 0
        mask[search_bot:h, 0:w] = 0
        # 计算mask图像的重心，即几何中心
        M = cv2.moments(mask)
        if M['m00'] > 0:
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            cv2.circle(image, (cx, cy), 10, (255, 0, 255), -1)
            #cv2.circle(image, (cx-75, cy), 10, (0, 0, 255), -1)
            #cv2.circle(image, (w/2, h), 10, (0, 255, 255), -1)
            if cv2.circle:
            # 计算图像中心线和目标指示线中心的距离
                erro = cx - w/2-15
                d_erro=erro-last_erro
                self.twist.linear.x = 0.18
                if erro!=0:
                    self.twist.angular.z = -float(erro)*0.005-float(d_erro)*0.000
                else :
                    self.twist.angular.z = 0
                last_erro=erro
        else:
            self.twist.linear.x = 0
            self.twist.angular.z = 0
        self.cmd_vel_pub.publish(self.twist)
        #cv2.imshow("window", image)
        #cv2.imshow("window1", hsv)
        #cv2.imshow("window2", hsv_erode)
        #cv2.imshow("window3", hsv_dilate)
        #cv2.imshow("window4", mask)
        


# if __name__ == '__main__':
#     rospy.init_node("opencv")
#     follower = Follower()
#     while not rospy.is_shutdown():
#         if mask is not None:
#             cv2.imshow("Adjust_hsv", mask)
#             cv2.waitKey(1)
# import cv2
# import numpy as np
# import rospy
# from cv_bridge import CvBridge
# from sensor_msgs.msg import Image
# from std_msgs.msg import Header

# def neighbours(x, y, img):
#     return [
#         img[y-1, x], img[y-1, x+1], img[y, x+1], img[y+1, x+1],
#         img[y+1, x], img[y+1, x-1], img[y, x-1], img[y-1, x-1]
#     ]

# def transitions(n):
#     return sum((n[i] == 0 and n[(i + 1) % len(n)] == 1) for i in range(len(n)))

# def zhangSuenThinning(img):
#     hasChanged = True
#     while hasChanged:
#         hasChanged = False
#         toBeRemoved = []
#         for y in range(1, img.shape[0] - 1):
#             for x in range(1, img.shape[1] - 1):
#                 if img[y, x] == 255:
#                     n = neighbours(x, y, img)
#                     sum_ = sum(val == 255 for val in n)
#                     if 2 <= sum_ <= 6 and transitions(n) == 1 and (n[0] * n[2] * n[4] == 0) and (n[2] * n[4] * n[6] == 0):
#                         toBeRemoved.append((y, x))
#                         hasChanged = True
#         for y, x in toBeRemoved:
#             img[y, x] = 0
#         toBeRemoved.clear()
#         for y in range(1, img.shape[0] - 1):
#             for x in range(1, img.shape[1] - 1):
#                 if img[y, x] == 255:
#                     n = neighbours(x, y, img)
#                     sum_ = sum(val == 255 for val in n)
#                     if 2 <= sum_ <= 6 and transitions(n) == 1 and (n[0] * n[2] * n[6] == 0) and (n[0] * n[4] * n[6] == 0):
#                         toBeRemoved.append((y, x))
#                         hasChanged = True
#         for y, x in toBeRemoved:
#             img[y, x] = 0
#     return img

# def main():
#     rospy.init_node('img_publisher', anonymous=True)
#     pub = rospy.Publisher('camera/image', Image, queue_size=1)
#     cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
#     bridge = CvBridge()
#     rate = rospy.Rate(30)  # 30hz
#     while not rospy.is_shutdown():
#         ret, frame = cap.read()
#         if ret:
#             hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
#             lower_white = np.array([0, 0, 200])
#             upper_white = np.array([180, 255, 255])
#             mask = cv2.inRange(hsv, lower_white, upper_white)
#             binary = cv2.threshold(mask, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)[1]
#             img = binary.copy()
#             img = zhangSuenThinning(img)
#             h, w = binary.shape
#             last_erro, erro, d_erro = 0, 0, 0
#             search_top = h - 40
#             search_bot = h
#             mask[0:search_top, 0:w] = 0
#             mask[search_bot:h, 0:w] = 0
#             M = cv2.moments(mask)
#             if M['m00'] > 0:
#                 cx = int(M['m10'] / M['m00'])
#                 cy = int(M['m01'] / M['m00'])
#                 cv2.circle(binary, (cx, cy), 10, (0, 0, 255), -1)
#                 erro = cx - w / 2.0 - 15
#                 d_erro = erro - last_erro
#                 last_erro = erro
#             cv2.imshow('Thinned Image', img)
#             cv2.imshow('binary', binary)
#             cv2.waitKey(1)
#             msg = bridge.cv2_to_imgmsg(mask, encoding="mono8")
#             pub.publish(msg)
#         rate.sleep()

# if __name__ == '__main__':
#     main()