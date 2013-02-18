; Auto-generated. Do not edit!


(cl:in-package Saetta_Messages-msg)


;//! \htmlinclude cmd_vel.msg.html

(cl:defclass <cmd_vel> (roslisp-msg-protocol:ros-message)
  ((linear
    :reader linear
    :initarg :linear
    :type cl:float
    :initform 0.0)
   (angular
    :reader angular
    :initarg :angular
    :type cl:float
    :initform 0.0))
)

(cl:defclass cmd_vel (<cmd_vel>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <cmd_vel>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'cmd_vel)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name Saetta_Messages-msg:<cmd_vel> is deprecated: use Saetta_Messages-msg:cmd_vel instead.")))

(cl:ensure-generic-function 'linear-val :lambda-list '(m))
(cl:defmethod linear-val ((m <cmd_vel>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Saetta_Messages-msg:linear-val is deprecated.  Use Saetta_Messages-msg:linear instead.")
  (linear m))

(cl:ensure-generic-function 'angular-val :lambda-list '(m))
(cl:defmethod angular-val ((m <cmd_vel>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader Saetta_Messages-msg:angular-val is deprecated.  Use Saetta_Messages-msg:angular instead.")
  (angular m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <cmd_vel>) ostream)
  "Serializes a message object of type '<cmd_vel>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'linear))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angular))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <cmd_vel>) istream)
  "Deserializes a message object of type '<cmd_vel>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'linear) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angular) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<cmd_vel>)))
  "Returns string type for a message object of type '<cmd_vel>"
  "Saetta_Messages/cmd_vel")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'cmd_vel)))
  "Returns string type for a message object of type 'cmd_vel"
  "Saetta_Messages/cmd_vel")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<cmd_vel>)))
  "Returns md5sum for a message object of type '<cmd_vel>"
  "9d5c2dcd348ac8f76ce2a4307bd63a13")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'cmd_vel)))
  "Returns md5sum for a message object of type 'cmd_vel"
  "9d5c2dcd348ac8f76ce2a4307bd63a13")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<cmd_vel>)))
  "Returns full string definition for message of type '<cmd_vel>"
  (cl:format cl:nil "float32 linear~%float32 angular~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'cmd_vel)))
  "Returns full string definition for message of type 'cmd_vel"
  (cl:format cl:nil "float32 linear~%float32 angular~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <cmd_vel>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <cmd_vel>))
  "Converts a ROS message object to a list"
  (cl:list 'cmd_vel
    (cl:cons ':linear (linear msg))
    (cl:cons ':angular (angular msg))
))
