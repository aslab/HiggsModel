import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command
from launch.actions import RegisterEventHandler
from launch.event_handlers import OnProcessStart
from launch_ros.descriptions import ParameterValue 
from launch_ros.actions import Node
use_sim_time = True

def generate_launch_description():
    pkg_ros_ign_gazebo = get_package_share_directory('ros_ign_gazebo')
    pkg = get_package_share_directory('higgs2')
    path_to_urdf = pkg+'/model/description/robot.urdf.xacro'
    
    ign_gazebo = IncludeLaunchDescription(
      PythonLaunchDescriptionSource(
      os.path.join(pkg_ros_ign_gazebo, 'launch', 'ign_gazebo.launch.py')),
    )


    state_publisher = Node(
    package='robot_state_publisher',
    executable='robot_state_publisher',
    parameters=[{
        'robot_description': ParameterValue(Command(['xacro ', str(path_to_urdf)]),
                                       value_type=str)}])

    rviz = Node(
        package='rviz2',
        executable='rviz2',
         parameters=[{'use_sim_time': use_sim_time}],
        arguments=[
            '-d',
            os.path.join(pkg, 'config', 'view_bot_nav2.rviz')
        ]
    )

  
    # Spawn
    spawn = Node(package='ros_ign_gazebo', executable='create',
                 arguments=[
                    '-name', 'higgs2_sim',
                    '-z', '0', #-0.11
                    '-y', '0', #-0.32
                    '-x', '2.8', #2.8
                    '-topic', '/robot_description'],
                 output='screen')



    ign_bridge = IncludeLaunchDescription(
		PythonLaunchDescriptionSource(
		    os.path.join(pkg, 'launch', 'bridge.launch.py'),),
    )
    
    # Joystick

    joystick = IncludeLaunchDescription(
                 PythonLaunchDescriptionSource([os.path.join(
                     pkg,'launch','joystick.launch.py'
                 )]), launch_arguments={'use_sim_time': 'true'}.items()
     )

    twist_mux_params = os.path.join(pkg,'config','twist_mux.yaml')
    twist_mux = Node(
            package="twist_mux",
            executable="twist_mux",
            parameters=[twist_mux_params, {'use_sim_time': True}],
            remappings=[('/cmd_vel_out','/cmd_vel')]
        )

    # Slamtoolbox
        
    slam = IncludeLaunchDescription(
		PythonLaunchDescriptionSource(
		    os.path.join(pkg, 'launch', 'online_async_launch.py'),),
        )

    # Nav2

    nav2 = IncludeLaunchDescription(
                PythonLaunchDescriptionSource([os.path.join(
                    pkg,'launch','navigation_launch.py'
                )]), launch_arguments={'use_sim_time': 'true'}.items()
    )

    
    
    
    return LaunchDescription([
       DeclareLaunchArgument(
           'ign_args',
             default_value=['-r ' + os.path.join(pkg, 'worlds', 'cave_world.sdf')]),
        joystick,
        twist_mux,
        ign_gazebo,
        spawn,
        ign_bridge,
        state_publisher,
        rviz,
        slam,
        nav2,
        
    ])