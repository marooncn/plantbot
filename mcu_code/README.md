在plantbot中，NUC作为核心控制器除了处理激光雷达信息、与上位机通信外，还负责处理串口传来的其它传感器数据，这些传感器数据由Arduino和Launchpad作为mcu
来进行获取，然后通过串口传向NUC。其中Launchapd连接电机驱动器（驱动）、编码器（测速）、超声波传感器（避障），Arduino连接蓝牙模块（控制）、红外模块（避障、防跌）、RGB灯（显示）、蜂鸣器（报警），为获得更准确的里程信息，还可以连接MPU6050与里程计一起作为robot_localization的输入。Launchpad_node.ino为Energia的程序，Arduino_node.ino为Arduino的程序，可以通过两个IDE拷到对应mcu中，Launchpad_node.ino调用了第三方库文件Messenger.h来处理串口数据，当Arduino连接MPU6050时也需要调用第三方库文件，库文件地址为：https://github.com/qboticslabs/Chefbot_ROS_pkg/tree/master/tiva_c_energia_code_final 。 库文件需要下载解压到IDE首选项下规定路径的库文件下。
test_speed.ino是测量PWM值对应的速度值，由Energia拷到Launchpad中，为减小运动空间，代码先测左轮，左轮测完后修改代码测量右轮，所得数值整理后可以作为速度分段控制（sec_control.py）的依据。
 
