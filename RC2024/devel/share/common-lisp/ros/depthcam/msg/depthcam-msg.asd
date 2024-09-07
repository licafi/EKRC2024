
(cl:in-package :asdf)

(defsystem "depthcam-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "position3d" :depends-on ("_package_position3d"))
    (:file "_package_position3d" :depends-on ("_package"))
    (:file "speed" :depends-on ("_package_speed"))
    (:file "_package_speed" :depends-on ("_package"))
  ))