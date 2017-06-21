#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>
#define d 0.1  // d denotes the distance between both active wheels

double v_left=0.0;
double v_right=0.0;
double v_rx, v_ry, v_wx, v_wy, omega_r, theta, thetadot;

void l_callback(const std_msgs::Float32& msg)
{
  v_left = msg.data;
}
void r_callback(const std_msgs::Float32& msg)
{
  v_right = msg.data;
}
  

int main(int argc, char** argv) {
  ros::init(argc, argv, "odometry_publisher");
  
  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 10);
  tf::TransformBroadcaster odom_broadcaster;
  ros::Subscriber l_sub=n.subscribe("lwheel_vel", 10, l_callback);
  ros::Subscriber r_sub=n.subscribe("rwheel_vel", 10, r_callback); 
  
  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();
  ros::Rate r(10);
  double x = 0.0;
  double y = 0.0;
  double theta = 0.0;

  while(n.ok()) {
    ros::spinOnce();
 //calculate odometry
    current_time = ros::Time::now();
    double dt = (current_time-last_time).toSec();
   // calculate velocities in the robot frame (which are located in the center of rotation, e.g. base_link frame)
    v_rx = (v_right+v_left)/2;
    v_ry = 0;  // we have a non-holonomic constraint (for a holonomic robot, this is non-zero)
    omega_r = (v_right-v_left)/d;  
   // calculate Velocities in the odom frame (which is fixed-world and the same as the robot frame at the beginning.)
    v_wx = cos(theta)*v_rx-sin(theta)*v_ry;
    v_wy = sin(theta)*v_rx+cos(theta)*v_ry;
    thetadot = omega_r;
    x += v_wx*dt;
    y += v_wy*dt;
    theta += thetadot*dt;
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(theta);
 //publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;
 //send the transform
    odom_broadcaster.sendTransform(odom_trans);
  
 //publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";
    odom.child_frame_id = "base_link";
 //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;
 //set the velocity
    odom.twist.twist.linear.x = v_rx;
    odom.twist.twist.linear.y = v_ry;
    odom.twist.twist.angular.z = omega_r;
  //publish the message
    odom_pub.publish(odom);
    last_time = current_time;
    r.sleep();
    }
}
