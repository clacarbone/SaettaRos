
(cl:in-package :asdf)

(defsystem "Saetta_Messages-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "cmd_vel" :depends-on ("_package_cmd_vel"))
    (:file "_package_cmd_vel" :depends-on ("_package"))
    (:file "odom" :depends-on ("_package_odom"))
    (:file "_package_odom" :depends-on ("_package"))
  ))