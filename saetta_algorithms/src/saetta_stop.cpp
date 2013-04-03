#define __GXX_EXPERIMENTAL_CXX0X__
#define LOOP_RATE	50 	// Hz
#include <ros/ros.h>
#include <saetta_msgs/cmd_vel.h>

int main( int argc, char** argv )
{
    

    ros::init(argc, argv, "Saetta_StopAll");
    ros::NodeHandle n;

    // Initialize node parameters from launch file or command line.
    // Use a private node handle so that multiple instances of the node can be run simultaneously
    // while using different parameters.
    ros::NodeHandle base_handle_("~");

    //base_handle_.param("rate", rate, int(40));
    //ros::Subscriber sub = n.subscribe ( "/saetta/velocity", 10, &Saetta_Base::listenerCallback, localbase );
    //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);


    // Tell ROS how fast to run this node.

    ros::Rate r(LOOP_RATE);
    ros::Publisher pubCmdVel1, pubCmdVel2, pubCmdVel3;
    pubCmdVel1 = n.advertise<saetta_msgs::cmd_vel > ("/robot_1/cmd_vel", 1);
    pubCmdVel2 = n.advertise<saetta_msgs::cmd_vel > ("/robot_2/cmd_vel", 1);
    pubCmdVel3 = n.advertise<saetta_msgs::cmd_vel > ("/robot_3/cmd_vel", 1);
    saetta_msgs::cmd_vel msg;
    msg.linear = 0;
    msg.angular = 0;

    while (n.ok())
    {        
	
    	pubCmdVel1.publish(msg);
	pubCmdVel2.publish(msg);
	pubCmdVel3.publish(msg);
	
        ros::spinOnce();
        r.sleep();
    }



}

