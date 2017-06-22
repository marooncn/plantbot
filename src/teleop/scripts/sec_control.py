#!/usr/bin/env python

import rospy
from std_msgs.msg import Float32 
from std_msgs.msg import Int16, Int64 
from std_msgs.msg import String 


class Sec(object):
  def __init__(self):
 
    rospy.init_node('sec_control', anonymous=True)
# Set varibles
    self.l_enc = 0.0
    self.l_enc_prev = 0.0
    self.lwheel_number = 0
    self.lwheel_dis = 0.0
    self.r_enc = 0.0
    self.r_enc_prev = 0.0
    self.rwheel_number = 0
    self.rwheel_dis = 0.0
    self.l_then = rospy.Time.now()
    self.lwheel_dis_prev = 0.0
    self.r_then = rospy.Time.now()
    self.rwheel_dis_prev = 0.0
    self.l_error_int = 0.0
    self.l_pid_duration_prev = rospy.Time.now()
    self.lwheel_vel = 0.0
    self.r_error_int = 0.0
    self.r_pid_duration_prev = rospy.Time.now()
    self.rwheel_vel = 0.0
    self.r_error_int = 0.0
    self.lwheel_control = 0
    self.rwheel_control = 0
    self.l_number = 0
    self.r_number = 0
    self.l_flag = 1
    self.r_flag = 1
    self.left = (59.42, 61.05, 63.05, 65.78, 67.75, 69.16, 70.31, 71.63, 73.13, 74.15, 75.22, 76.50, 79.19, 80.00, 80.73, 82.18, 83.63, 84.82, 86.19, 87.90, 88.92, 89.05, 89.86, 91.31, 92.38, 93.83, 94.94, 95.58, 96.52, 97.89, 96.82, 99.04, 99.59, 100.49, 102.28, 103.27, 104.72, 106.13, 106.04, 107.07, 108.22, 108.52, 108.30, 110.61, 110.27, 110.27, 111.38, 111.85, 113.98, 114.24, 112.96, 112.70, 113.55, 116.37, 117.57, 118.76, 117.14, 117.74, 118.89, 120.38, 121.32, 122.09, 122.90, 123.16, 124.31, 125.04, 125.55, 126.02, 125.55, 125.34, 126.40, 126.92, 128.41, 128.11, 128.41, 129.52, 129.69, 130.16, 130.42, 131.31, 131.48, 132.51, 134.00, 133.40, 134.60, 134.77, 135.75, 136.05, 135.80, 136.39, 136.22, 136.31, 136.56, 136.99, 138.06, 138.36, 139.08, 139.09, 140.06, 141.13, 141.55, 141.62, 141.88, 141.94, 141.52, 141.13, 142.37, 143.56, 142.97, 142.97, 143.78, 144.12, 144.97, 145.57, 145.78, 145.66, 145.87, 146.21, 146.30, 147.07, 147.28, 147.58, 147.53, 148.09, 148.35, 148.73, 148.39, 148.86, 149.46, 149.28, 150.01, 150.22, 150.35, 151.12, 151.59, 151.63, 152.06, 152.15, 152.74, 152.36, 152.66, 152.79, 153.51, 153.77, 154.11, 153.68, 154.19, 154.71, 155.60, 155.60, 155.77, 155.99, 156.20, 156.41, 156.67, 156.41, 156.50, 156.54, 156.20, 156.97, 157.10, 157.52, 157.61, 157.87, 158.34, 158.93, 159.02, 159.74, 159.74, 159.96, 160.34, 160.43, 160.51, 160.60, 160.98, 160.94, 160.30, 160.64, 160.94, 161.41, 162.69, 164.31, 165.12)
    self.right = (59.89, 69.07, 69.20, 69.71, 70.57, 73.85, 76.07, 78.25, 79.87, 80.81, 81.92, 83.37, 84.70, 85.68, 86.66, 89.61, 90.59, 92.42, 93.40, 94.47, 96.05, 98.48, 99.77, 101.30, 101.60, 102.97, 103.78, 104.50, 106.04, 107.62, 108.22, 107.32, 109.11, 110.74, 110.69, 111.63, 113.17, 112.96, 111.97, 112.74, 113.77, 115.05, 115.47, 116.37, 118.16, 118.46, 117.82, 118.72, 120.26, 120.55, 121.02, 120.90, 122.35, 122.52, 123.80, 124.06, 124.35, 125.21, 125.98, 127.39, 127.51, 127.17, 128.11, 128.62, 128.84, 129.73, 130.07, 130.16, 129.73, 130.80, 131.14, 131.87, 131.87, 132.21, 132.72, 133.66, 133.87, 135.11, 134.94, 135.24, 135.41, 136.44, 136.99, 136.39, 136.99, 136.99, 137.59, 137.59, 138.44, 138.14, 137.80, 138.19, 138.61, 139.08, 139.12, 139.72, 139.89, 140.02, 139.94, 140.36, 140.53, 141.17, 141.56, 141.81, 142.37, 142.92, 143.95, 144.72, 145.23, 145.36, 145.31, 145.83, 145.96, 146.30, 146.94, 147.71, 147.79, 148.26, 147.96, 148.18, 148.05, 148.86, 148.43, 148.64, 149.11, 149.46, 149.50, 149.97, 150.18, 150.65, 150.95, 150.95, 151.50, 151.63, 151.80, 151.97, 152.32, 152.53, 152.02, 151.72, 152.15, 152.19, 151.80, 151.80, 152.87, 153.60, 153.04, 153.90, 153.64, 153.90, 153.72, 153.72, 153.98, 154.49, 154.75, 154.15, 154.41, 151.55, 151.55, 153.68, 153.68, 155.09, 155.05, 155.22, 155.65, 156.07, 156.80, 156.46, 156.07, 156.63, 156.97, 157.31, 157.27, 157.01, 157.69, 157.95, 157.31, 157.31, 157.27, 157.10, 157.48, 157.10, 157.57, 158.46, 159.70, 160.51, 160.77)
    self.rate = rospy.get_param('rate', 10)
    self.vel_threshold = rospy.get_param('~vel_threshold', 0.001)
    self.ticks_per_meter = rospy.get_param('ticks_per_meter', 4685)
    self.encoder_min = rospy.get_param('encoder_min', -4294967296) 
    self.encoder_max = rospy.get_param('encoder_max', +4294967296)
    self.encoder_low_wrap = rospy.get_param('encoder_low_wrap', 0.2*(self.encoder_max-self.encoder_min)+self.encoder_min)
    self.encoder_high_wrap = rospy.get_param('encode_high_wrap', 0.8*(self.encoder_max-self.encoder_max)+self.encoder_max)

    rospy.Subscriber('lwheel_vtarget', Float32, self.l_sec_func, queue_size=1)
    rospy.Subscriber('rwheel_vtarget', Float32, self.r_sec_func, queue_size=1)
    rospy.Subscriber('lwheel', Int64, self.l_distance, queue_size=1)
    rospy.Subscriber('rwheel', Int64, self.r_distance, queue_size=1)
    rospy.Subscriber('str', String, self.stop)
    self.l_speed_pub = rospy.Publisher('lwheel_vel', Float32, queue_size=10)
    self.r_speed_pub = rospy.Publisher('rwheel_vel', Float32, queue_size=10)
    self.l_control_pub = rospy.Publisher('left_wheel_control', Float32, queue_size=10)
    self.r_control_pub = rospy.Publisher('right_wheel_control', Float32, queue_size=10)

  def l_sec_func(self, msg):
    self.l_target = abs(1000* msg.data)
    if msg.data > 0:
      self.l_flag = 1
    elif msg.data < 0:
      self.l_flag = -1
    if self.l_target == 0:
      self.lwheel_control = 0
    elif self.l_target >= 165.12:
      self.lwheel_control = 255
    else:
      for i in self.left:
        if self.l_target <= i:
          self.lwheel_control = self.l_number + 72
          self.l_number = 0
          break
        self.l_number += 1
    self.lwheel_control = self.l_flag*self.lwheel_control
    self.l_control_pub.publish(self.lwheel_control)
  def r_sec_func(self, msg):
    self.r_target = abs(1000* msg.data)
    if msg.data > 0:
      self.r_flag = 1
    elif msg.data < 0:
      self.r_flag = -1
    if self.r_target == 0:
      self.rwheel_control = 0
    elif self.r_target >= 160.77:
      self.rwheel_control = 255
    else:
      for i in self.right:
        if self.r_target <= i:
          self.rwheel_control = self.r_number + 68
          self.r_number = 0
          break
        self.r_number += 1
    self.rwheel_control = self.r_flag*self.rwheel_control
    self.r_control_pub.publish(self.rwheel_control)
  # Calculate the odom accroding to the encoder data from launchpad_node
  def l_distance(self, msg):
    self.l_enc = msg.data
    if self.l_enc < self.encoder_low_wrap and self.l_enc_prev > self.encoder_high_wrap:
      self.lwheel_number += 1
    if self.l_enc > self.encoder_high_wrap and self.l_enc_prev < self.encoder_low_wrap:
      self.lwheel_number -= 1
    self.l_enc_prev = self.l_enc
    self.lwheel_dis = (float(self.l_enc+self.lwheel_number*(self.encoder_max-self.encoder_min)))/self.ticks_per_meter   #If not declare "float", the lwheel_dis's output will be integral and the lwheel_vel is often zero which is wrong. I waste much time here.
    rospy.loginfo("The left wheel's current lwheel_dis is %f m." % self.lwheel_dis)
    self.l_speed()
  def r_distance(self, msg):
    self.r_enc = msg.data
    if self.r_enc < self.encoder_low_wrap and self.r_enc_prev > self.encoder_high_wrap:
      self.rwheel_number += 1
    if self.r_enc > self.encoder_high_wrap and self.r_enc_prev < self.encoder_low_wrap:
      self.rwheel_number -= 1
    self.r_enc_prev = self.r_enc
    self.rwheel_dis = (self.r_enc+self.rwheel_number*(self.encoder_max-self.encoder_min))/self.ticks_per_meter
    self.r_speed()
