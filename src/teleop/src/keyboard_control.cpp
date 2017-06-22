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

  ~TeleopRobot() {}     //析构函数，用于释放对象使用的资源，销毁对象的非static数据成员
  void keyboardLoop();
};

int kfd = 0;
struct termios cooked, raw;
  
void quit(int sig)
{
  tcsetattr(kfd, TCSANOW, &cooked);   /* 头文件为,#include <termios.h>, 用于设置终端参数,函数格式为：
int tcsetattr(int fd, int optional_actions, const struct termios *termios_p); 参数fd为终端的文件描述符，0为标准输入，1为标准输出，
2为标准错误；optional_actions取值TCSANOW表示不等数据传输完毕就立即改变属性；返回的结果保存在termios结构体中。  */
  exit(0);
}

int main(int argc, char ** argv)
{
  ros::init(argc, argv, "keyboard_control");
  TeleopRobot tpk;
  tpk.init();

  signal(SIGINT, quit);  /* 头文件为#include <signal.h>，Signals are generated when an event occurs that requires attention. It can be considered as a software version of a hardware interrupt. 
SIGINT – interrupt (i.e., Ctrl-C).表示程序中断的话调用就quit函数。 */
  tpk.keyboardLoop();
	
  return(0);
}

void TeleopRobot::keyboardLoop()
{
  char c;
  bool dirty = false;
  // get the console in raw mode
  tcgetattr(kfd,&cooked);  /* 头文件为,#include <termios.h>,用于获取终端的相关参数,函数格式为：
int tcgetattr(int fd, struct termios *termios_p); 参数fd为终端的文件描述符；返回的结果保存在termios结构体中。  */
  memcpy(&raw, &cooked, sizeof(struct termios));  /*头文件为#include<string.h>，作用为由src指向地址为起始地址的连续n个字节的数
  据复制到以dest指向地址为起始地址的空间内。函数格式为：void *memcpy(void*dest, const void *src, size_t n);  */
  raw.c_lflag &= (ICANON | ECHO);  //c_lflag：termios结构体中的参数，为本地模式标志，控制终端编辑功能。ICANON:使用标准输入模式,ECHO:显示输入字符
  // get the console in raw mode
  raw.c_cc[VEOL] = 1; /*c_cc[NCCS]:termios结构体中的参数，控制字符，用于保存终端驱动程序中的特殊字符，如输入结束符等。
  VEOL：附加的End-of-file字符，VEOF：End-of-file字符  */
  raw.c_cc[VEOF] = 2;

  tcsetattr(kfd, TCSANOW, &raw);
  
  puts("Reading from keyboard"); /* Prototype: int puts(const char *astring);
                                      Header File: stdio.h (C) or cstdio (C++) */
  puts("---------------------");
  puts("Use 'w/s to go forwad/back");
  puts("Use 'a/d' to turn left/right");
  puts("And use 'x' to stop");

  while(ros::ok())
  {
// get the next event from the keyboard
	if(read(kfd, &c, 1) < 0)  /* 头文件为 #include <unistd.h>，格式为：
ssize_t read(int fd, void *buf, size_t count); 
read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf, 
read returns the number of BYTES transferred successfully, or -1 on error  */
	{
	  perror("read():"); //头文件：#include <stdio.h>,函数perror()用于抛出最近的一次系统错误信息，其原型为： void perror(char *string);
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
         case KEYCODE_X:
	  puts("stop"); 
	  cmd.data = 'x';
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

