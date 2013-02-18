/* 
 * File:   main.cpp
 * Author: andrea
 *
 * Created on February 8, 2013, 3:14 PM
 */

#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iostream>
#include "Vision.hpp"
#include <ros/ros.h>

/*
 * 
 */


int main ( int argc, char** argv )
{
    std::string visionPrefix, visionRobnum, robName, visionRobCtrl;
    std::stringstream ss;
    std::vector<std::pair<std::string, int>> robVector;
    int robNum;
    Vision::VisionConfig myconfig;
    myconfig.set_camera ( 1280, 720 );
    myconfig.set_windows ( 640, 480 );
    myconfig.set_mapname ( "Map.txt" );
    myconfig.set_showwindows ( true );
    Vision::Vision myvision ( myconfig );
    myvision.Start ( );

    ros::init ( argc, argv, "Saetta_Vision" );
    ros::NodeHandle n;

    // Declare variables that can be modified by launch file or command line.
    int rate = 20;

    n.getParam("saetta_vision_prefix", visionPrefix);
    n.getParam("saetta_vision_robnum", visionRobnum);
    n.getParam("saetta_vision_ctrl_topic", visionRobCtrl);
    robNum = atoi(visionRobnum.c_str());
    for (int i=0; i<robNum;i++)
    {
        ss.clear ();
        ss << visionPrefix <<robNum;
        robName = ss.str();
        if (n.hasParam(robName))
        {
            std::pair<std::string,int> localpair(robName,-1);
            robVector.push_back (localpair);            
        }
    }
    
    //    localbase->InitWifi(ip);
    // Initialize node parameters from launch file or command line.
    // Use a private node handle so that multiple instances of the node can be run simultaneously
    // while using different parameters.
    ros::NodeHandle base_handle_ ( "~" );

    //base_handle_.param("rate", rate, int(40));
    //ros::Subscriber sub = n.subscribe ( "/saetta/velocity", 10, &Saetta_Base::listenerCallback, localbase );
    //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

    // Create a new NodeExample object.
    //Saetta_Base *base_node = new Saetta_Base();

    // Create a subscriber.
    // Name the topic, message queue, callback function with class name, and object containing callback function.
    //ros::Subscriber sub_message = n.subscribe("example", 1000, &base_node.messageCallback, Saetta_Base);

    // Tell ROS how fast to run this node.
    ros::Rate r ( rate );
    while (n.ok ( ))
    {
        ros::spinOnce ( );
        r.sleep ( );
    }

    myvision.Stop ( );


}


