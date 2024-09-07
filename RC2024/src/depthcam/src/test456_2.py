#! /usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image, CameraInfo
import message_filters
import cv2
import copy
from cv_bridge import CvBridge, CvBridgeError
from darknet_ros_msgs.msg import BoundingBoxes
from darknet_ros_msgs.msg import BoundingBox
from darknet_ros_msgs.msg import ObjectCount
from std_msgs.msg import Int8
from rcros.msg import position3d

yolo_count = 0
temp_box = []

# 215 240 35

def callback(data1,data2):
    global temp_box
    global yolo_count
    axis = position3d()
    bridge = CvBridge()
    color_image = bridge.imgmsg_to_cv2(data1, 'bgr8')
    depth_image = bridge.imgmsg_to_cv2(data2, '16UC1')
    det_box = [] # ensure the same flame of yolo and depth_img
    det_boxes = []
    if (yolo_count == 0):
        # rospy.loginfo("didn`t find any box!")
        axis.x3d = 0
        axis.y3d = 0
        axis.z3d = 0
        # new_image_pub.publish(axis)
    else:
        for i in range(len(temp_box)):
            # print(temp_box)
            real_z = depth_image[int(temp_box[i][2]),int(temp_box[i][1])] + 35
            real_x = int((temp_box[i][1] - ppx) / fx * real_z) + 215
            real_y = int((temp_box[i][2] - ppy) / fy * real_z) + 240
            # x,y,z should be judged and screened
            if (real_z<=5000):
                axis.x3d = int(real_x)
                axis.y3d = int(real_y)
                axis.z3d = int(real_z)
                new_image_pub.publish(axis)
                det_box.append(temp_box[i][0])
                det_box.append(real_x)
                det_box.append(real_y)
                det_box.append(real_z)
                det_boxes.append(copy.deepcopy(det_box))
                det_box.clear()
        rospy.loginfo(det_boxes)
        # if (det_boxes[i[3]]<=5000):
        # axis.x3d = int(real_x)
        # axis.y3d = int(real_y)
        # axis.z3d = int(real_z)
        # new_image_pub.publish(axis)
    # rospy.loginfo("---------------------")
    # pub1 = 1
    # pub2 = 2
    # pub3 = 3
    # rospy.loginfo("%d,%d,%d,",pub1,pub2,pub3)
    

def block_callback(msg):
    global temp_box

    yolo_name = []
    yolo_x = []
    yolo_y = []

    for boxes in msg.bounding_boxes:
        block_x =(boxes.xmin + boxes.xmax) / 2
        block_y =(boxes.ymin + boxes.ymax) / 2
        box_xmin =boxes.xmin
        box_xmax =boxes.xmax
        box_ymin =boxes.ymin
        box_ymax =boxes.ymax
        box_name = boxes.Class
        yolo_name.append(box_name)
        yolo_x.append(block_x)
        yolo_y.append(block_y)

    yolo_box = []
    yolo_box.append(yolo_name)
    yolo_box.append(yolo_x)
    yolo_box.append(yolo_y)

    box_1 = []
    temp_box.clear()
    for i in range(len(yolo_x)):
        box_1.append(yolo_name[i])
        box_1.append(yolo_x[i])
        box_1.append(yolo_y[i])
        temp_box.append(copy.deepcopy(box_1))
        box_1.clear()
    # print(temp_box)


def count_callback(msg):
    global yolo_count
    yolo_count = msg.data
    #rospy.loginfo("found_object_count: %d", msg.data)

 
if __name__ == '__main__':
    global fx, fy, ppx, ppy #相机内参
    fx = 606.3499755859375
    fy = 605.1943359375
    ppx = 328.8806457519531
    ppy = 249.58609008789062
    
    # node
    rospy.init_node('get_image', anonymous=True)
    
    # sub
    color = message_filters.Subscriber("/camera/color/image_raw", Image)
    depth = message_filters.Subscriber("/camera/aligned_depth_to_color/image_raw", Image)
    yolo_bounding_boxes = rospy.Subscriber("/darknet_ros/bounding_boxes", BoundingBoxes, block_callback)
    yolo_boxes_count = rospy.Subscriber("/darknet_ros/found_object", Int8, count_callback)


    # pub
    new_image_pub = rospy.Publisher("get_3dpositon", position3d, queue_size=1)
    color_depth = message_filters.TimeSynchronizer([color, depth], 1)  # 绝对时间同步
    #color_depth = message_filters.ApproximateTimeSynchronizer([color, depth], 10, 1, allow_headerless=True)  # 接近时间同步
    color_depth.registerCallback(callback)  
    rospy.loginfo("node_test is activated")
    rospy.spin()

    # color_depth = message_filters.ApproximateTimeSynchronizer([color, depth], 10, 1, allow_headerless=True)  # 接近时间同步
    # 同时订阅/camera/color/image_raw和/camera/aligned_depth_to_color/image_raw话题，并利用message_filters实现话题同步，共同调用callback
