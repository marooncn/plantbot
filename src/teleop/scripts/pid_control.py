#!/usr/bin/env python

import rospy
from std_msgs.msg import Float32 
from std_msgs.msg import Int16, Int64 
from std_msgs.msg import String 

class Pid(object):
  def __init__(self):
# Initialize the node
    rospy.init_node('pid_control', anonymous=True)
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
    self.l_error_int = 0.0
    self.l_target = 0.0
    self.r_pid_duration_prev = rospy.Time.now()
    self.rwheel_vel = 0.0
    self.r_error_int = 0.0
    self.r_target = 0.0
    
# Set parameters
    self.kp = rospy.get_param('Kp',500)
    self.ki = rospy.get_param('ki',200)
    self.kd = rospy.get_param('kd',10)
    self.rate = rospy.get_param('rate', 10)
    self.ticks_per_meter = rospy.get_param('ticks_per_meter', 4685)   #4685
    self.encoder_min = rospy.get_param('encoder_min', -4294967296) 
    self.encoder_max = rospy.get_param('encoder_max', +4294967296)
    self.encoder_low_wrap = rospy.get_param('encoder_low_wrap', 0.2*(self.encoder_max-self.encoder_min)+self.encoder_min)
    self.encoder_high_wrap = rospy.get_param('encode_high_wrap', 0.8*(self.encoder_max-self.encoder_max)+self.encoder_max)
    self.control_max = rospy.get_param('out_max', 255)
    self.control_min = rospy.get_param('out_min', -255)
# Subscrption and publishment message
    rospy.Subscriber('lwheel_vtarget', Float32, self.l_pid_func)
    rospy.Subscriber('rwheel_vtarget', Float32, self.r_pid_func)
    rospy.Subscriber('lwheel', Int64, self.l_distance)
    rospy.Subscriber('rwheel', Int64, self.r_distance)
    rospy.Subscriber('str', String, self.stop)
    self.l_speed_pub = rospy.Publisher('lwheel_vel', Float32, queue_size=10)
    self.r_speed_pub = rospy.Publisher('rwheel_vel', Float32, queue_size=10)
    self.l_control_pub = rospy.Publisher('left_wheel_control', Float32, queue_size=10)
    self.r_control_pub = rospy.Publisher('right_wheel_control', Float32, queue_size=10)
# Calculate the odom accroding to the encoder data from launchpad_node
  def l_distance(self, msg):
    self.l_enc = msg.data
    if self.l_enc < self.encoder_low_wrap and self.l_enc_prev > self.encoder_high_wrap:
      self.lwheel_number += 1
    if self.l_enc > self.encoder_high_wrap and self.l_enc_prev < self.encoder_low_wrap:
      self.lwheel_number -= 1
    self.l_enc_prev = self.l_enc
    self.lwheel_dis = (self.l_enc+self.lwheel_number*(self.encoder_max-self.encoder_min))/self.ticks_per_meter
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
    if self.lwheel_dis != self.lwheel_dis_prev:
      self.lwheel_vel = (self.lwheel_dis-self.lwheel_dis_prev)/self.l_dt
      self.l_speed_pub.publish(self.lwheel_vel)
      rospy.loginfo("The left wheel's current speed is %f m/s." % self.lwheel_vel)
      self.lwheel_dis_prev = self.lwheel_dis
      self.l_then = self.l_dt_duration
  def r_speed(self):
    self.r_dt_duration = rospy.Time.now() - self.r_then
    self.r_dt = self.r_dt_duration.to_sec()
    if self.rwheel_dis != self.rwheel_dis_prev:
      self.rwheel_vel = (self.rwheel_dis-self.rwheel_dis_prev)/self.r_dt
      self.r_speed_pub.publish(self.rwheel_vel)
      rospy.loginfo("The right wheel's current speed is %f m/s." % self.rwheel_vel)
      self.rwheel_dis_prev = self.rwheel_dis
      self.r_then = self.r_dt_duration
# Calculate the control variable accroding to the current speed and the target speed from wheel_speed node
  def l_pid_func(self, msg):
    self.l_pid_duration = rospy.Time.now() - self.l_pid_duration_prev
    self.l_pid_dt = self.l_pid_duration.to_sec()
    self.l_pid_duration_prev = rospy.Time.now()
    self.l_target = msg.data
    self.l_error = self.l_target - self.lwheel_vel
    self.l_error_int += self.l_error*self.l_pid_dt
    self.l_error_diff = self.l_error/self.l_pid_dt
    self.lwheel_control = 1.2*(self.kp*self.l_error + self.ki*self.l_error_int + self.kd*self.l_error_diff)
    if self.lwheel_control > self.control_max:
      self.lwheel_control = self.control_max
    if self.lwheel_control < self.control_min:
      self.lwheel_control = self.control_min
    self.l_control_pub.publish(self.lwheel_control)
  def r_pid_func(self, msg):
    self.r_pid_duration = rospy.Time.now() - self.r_pid_duration_prev
    self.r_pid_dt = self.r_pid_duration.to_sec()
    self.r_pid_duration_prev = rospy.Time.now()
    self.r_target = msg.data
    self.r_error = self.r_target - self.rwheel_vel
    self.r_error_int += self.r_error*self.r_pid_dt
    self.r_error_diff = self.r_error/self.r_pid_dt
    self.rwheel_control = self.kp*self.r_error + self.ki*self.r_error_int + self.kd*self.r_error_diff
    if self.r_target == self.l_target:  
# Right wheel response more rapidly than the left, So Use feedback to pay off the error in case of accumulation.
      self.r_error = self.lwheel_vel - self.rwheel_vel
      self.r_error_int += self.r_error*self.r_pid_dt
      self.r_error_diff = self.r_error/self.r_pid_dt
      self.rwheel_control += 5*(self.kp*self.r_error + self.ki*self.r_error_int + self.kd*self.r_error_diff)
    if self.rwheel_control > self.control_max:
       self.rwheel_control = self.control_max
    if self.rwheel_control < self.control_min:
       self.rwheel_control = self.control_min
    self.r_control_pub.publish(self.rwheel_control)
# stop
  def stop(self, msg):
    if msg.data == 'stop':
      self.lwheel_control = 0;
      self.rwheel_control = 0;
      self.l_control_pub.publish(self.lwheel_control)
      self.r_control_pub.publish(self.rwheel_control)
# Every subscriber has its own thread, So just do the loop
  def loop(self):
    self.r = rospy.Rate(self.rate)
    while not rospy.is_shutdown():
      self.r.sleep()

if __name__ == '__main__':
  do_pid = Pid()
  do_pid.loop()