# Calculate the current speed according to the odom
  def l_speed(self):
    self.l_dt_duration = rospy.Time.now() - self.l_then
    self.l_dt = self.l_dt_duration.to_sec()

    self.lwheel_vel = (self.lwheel_dis-self.lwheel_dis_prev)/self.l_dt
   
    rospy.loginfo("The left wheel's current speed is %f m/s." % self.lwheel_vel)
    rospy.loginfo("The left wheel's PWM data is %d" % self.lwheel_control)
    self.lwheel_dis_prev = self.lwheel_dis
    self.l_then = rospy.Time.now()
    self.l_speed_pub.publish(self.lwheel_vel)


  def r_speed(self):
    self.r_dt_duration = rospy.Time.now() - self.r_then
    self.r_dt = self.r_dt_duration.to_sec()

    self.rwheel_vel = (self.rwheel_dis-self.rwheel_dis_prev)/self.r_dt

    rospy.loginfo("The right wheel's current speed is %f m/s." % self.rwheel_vel)
    rospy.loginfo("The right wheel's PWM data is %d" % self.rwheel_control)
    self.rwheel_dis_prev = self.rwheel_dis
    self.r_then = rospy.Time.now()
    self.r_speed_pub.publish(self.rwheel_vel)

# stop
  def stop(self, msg):
    if msg.data == 'stop':
      self.lwheel_control = 0
      self.rwheel_control = 0
      self.l_control_pub.publish(self.lwheel_control)
      self.r_control_pub.publish(self.rwheel_control)

  def spin(self):
    self.r = rospy.Rate(self.rate)
    while not rospy.is_shutdown():
     # self.l_speed()
     # self.r_speed()
      self.r.sleep()

if __name__ == '__main__':
  do_sec = Sec()
  do_sec.spin()

