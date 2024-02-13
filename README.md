# higgs2_simulation
Simulation of UPM robot with Gazebo (Ignition) Citadel and ROS2 Foxy.

![image](https://github.com/aslab/HiggsModel/assets/121524021/7b4a862d-0edf-4cdc-933a-f2afa433663e)
![image](https://github.com/aslab/HiggsModel/assets/121524021/5d1d4360-8e4f-4c66-888d-d64c84a4d880)


## Install
Create workspace and clone this repo:

```
source /opt/ros/foxy/setup.bash
mkdir -p ~/rob_igni/src
cd ~/rob_igni/src
git clone https://github.com/aslab/HiggsModel.git higgs2
cd ..
rosdep update
rosdep install -y -r -q --from-paths src --ignore-src --rosdistro foxy 
colcon build --symlink-install
source install/setup.bash
```

## Launch the simulation
* Simulation with NAV2 navigation(known map). To change the map, go to ./src/higgs2/config/mapper_params_online_async.yaml and change map_file_name
```
 ros2 launch higgs2 launch_sim.launch.py  
```

* Simulation with NAV2 mapping(no map, save map when finished)
```
 ros2 launch higgs2 launch_sim_mapping.launch.py  

```

* Teleoperate the robot: (or with joystick)
```
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

## Launch the real robot
First make sure you clone the hardware interface, differential drive, lidar and serial communication:
```
cd ~/rob_igni/src
git clone https://github.com/AlvaroBarry/ros_arduino_bridge.git

git clone https://github.com/joshnewans/serial

git clone https://github.com/AlvaroBarry/diffdrive_arduino.git -b foxy

git clone https://github.com/ros-drivers/urg_node.git

cd ..
colcon build --symlink-install
source install/setup.bash

```

* Launch real robot with NAV2 and AMCL(select initial position) with known map. To change the map, go to ./src/higgs2/config/mapper_params_online_async_robot.yaml and change map_file_name

```
 ros2 launch higgs2 launch_robot.launch.py  
```

* If there is a problem with permission try this:

```
sudo chmod 666 /dev/ttyACM0
```
* Launch real robot with NAV2 and SLAM for mapping(no map, save map when finished)

```
 ros2 launch higgs2 launch_robot_mapping.launch.py  
```