#define __GXX_EXPERIMENTAL_CXX0X__
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
#define LOOP_RATE 50

bool publish = false;
float alive = 0;

void timerCallbackNuller(const ros::TimerEvent&)
{
    alive = 0;
}

void timerCallbackUpdater(const ros::TimerEvent&)
{
    publish = true;
}

void subscriberCallback(const std_msgs::Bool& msg)
{
    alive = 1;
}

int main(int argc, char** argv)
{


    ros::init(argc, argv, "RigidForm_Verifier");
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
    ros::Publisher pubStatus;
    ros::Subscriber subStatus;
    pubStatus = n.advertise<std_msgs::Float32>("/rigidform_status",1);
    subStatus = n.subscribe("rigidform_service", 1, subscriberCallback);
    ros::Timer timer1 = n.createTimer(ros::Duration(2.0), timerCallbackNuller);
    ros::Timer timer2 = n.createTimer(ros::Duration(0.52), timerCallbackUpdater);
    while (n.ok())
    {

        if(publish)
	{
	    std_msgs::Float32 newmsg;
	    newmsg.data = alive;
	    pubStatus.publish(newmsg);
	    publish=false;
	}

        ros::spinOnce();
        r.sleep();
    }



}

