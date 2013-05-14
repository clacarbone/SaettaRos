#define __GXX_EXPERIMENTAL_CXX0X__
#include "rigidform.hpp"
#define LOOP_RATE	50 	// Hz
#define	MAX_SPEED	0.2	// m/s
#define PIVOT_D_AXIS	0.8	// m
#define SPEED_MULT	10.0
#define SPD_TSRH	1.0
#define ANG_MULT	2.0
#define ERR_TSRH	0.000625
std::string 
bool publish = false;
std::list<std::string> neighbours;
std::list<float> distances;

void timerCallback(const ros::TimerEvent&)
{
    publish = true;
}

void subscriberCallback(const timestats::StatsInfo& msg)
{    
    if (msg.rob_name.compare(ns) == 0)
    {
	distances.clear();
	distances.resize(msg.neighbours.size());
        neighbours.clear();
        neighbours.resize(msg.neighbours.size());
        std::list<std::string>::iterator iter_n = msg.neighbours.begin;
	std::list<float>::iterator iter_d = msg.distances.begin;
        while (iter_n != msg.neighbours.end)
        {
	    neighbours.insert(*iter_n);
	    iter_n++;
	    distances.indert(*iter_d);
	    iter_d++;
        }
    }    
}

int main(int argc, char** argv)
{


    ros::init(argc, argv, "Saetta_RigidForm");
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
    ros::Publisher pubCmdVel;
    tf::TransformListener listener;
    tf::StampedTransform transform;
    tf::Vector3 self_pos, neighb_pos, cmd, *tmp, *err;
    tf::Quaternion qactual, self_att, neighb_att;
    double roll, pitch, yaw, dist, factor, lin_vel, ang_vel;
    saetta_msgs::cmd_vel msg;
    ros::Timer timer = n.createTimer(ros::Duration(0.25), timerCallback);
    std::string ns = strtok(ros::this_node::getNamespace(),"/");
    stringstream ss;
    ss << "/" << ns << "/base_link";
    std::string tf_name(ss.str());
    ss.str("");
    ss << "/" << ns << "/cmd_vel";
    std::string ctrl_topic(ss.str());
    ss.str("");
    ros::Subscriber localsub = n.subscribe("visib_graph", 1, subscriberCallback);
    pubCmdVel = n.advertise<saetta_msgs::cmd_vel > (ctrl_topic, 1);

    while (n.ok())
    {

        try
        {
	    std::list<std::string>::iterator iter_n = neighbours.begin;
	    std::list<float>::iterator iter_d = distances.begin;
	    tf::Vector3 self_pos, neighb_pos;
	    tf::Quaternion self_att, neighb_att;
    	    ss << "/" << ns << "/base_link";
    	    std::string tf_name(ss.str());
    	    ss.str("");
	    listener.lookupTransform("world", tf_name, ros::Time(0), transform);
	    self_pos = transform.getOrigin();
	    self_att = transform.getRotation();
	    while(iter_n != neighbours.end)
	    {
		stringstream ss;
    		ss << "/" << (*iter_n) << "/base_link";
		std::string tf_name(ss.str());
		ss.str("");
		listener.lookupTransform("world", tf_name, ros::Time(0), transform);
		neighb_pos = transform.getOrigin();
		neighb_att = transform.getRotation();
		dist = sqrtf(powf(neighb_pos.x() - self_pos.x(), 2) + powf(neighb_pos.y() - self_pos.y(), 2));
		err = new tf::Vector3(neighb_pos.x() - self_pos.x(), neighb_pos.y() - self_pos.y(), 0);
		factor = 0.5 * (dist - (*iter_d));
		tmp = new tf::Vector3(factor * err->x(), factor * err->y(), 0);
		cmd += *tmp;
		iter_d++;
		iter_n++;
	    }

	    tf::Matrix3x3(qactual).getRPY(roll, pitch, yaw);
            lin_vel = cos(yaw) * cmd.x() + sin(yaw) * cmd.y();
            ang_vel = -(1 / PIVOT_D_AXIS) * sin(yaw) * cmd.x()+(1 / PIVOT_D_AXIS) * cos(yaw) * cmd.y();
            msg.linear = lin_vel * SPEED_MULT;
	    if(powf(dist12-1,2) > ERR_TSRH || powf(dist13-1,2) > ERR_TSRH)
	    {
                if (msg.linear > 10.0)
                    msg.linear = 10.0;
	        if (msg.linear < -10.0)
                    msg.linear = -10.0;
                msg.angular = ang_vel * ANG_MULT;
                if (msg.angular > 0.4)
                    msg.angular = 0.4;
                if (msg.angular < -0.4)
                    msg.angular = -0.4;
	    }
	    else
	    {
		msg.linear = 0;
		msg.angular= 0;
	    }


            if (publish)
            {
                pubCmdVel1.publish(msg);
            }
        }
        catch (tf::TransformException ex)
        {
            ROS_ERROR("%s", ex.what());
        }

        ros::spinOnce();
        r.sleep();
    }



}

