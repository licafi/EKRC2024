;; Auto-generated. Do not edit!


(when (boundp 'depthcam::speed)
  (if (not (find-package "DEPTHCAM"))
    (make-package "DEPTHCAM"))
  (shadow 'speed (find-package "DEPTHCAM")))
(unless (find-package "DEPTHCAM::SPEED")
  (make-package "DEPTHCAM::SPEED"))

(in-package "ROS")
;;//! \htmlinclude speed.msg.html


(defclass depthcam::speed
  :super ros::object
  :slots (_X_speed _Y_speed _Z_speed ))

(defmethod depthcam::speed
  (:init
   (&key
    ((:X_speed __X_speed) 0.0)
    ((:Y_speed __Y_speed) 0.0)
    ((:Z_speed __Z_speed) 0.0)
    )
   (send-super :init)
   (setq _X_speed (float __X_speed))
   (setq _Y_speed (float __Y_speed))
   (setq _Z_speed (float __Z_speed))
   self)
  (:X_speed
   (&optional __X_speed)
   (if __X_speed (setq _X_speed __X_speed)) _X_speed)
  (:Y_speed
   (&optional __Y_speed)
   (if __Y_speed (setq _Y_speed __Y_speed)) _Y_speed)
  (:Z_speed
   (&optional __Z_speed)
   (if __Z_speed (setq _Z_speed __Z_speed)) _Z_speed)
  (:serialization-length
   ()
   (+
    ;; float32 _X_speed
    4
    ;; float32 _Y_speed
    4
    ;; float32 _Z_speed
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _X_speed
       (sys::poke _X_speed (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _Y_speed
       (sys::poke _Y_speed (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _Z_speed
       (sys::poke _Z_speed (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _X_speed
     (setq _X_speed (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _Y_speed
     (setq _Y_speed (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _Z_speed
     (setq _Z_speed (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get depthcam::speed :md5sum-) "7b206176458d8484913bad2dea5ffce8")
(setf (get depthcam::speed :datatype-) "depthcam/speed")
(setf (get depthcam::speed :definition-)
      "float32 X_speed
float32 Y_speed
float32 Z_speed


")



(provide :depthcam/speed "7b206176458d8484913bad2dea5ffce8")


