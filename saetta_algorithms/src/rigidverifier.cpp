#define __GXX_EXPERIMENTAL_CXX0X__
#include <ros/ros.h>
#include <std_msgs/Bool.h>
#define LOOP_RATE 50

bool publish = false, alive = false;

void timerCallbackNuller(const ros::TimerEvent&)
{
    alive = false;
}

void timerCallbackUpdater(const ros::TimerEvent&)
{
    publish = true;
}

void subscriberCallback(const std_msgs::Bool& msg)
{
    alive = true;
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
    pubStatus = n.advertise<std_msgs::Bool>("/rigidform_status",1);
    subStatus = n.subscribe("rigidform_service", 1, subscriberCallback);
    ros::Timer timer1 = n.createTimer(ros::Duration(0.1), timerCallbackNuller);
    ros::Timer timer2 = n.createTimer(ros::Duration(0.5), timerCallbackUpdater);
    while (n.ok())
    {

        if(publish)
	{
	    std_msgs::Bool newmsg;
	    newmsg.data = alive;
	    pubStatus.publish(newmsg);
	}

        ros::spinOnce();
        r.sleep();
    }



}

