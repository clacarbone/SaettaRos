/* 
 * File:   main.cpp
 * Author: andrea
 *
 * Created on February 8, 2013, 3:14 PM
 */
#define __GXX_EXPERIMENTAL_CXX0X__
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <string>
#include <boost/algorithm/string.hpp>
#include <ros/ros.h>
#include <math.h>
#include <tf/transform_broadcaster.h>

#define LOOP_RATE	50 //Hz
#define FILTER_COEFF	0.8
/*
 * 
 */


int main( int argc, char** argv )
{
    ros::init(argc, argv, "tf_broadcaster");
    ros::NodeHandle n;

    // Initialize node parameters from launch file or command line.
    // Use a private node handle so that multiple instances of the node can be run simultaneously
    // while using different parameters.
    ros::NodeHandle base_handle_("~");

    //base_handle_.param("rate", rate, int(40));
    //ros::Subscriber sub = n.subscribe ( "/saetta/velocity", 10, &Saetta_Base::listenerCallback, localbase );
    //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);


    // Tell ROS how fast to run this node.
    tf::TransformBroadcaster br;
    tf::Transform transform;
    ros::Rate r(50);
    float distx, disty, dist, angle=0;
    long int index = 0;
    while (n.ok())
    {

        /*switch (index)
	{
	    case 0:*/
		transform.setOrigin(tf::Vector3(0,0,0));
		transform.setRotation(tf::Quaternion(0,0,0.785 + angle));
		br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world"  , "/robot_1/base_link"));
	    //break;
	    //case 1:
		transform.setOrigin(tf::Vector3(1,0,0));
		transform.setRotation(tf::Quaternion(0,0,angle));
		br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world"  , "/robot_2/base_link"));
	    //break;
	    //case 2:
		transform.setOrigin(tf::Vector3(0,1,0));
		transform.setRotation(tf::Quaternion(0,0,1.57 + angle));
		br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world"  , "/robot_3/base_link"));
	    /*break;
	    default:
	    break;
	}*/
	index++;
	angle = (float)index/360 * M_PI;
        ros::spinOnce();
        r.sleep();
    }


}

