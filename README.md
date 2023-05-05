# higgs2_simulation
Simulation of UPM robot with Gazebo (Ignition) Citadel and ROS2 Foxy.

## Install
Create workspace and clone this repo:

```
source /opt/ros/foxy/setup.bash
mkdir -p ~/rob_igni/src
cd ~/rob_igni/src
git clone https://github.com/aslab/HiggsModel.git higgs2 -b higgs2_foxy_igni
cd ..
rosdep update
rosdep install -y -r -q --from-paths src --ignore-src --rosdistro foxy 
colcon build --symlink-install
source install/setup.bash
```

## Launch the simulation
* First execute the simulation
```
 ros2 launch higgs2 launch_sim.launch.py  
```

* Teleoperate the robot: (or with joystick)
```
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```
## Launch the real robot
First make sure you clone the hardware interface, differential drive  and serial communication:
```
git clone https://github.com/AlvaroBarry/ros_arduino_bridge.git

git clone https://github.com/joshnewans/serial

git clone https://github.com/AlvaroBarry/diffdrive_arduino.git -b foxy
```

* Launch real robot

```
 ros2 launch higgs2 launch_robot.launch.py  
```
* If there is a problem with permission try this:

```
sudo chmod 666 /dev/ttyACM0
```