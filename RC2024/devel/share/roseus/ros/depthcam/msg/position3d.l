;; Auto-generated. Do not edit!


(when (boundp 'depthcam::position3d)
  (if (not (find-package "DEPTHCAM"))
    (make-package "DEPTHCAM"))
  (shadow 'position3d (find-package "DEPTHCAM")))
(unless (find-package "DEPTHCAM::POSITION3D")
  (make-package "DEPTHCAM::POSITION3D"))

(in-package "ROS")
;;//! \htmlinclude position3d.msg.html


(defclass depthcam::position3d
  :super ros::object
  :slots (_x3d _y3d _z3d ))

(defmethod depthcam::position3d
  (:init
   (&key
    ((:x3d __x3d) 0)
    ((:y3d __y3d) 0)
    ((:z3d __z3d) 0)
    )
   (send-super :init)
   (setq _x3d (round __x3d))
   (setq _y3d (round __y3d))
   (setq _z3d (round __z3d))
   self)
  (:x3d
   (&optional __x3d)
   (if __x3d (setq _x3d __x3d)) _x3d)
  (:y3d
   (&optional __y3d)
   (if __y3d (setq _y3d __y3d)) _y3d)
  (:z3d
   (&optional __z3d)
   (if __z3d (setq _z3d __z3d)) _z3d)
  (:serialization-length
   ()
   (+
    ;; int64 _x3d
    8
    ;; int64 _y3d
    8
    ;; int64 _z3d
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int64 _x3d
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _x3d (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _x3d) (= (length (_x3d . bv)) 2)) ;; bignum
              (write-long (ash (elt (_x3d . bv) 0) 0) s)
              (write-long (ash (elt (_x3d . bv) 1) -1) s))
             ((and (class _x3d) (= (length (_x3d . bv)) 1)) ;; big1
              (write-long (elt (_x3d . bv) 0) s)
              (write-long (if (>= _x3d 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _x3d s)(write-long (if (>= _x3d 0) 0 #xffffffff) s)))
     ;; int64 _y3d
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _y3d (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _y3d) (= (length (_y3d . bv)) 2)) ;; bignum
              (write-long (ash (elt (_y3d . bv) 0) 0) s)
              (write-long (ash (elt (_y3d . bv) 1) -1) s))
             ((and (class _y3d) (= (length (_y3d . bv)) 1)) ;; big1
              (write-long (elt (_y3d . bv) 0) s)
              (write-long (if (>= _y3d 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _y3d s)(write-long (if (>= _y3d 0) 0 #xffffffff) s)))
     ;; int64 _z3d
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _z3d (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _z3d) (= (length (_z3d . bv)) 2)) ;; bignum
              (write-long (ash (elt (_z3d . bv) 0) 0) s)
              (write-long (ash (elt (_z3d . bv) 1) -1) s))
             ((and (class _z3d) (= (length (_z3d . bv)) 1)) ;; big1
              (write-long (elt (_z3d . bv) 0) s)
              (write-long (if (>= _z3d 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _z3d s)(write-long (if (>= _z3d 0) 0 #xffffffff) s)))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int64 _x3d
#+(or :alpha :irix6 :x86_64)
      (setf _x3d (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _x3d (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; int64 _y3d
#+(or :alpha :irix6 :x86_64)
      (setf _y3d (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _y3d (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; int64 _z3d
#+(or :alpha :irix6 :x86_64)
      (setf _z3d (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _z3d (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;;
   self)
  )

(setf (get depthcam::position3d :md5sum-) "0c2f0543d8404bbb5cf654877be20b1f")
(setf (get depthcam::position3d :datatype-) "depthcam/position3d")
(setf (get depthcam::position3d :definition-)
      "int64 x3d
int64 y3d
int64 z3d
")



(provide :depthcam/position3d "0c2f0543d8404bbb5cf654877be20b1f")


