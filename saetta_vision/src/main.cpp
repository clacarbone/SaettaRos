/* 
 * File:   main.cpp
 * Author: Claudio Carbone
 *
 * Created on February 10, 2013, 3:14 PM
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
#include "Vision.hpp"
#include <ros/ros.h>
#include <saetta_msgs/cmd_vel.h>

/*
 * 
 */

typedef enum
{
    init = 0,
    err,
    waiting,
    move,
    checkmoving,
    assignindex,
    nextrob,
    configured,
    resumed
} autoconf_states_t;

typedef struct
{
    autoconf_states_t state;
    int index;
    std::string visionPrefix, robName, visionRobCtrl;
    int robCounter, visionRobnum, delayCounter, delayThreshold;
} autoconf_status_t;

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

void ConfigInit( autoconf_status_t& obj );

int main( int argc, char** argv )
{

    autoconf_status_t autoConfiguration;
    ConfigInit(autoConfiguration);
    std::stringstream ss;
    std::vector < std::pair < std::string, int >> robVector;
    std::string topic;

    ros::init(argc, argv, "Saetta_Vision");
    ros::NodeHandle n;

    // Initialize node parameters from launch file or command line.
    // Use a private node handle so that multiple instances of the node can be run simultaneously
    // while using different parameters.
    ros::NodeHandle base_handle_("~");

    //base_handle_.param("rate", rate, int(40));
    //ros::Subscriber sub = n.subscribe ( "/saetta/velocity", 10, &Saetta_Base::listenerCallback, localbase );
    //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);


    // Tell ROS how fast to run this node.


    saetta_vision::VisionConfig myconfig;
    myconfig.set_camera(1280, 720);
    myconfig.set_windows(640, 480);
    myconfig.set_mapname("Map.txt");
    myconfig.set_showwindows(true);
    saetta_vision::Vision myvision(myconfig);
    myvision.Start();
    autoConfiguration.state = autoconf_states_t::init;
    ros::Rate r(60);
    saetta_vision::RobotList_t locllist;
    saetta_msgs::cmd_vel vel;
    ros::Publisher pubCmdVel;
    while (n.ok())
    {

        if (autoConfiguration.state != configured)
        {
            switch (autoConfiguration.state)
            {
                case init:

                    //if (RosFetchParam(n, "saetta_vision_prefix", visionPrefix) != 0)
                    if (!(n.getParam("saetta_vision_prefix", autoConfiguration.visionPrefix)))
                    {
                        autoConfiguration.state = autoconf_states_t::err;
                        n.shutdown();
                        break;
                    }
                    std::cout << "Prefix: " << autoConfiguration.visionPrefix << std::endl;
                    //if ((RosFetchParam(n, "saetta_vision_robnum", visionRobnum)) != 0)
                    if (!(n.getParam("saetta_vision_robnum", autoConfiguration.visionRobnum)))
                    {
                        autoConfiguration.state = autoconf_states_t::err;
                        n.shutdown();
                        break;
                    }
                    std::cout << "Robot number: " << autoConfiguration.visionRobnum << std::endl;
                    //if ((RosFetchParam(n, "saetta_vision_cmd_topic", visionRobCtrl)) != 0)
                    if (!(n.getParam("saetta_vision_cmd_topic", autoConfiguration.visionRobCtrl)))
                    {
                        autoConfiguration.state = autoconf_states_t::err;
                        n.shutdown();
                        break;
                    }
                    std::cout << "Ctrl Topic: " << autoConfiguration.visionRobCtrl << std::endl << std::endl;
                    autoConfiguration.state = autoconf_states_t::move;
                    break;

                case move:
                    std::cout << "Issuing move command." <<std::endl;
                    autoConfiguration.robName = "";
                    ss.str("");
                    ss << autoConfiguration.visionPrefix << (autoConfiguration.robCounter + 1);
                    autoConfiguration.robName = ss.str();
                    std::cout << "Looking up " << ss.str() << std::endl;
                    ss.str("");
                    ss << "/" << autoConfiguration.robName << "/cmd_vel";
                    vel.linear = 10.0;
                    vel.angular = 0.0;
                    topic=("");
                    topic=ss.str();
                    pubCmdVel = n.advertise<saetta_msgs::cmd_vel > (topic, 1);
                    autoConfiguration.state = waiting;
                    std::cout << "Waiting 1s" <<std::endl;
                    break;

                case err:
                    break;


                    break;
                case waiting:
                    if (autoConfiguration.delayCounter==60)
                        pubCmdVel.publish(vel);
                    autoConfiguration.delayCounter++;
                    if (autoConfiguration.delayCounter >= autoConfiguration.delayThreshold)
                    {
                        autoConfiguration.state = checkmoving;
                        autoConfiguration.delayCounter = 0;
                    }
                    break;
                    
                case checkmoving:
                    std::cout << "Checking if any marker has moved." <<std::endl;
                    vel.linear = 0;
                    vel.angular = 0;
                    pubCmdVel.publish(vel);
                    
                    locllist = myvision.getRobList();
                    for (int k = 0; k < ROB_MAX; k++)
                    {
                        if (locllist.robList[k].moving)
                        {
                            std::pair<std::string, int> loclpair(autoConfiguration.robName, k);
                            robVector.push_back(loclpair);
                        }
                    }
                    autoConfiguration.state = nextrob;
                    break;
                    
                case nextrob:
                    if (autoConfiguration.robCounter < autoConfiguration.visionRobnum)
                    {
                        autoConfiguration.robCounter++;
                        autoConfiguration.state = move;
                    }
                    else
                        autoConfiguration.state = configured;
                    break;

                case resumed:
                    break;
                case configured:
                    break;

            }

        }
        ros::spinOnce();
        r.sleep();
    }

    myvision.Stop();


}


void ConfigInit( autoconf_status_t& obj )
{
    obj.state = init;
    obj.index = 0;
    obj.robCounter = 0;
    obj.visionRobnum = 5;
    obj.delayCounter = 0;
    obj.delayThreshold = 240;
}
