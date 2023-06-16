import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():

        package_name='urg_node'
        urg_params=os.path.join(get_package_share_directory(package_name),'launch','urg_node_ethernet.yaml')
        urg_node=Node(
            package='urg_node',
            executable='urg_node_driver',
            output='screen',
            parameters=[urg_params],
        )

        tf2_ros_node=Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            output='screen',
            arguments=['0', '0' ,'0' ,'0' ,'0' ,'0', 'base_link', 'laser'],
        )

        return LaunchDescription([
            urg_node,
            tf2_ros_node
        ])