#!/usr/bin/env python 
# -*- coding: UTF-8 -*- 

import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32

r = 32.5*(10e-3)
l = 120.715*(10e-3)
lv = 0
rv = 0

def callback(cmd):
  global lv, rv
  lv = (cmd.linear.x + l*cmd.angular.z)/r
  rv = (cmd.linear.x - l*cmd.angular.z)/r
 
def speed_converter():
  rospy.init_node('wheel_speed', anonymous=True)
  pub1 = rospy.Publisher('lwheel_vtarget', Float32, queue_size=10)
  pub2 = rospy.Publisher('rwheel_vtarget', Float32, queue_size=10)
  rate = rospy.Rate(10)
  while not rospy.is_shutdown():
    rospy.Subscriber('cmd_vel', Twist, callback)
    s1 = "The left wheel's target speed is %f m/s." % lv
    s2 = "The right wheel's target speed is %f m/s." % rv
    rospy.loginfo(s1)
    rospy.loginfo(s2)
    pub1.publish(lv)
    pub2.publish(rv) 
    rate.sleep()

if __name__ == '__main__':
  speed_converter()
