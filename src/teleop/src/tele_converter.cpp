#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/Char.h>

geometry_msgs::Twist cmd;
bool dirty = false;

void callback(const std_msgs::Char& msg)
{
  switch(msg.data)
	{
	 case 'w':
	  cmd.linear.x += 0.01;
	  dirty = true;
	  break;
	 case 's':
	  cmd.linear.x -= 0.01;
	  dirty = true;
	  break;
	 case 'a':
	  cmd.angular.z +=  0.1;
	  dirty = true;
	  break;
	 case 'd':
	  cmd.angular.z -= 0.1;
	  dirty = true;
	  break;
	}
}

int main(int argc, char **argv)
{

  cmd.linear.x = 0;
  cmd.linear.y = 0;
  cmd.linear.z = 0;
  cmd.angular.x = 0;
  cmd.angular.y = 0;
  cmd.angular.z = 0;

  ros::init(argc, argv, "tele_converter");
  ros::NodeHandle n_;
  ros::Publisher pub = n_.advertise<geometry_msgs::Twist>("cmd_vel",1);
  ros::Subscriber sub = n_.subscribe("cmd_char",1,callback);
  ros::Rate rate(10);
  while(ros::ok())
  {
    if(dirty == true)
    {
       pub.publish(cmd);
       dirty = false;
    }
    ros::spinOnce();
  }
  return 0;
}#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/Char.h>
#define max_vel 0.18
#define min_vel -0.18

geometry_msgs::Twist cmd;
bool dirty = false;

void callback(const std_msgs::Char& msg)
{
  switch(msg.data)
	{
	 case 'w':
	   cmd.linear.x = 0.15;
           cmd.angular.z =  0;
	   dirty = true;
	   break;
	 case 's':
	   cmd.linear.x = -0.15;
           cmd.angular.z =  0;
	   dirty = true;
	   break;
	 case 'a':
           cmd.linear.x = 0;
	   cmd.angular.z = -0.15;
	   dirty = true;
	   break;
	 case 'd':
           cmd.linear.x = 0;
	   cmd.angular.z = 0.15;
	   dirty = true;
	   break;
         case 'x':
	   cmd.linear.x = 0;
           cmd.angular.z = 0;
	   dirty = true;
	   break;
	}
  if(cmd.linear.x > max_vel)
     cmd.linear.x = max_vel;
  if(cmd.linear.x < min_vel)
     cmd.linear.x = min_vel;
}

int main(int argc, char **argv)
{

  cmd.linear.x = 0;
  cmd.linear.y = 0;
  cmd.linear.z = 0;
  cmd.angular.x = 0;
  cmd.angular.y = 0;
  cmd.angular.z = 0;

  ros::init(argc, argv, "tele_converter");
  ros::NodeHandle n_;
  ros::Publisher pub = n_.advertise<geometry_msgs::Twist>("cmd_vel",1);
  ros::Subscriber sub = n_.subscribe("cmd_char",1,callback);
  ros::Rate rate(10);
  while(ros::ok())
  {
    if(dirty == true)
    {
       pub.publish(cmd);
       dirty = false;
    }
    ros::spinOnce();
  }
  return 0;
}

