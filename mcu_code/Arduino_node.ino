#define USB_CON
#include <ros.h>
#include <std_msgs/Char.h>
#include <std_msgs/String.h>


//四路红外模块,当测到障碍物对应口输出0,没有障碍物时为1；可通过调节电位计改变对应通道的测距范围，
//由此，调节1、2路电位计使其正常工作时输出0，遇到台阶时输出1；调节3、4路电位计使其正常工作时输出1，遇到障碍物时输出0
int D01 = 3;
int D02 = 4;    //防跌
int D03 = 5;
int D04 = 6;    //壁障
//RGB模块，对应接口为高电平时亮
int R = 7;
int G = 8;
int B = 9;
//蜂鸣器模块,接口为低电平时响
int Beep = 12;
//超声波传感器
int Trig = 26;
int echo = 28;
long duration, cm;

ros::NodeHandle n_;
std_msgs::Char msg;
std_msgs::String str;
char a[5] = "stop";
bool flag=false;
ros::Publisher pub_vel("cmd_char",&msg);
ros::Publisher pub_str("str",&str);
char c=' ';

void warn();

void setup() {
  n_.initNode();
  n_.advertise(pub_vel);
  n_.advertise(pub_str);
  Serial.begin(57600);  //communication with the host computer
//  Bluetooth Pins
//  BT VCC to Arduino 5V out. Disconnect before running the sketch
//  BT GND to Arduino GND
//  BT RX to Arduino TX1 (pin 18)
//  BT TX  to Arduino RX1   (pin 19)
  Serial1.begin(9600);  // communication with the BT module on serial1
  str.data = a;
  pinMode(D01, INPUT);
  pinMode(D02,INPUT);
  pinMode(D03,INPUT);
  pinMode(D04,INPUT);
  pinMode(Beep,OUTPUT);
  digitalWrite(Beep,1);
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(Trig,OUTPUT);
  pinMode(echo,INPUT);
  digitalWrite(R,0);
  digitalWrite(G,1);
  digitalWrite(B,0);
}

void loop() {
 //读取蓝牙数据
  if ( Serial1.available() )   
    {  
         c = Serial1.read();
    //   Serial.write( c );
    }
        
   if (c=='w' || c=='s' || c=='a' || c=='d' || c=='x')
     {msg.data = c;
       flag=true;
     }
   if (flag==true)
      { pub_vel.publish(&msg);
          flag=false;
      }

 
 
    digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  // The echo pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echo, HIGH);
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  //Serial.print("u");
  //Serial.print("\t");
 // Serial.print(cm);
  //Serial.print("\n");
 
    if (cm < 5)
      {  warn();
       pub_str.publish(&str); }
    n_.spinOnce();
    delay(100);
 
}

void warn() {
  digitalWrite(G,0);
  digitalWrite(B,1);
  digitalWrite(Beep,0);
  delay(2000);
  digitalWrite(G,1);
  digitalWrite(B,0);
  digitalWrite(Beep,1);
}

long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}


