#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv) {

    ros::init(argc, argv, "state_publisher");
    ros::NodeHandle n;

    ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 100);
    tf::TransformBroadcaster broadcaster;
    ros::Rate loop_rate(30);


    // robot state
    double rueda = 0 ;

    // message declarations
    geometry_msgs::TransformStamped odom_trans;
    sensor_msgs::JointState joint_state;
    //odom_trans.header.frame_id = "base_link";
    //odom_trans.child_frame_id = " ";

    while (ros::ok()) {

        //update joint_state

        joint_state.header.stamp = ros::Time::now();
        joint_state.name.resize(4);
        joint_state.position.resize(4);
        joint_state.name[0] ="base_ruedaizqdel_joint";
        joint_state.position[0] = rueda;
        joint_state.name[1] ="base_ruedaderdel_joint";
        joint_state.position[1] = rueda;
        joint_state.name[2] ="base_ruedaizqtra_joint";
        joint_state.position[2] = rueda;
        joint_state.name[3] ="base_ruedadertra_joint";
        joint_state.position[3] = rueda;


        // update transform
        // (moving in a circle with radius=2)
        //odom_trans.header.stamp = ros::Time::now();
        //odom_trans.transform.translation.x = cos(angle)*2;
        //odom_trans.transform.translation.y = sin(angle)*2;
        //odom_trans.transform.translation.z = .7;
        //odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(angle+M_PI/2);

        //send the joint state and transform
        joint_pub.publish(joint_state);
        //broadcaster.sendTransform(odom_trans);

      

        // Ajusta lo necesario por iteracion
        loop_rate.sleep();
    }


    return 0;
}

