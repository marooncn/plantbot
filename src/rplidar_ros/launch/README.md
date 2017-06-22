激光雷达运行hector_mapping绘图:

安装hector_slam：
sudo apt-get install ros-indigo-hector-slam
下载编译rplidar的驱动：
cd ~/plantbot/src 
git clone https://github.com/robopeak/rplidar_ros.git
在rplidar_ros/launch下新建hector_rplidar.launch
编译：
cd ..
catkin_make
插入激光雷达，检查对应的串口：
ls -l /dev |grep ttyUSB
结果为ttyUSB0，增加写的权限，设置串口权限：
sudo chmod 666 /dev/ttyUSB0
运行测试：
roslaunch rplidar_ros hector_rplidar.launch model:='$(find teleop)/urdf/plantbot.urdf'
在rviz中选择Fixed Frame为map,选择Map的Topic为/map
打开rqt_graph：
rosrun rqt_graph rqt_graph
绘图完成后，可以使用map_server保存地图：
roscd rplidar_ros
cd maps
rosrun map_server map_saver -f <map_name>
