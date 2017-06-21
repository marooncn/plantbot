#include "ros/ros.h"
#include "sstream"
#include <termios.h>
#include <signal.h>
#include <fcntl.h>

#include <std_msgs/Char.h>

/* #define UP 0x26
#define DOWN 0x28
#define LEFT 0x25
#define RIGHT 0x27 */
#define KEYCODE_A 0x61
#define KEYCODE_D 0x64
#define KEYCODE_S 0x73
#define KEYCODE_W 0x77

class TeleopRobot
{
  private:
  std_msgs::Char cmd;
  ros::NodeHandle n_;
  ros::Publisher pub;
  
  public:
  void init()
  {
    pub = n_.advertise<std_msgs::Char>("cmd_char",1);
    ros::NodeHandle n_private("~");
  }

  ~TeleopRobot() {}
  void keyboardLoop();
};

int kfd = 0;
struct termios cooked, raw;
  
void quit(int sig)
{
  tcsetattr(kfd, TCSANOW, &cooked);
  exit(0);
}

int main(int argc, char ** argv)
{
  ros::init(argc, argv, "keyboard_control");
  TeleopRobot tpk;
  tpk.init();

  signal(SIGINT, quit);
  tpk.keyboardLoop();
	
  return(0);
}

void TeleopRobot::keyboardLoop()
{
  char c;
  bool dirty = false;

  tcgetattr(kfd,&cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &= (ICANON | ECHO);
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;

  tcsetattr(kfd, TCSANOW, &raw);
  
  puts("Reading from keyboard");
  puts("---------------------");
  puts("Use 'W/S to forwad/back");
  puts("Use 'A/D' to left/right");

  while(ros::ok())
  {
	if(read(kfd, &c, 1) < 0)
	{
	  perror("read():");
	  exit(-1);
	}

        switch(c)
	{
	 case KEYCODE_W:
	  puts("speed");
	  cmd.data = 'w';
	  dirty = true;
	  break;
	 case KEYCODE_S:
	  puts("brake");
	  cmd.data = 's';
	  dirty = true;
	  break;
	 case KEYCODE_A:
	  puts("turn left");
	  cmd.data = 'a';
	  dirty = true;
	  break;
	 case KEYCODE_D:
	  puts("turn right"); 
	  cmd.data = 'd';
	  dirty = true;
	  break;
	}
	if(dirty == true)
	{
          pub.publish(cmd);
          dirty = false;
        }
  }
  
}
