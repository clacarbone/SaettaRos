#define __GXX_EXPERIMENTAL_CXX0X__
#include "holonomic_ctrl.hpp"
#define LOOP_RATE	50 	//Hz
#define	MAX_SPEED	0.2	//m/s

template <typename Type1, typename Type2>
int RosFetchParam( ros::NodeHandle& node, Type1& parmname, Type2 storage )
{
    Type2 temp;
    if (node.getParam(parmname, temp))
    {
        storage = temp;
        return 0;
    }
    std::string newstr(parmname);
    boost::to_upper(newstr);
    ROS_ERROR("Parameter \"%s\" does not exist. Check your configuration.", newstr.c_str());
    return -1;

}

template <typename Type1>
bool VectorContains( std::vector<Type1> vect, Type1 elem )
{
    if (std::find(vect.begin(), vect.end(), elem) != vect.end()) return true;
    else return false;
}


int main( int argc, char** argv )
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
    ros::Publisher pubCmdVel1, pubCmdVel2, pubCmdVel3;
    pubCmdVel1 = n.advertise<saetta_msgs::cmd_vel > ("/robot_1/cmd_vel", 1);
    pubCmdVel2 = n.advertise<saetta_msgs::cmd_vel > ("/robot_2/cmd_vel", 1);
    pubCmdVel3 = n.advertise<saetta_msgs::cmd_vel > ("/robot_3/cmd_vel", 1);
    tf::TransformListener listener;
    tf::StampedTransform transform1, transform2, transform3;
    tf::Vector3 vactual1, vactual2, vactual3, cmd1, cmd2, cmd3, *tmp1, *tmp2, *err;
    tf::Quaternion qactual1,qactual2,qactual3;
    double roll,pitch,yaw, dist12, dist13, dist23, factor;
    while (n.ok())
    {        
	
    	try{
      	    listener.lookupTransform("world", "/robot_1/base_link", ros::Time(0), transform1);
      	    listener.lookupTransform("world", "/robot_2/base_link", ros::Time(0), transform2);
      	    listener.lookupTransform("world", "/robot_3/base_link", ros::Time(0), transform3);
	    vactual1 = transform1.getOrigin();
	    vactual2 = transform2.getOrigin();
	    vactual3 = transform3.getOrigin();
	    qactual1 = transform1.getRotation();
	    qactual2 = transform2.getRotation();
	    qactual3 = transform3.getRotation();
	    tf::Matrix3x3(qactual1).getRPY(roll, pitch, yaw);
	    dist12 = (powf(vactual2.x()-vactual1.x(),2)+powf(vactual2.y()-vactual1.y(),2));
	    dist13 = (powf(vactual3.x()-vactual1.x(),2)+powf(vactual3.y()-vactual1.y(),2));
	    dist23 = (powf(vactual3.x()-vactual2.x(),2)+powf(vactual3.y()-vactual2.y(),2));

	    //compute error correction for node 1
	    err = new tf::Vector3(vactual2.x() - vactual1.x(),vactual2.y() - vactual1.y(), 0);
	    factor = - 0.5 * (dist12 - 1.0);
	    tmp1 = new tf::Vector3(factor * err->x(), factor * err->y(), 0);
	    err = new tf::Vector3(vactual3.x() - vactual1.x(), vactual3.y() - vactual1.y(), 0);
	    factor = - 0.5 * (dist13 - 1.0);
	    tmp2 = new tf::Vector3(factor * err->x(), factor * err->y(), 0);
	    cmd1 = *tmp1 + *tmp2;

	    //compute error correction for node 2
	    err = new tf::Vector3(vactual1.x() - vactual2.x(), vactual1.y() - vactual2.y(), 0);
	    factor = - 0.5 * (dist12 - 1.0);
	    tmp1 = new tf::Vector3(factor * err->x(), factor * err->y(), 0);
	    err = new tf::Vector3(vactual3.x() - vactual2.x(), vactual3.y() - vactual2.y(), 0);
	    factor = - 0.5 * (dist23 - 1.0);
	    tmp2 = new tf::Vector3(factor * err->x(), factor * err->y(), 0);
	    cmd2 = *tmp1 + *tmp2;

	    //compute error correction for node 3
	    err = new tf::Vector3(vactual2.x() - vactual3.x(), vactual2.y() - vactual3.y(), 0);
	    factor = - 0.5 * (dist23 - 1.0); 
	    tmp1 = new tf::Vector3(factor * err->x(), factor * err->y(), 0);
	    err = new tf::Vector3(vactual1.x() - vactual3.x(), vactual1.y() - vactual3.y(), 0);
	    factor = - 0.5 * (dist13 - 1.0);
	    tmp2 = new tf::Vector3(factor * err->x(), factor * err->y(), 0);
	    cmd3 = *tmp1 + *tmp2;

	}
	catch (tf::TransformException ex){
      	    ROS_ERROR("%s",ex.what());
    	}
	

	std::cout << "Robot 1 -> X: " << cmd1.x() << "  Y: " << cmd1.y() << std::endl;
	std::cout << "Robot 2 -> X: " << cmd2.x() << "  Y: " << cmd2.y() << std::endl;
	std::cout << "Robot 3 -> X: " << cmd3.x() << "  Y: " << cmd3.y() << std::endl;
	std::cout << "Formation : [" << dist12 << " , " << dist13 << " , " << dist23 << "]" << std::endl << std::endl;
        ros::spinOnce();
        r.sleep();
    }



}


