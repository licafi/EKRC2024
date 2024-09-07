; Auto-generated. Do not edit!


(cl:in-package depthcam-msg)


;//! \htmlinclude position3d.msg.html

(cl:defclass <position3d> (roslisp-msg-protocol:ros-message)
  ((x3d
    :reader x3d
    :initarg :x3d
    :type cl:integer
    :initform 0)
   (y3d
    :reader y3d
    :initarg :y3d
    :type cl:integer
    :initform 0)
   (z3d
    :reader z3d
    :initarg :z3d
    :type cl:integer
    :initform 0))
)

(cl:defclass position3d (<position3d>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <position3d>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'position3d)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name depthcam-msg:<position3d> is deprecated: use depthcam-msg:position3d instead.")))

(cl:ensure-generic-function 'x3d-val :lambda-list '(m))
(cl:defmethod x3d-val ((m <position3d>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader depthcam-msg:x3d-val is deprecated.  Use depthcam-msg:x3d instead.")
  (x3d m))

(cl:ensure-generic-function 'y3d-val :lambda-list '(m))
(cl:defmethod y3d-val ((m <position3d>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader depthcam-msg:y3d-val is deprecated.  Use depthcam-msg:y3d instead.")
  (y3d m))

(cl:ensure-generic-function 'z3d-val :lambda-list '(m))
(cl:defmethod z3d-val ((m <position3d>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader depthcam-msg:z3d-val is deprecated.  Use depthcam-msg:z3d instead.")
  (z3d m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <position3d>) ostream)
  "Serializes a message object of type '<position3d>"
  (cl:let* ((signed (cl:slot-value msg 'x3d)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'y3d)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'z3d)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 18446744073709551616) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <position3d>) istream)
  "Deserializes a message object of type '<position3d>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'x3d) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'y3d) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'z3d) (cl:if (cl:< unsigned 9223372036854775808) unsigned (cl:- unsigned 18446744073709551616))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<position3d>)))
  "Returns string type for a message object of type '<position3d>"
  "depthcam/position3d")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'position3d)))
  "Returns string type for a message object of type 'position3d"
  "depthcam/position3d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<position3d>)))
  "Returns md5sum for a message object of type '<position3d>"
  "0c2f0543d8404bbb5cf654877be20b1f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'position3d)))
  "Returns md5sum for a message object of type 'position3d"
  "0c2f0543d8404bbb5cf654877be20b1f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<position3d>)))
  "Returns full string definition for message of type '<position3d>"
  (cl:format cl:nil "int64 x3d~%int64 y3d~%int64 z3d~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'position3d)))
  "Returns full string definition for message of type 'position3d"
  (cl:format cl:nil "int64 x3d~%int64 y3d~%int64 z3d~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <position3d>))
  (cl:+ 0
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <position3d>))
  "Converts a ROS message object to a list"
  (cl:list 'position3d
    (cl:cons ':x3d (x3d msg))
    (cl:cons ':y3d (y3d msg))
    (cl:cons ':z3d (z3d msg))
))
