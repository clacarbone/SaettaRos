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
#include "Vision.hpp"
#include <ros/ros.h>
#include <saetta_msgs/cmd_vel.h>
#include <math.h>
#include <tf/transform_broadcaster.h>

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
    nextrob,
    configured,
    resumed,
    delay
} autoconf_states_t;

typedef struct
{
    autoconf_states_t state, next_state;
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

template <typename Type1>
bool VectorContains( std::vector<Type1> vect, Type1 elem )
{
    if (std::find(vect.begin(), vect.end(), elem) != vect.end()) return true;
    else return false;
}
bool robothasmoved( int k, saetta_vision::RobotList_t list1, saetta_vision::RobotList_t list2 );
void ConfigInit( autoconf_status_t& obj );

int main( int argc, char** argv )
{
    std::vector<int> indexVector;
    saetta_vision::RobotList_t robotListLocal, movedRobotListLocal;
    saetta_msgs::cmd_vel vel;
    saetta_vision::VisionConfig myconfig;
    int sizet;
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


    myconfig.set_camera(1280, 720);
    myconfig.set_windows(640, 480);
    myconfig.set_mapname("Map.txt");
    myconfig.set_showwindows(true);
    saetta_vision::Vision myvision(myconfig);
    myvision.Start();
    autoConfiguration.state = autoconf_states_t::init;
    ros::Rate r(50);
    ros::Publisher pubCmdVel;
    tf::TransformBroadcaster br;
    tf::Transform transform;
    float distx, disty, dist;
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
                    std::cout << "Ctrl Topic: " << autoConfiguration.visionRobCtrl << std::endl << "Holding off until vision stabilizes..." << std::endl << std::endl;
                    
                    autoConfiguration.state = autoconf_states_t::delay;
                    autoConfiguration.next_state = autoconf_states_t::move;
                    
                    break;

                case move:
                    robotListLocal = myvision.getRobList();
                    autoConfiguration.robName = "";
                    ss.str("");
                    ss << autoConfiguration.visionPrefix << (autoConfiguration.robCounter + 1);
                    autoConfiguration.robName = ss.str();
                    std::cout << "Looking up " << ss.str() << "... " << std::endl;
                    ss.str("");
                    ss << "/" << autoConfiguration.robName << "/cmd_vel";
                    vel.linear = 10.0;
                    vel.angular = 0.0;
                    topic = ("");
                    topic = ss.str();
                    pubCmdVel = n.advertise<saetta_msgs::cmd_vel > (topic, 1);
                    autoConfiguration.state = waiting;
                    std::cout << "Waiting for movement... " << std::endl;
                    break;

                case err:
                    break;


                    break;
                case waiting:
                    if (autoConfiguration.delayCounter == 120)
                        pubCmdVel.publish(vel);
                    autoConfiguration.delayCounter++;
                    if (autoConfiguration.delayCounter >= autoConfiguration.delayThreshold)
                    {
                        autoConfiguration.state = checkmoving;
                        autoConfiguration.delayCounter = 0;
                    }
                    break;
                    
                case delay:
                    autoConfiguration.delayCounter++;
                    if (autoConfiguration.delayCounter >= autoConfiguration.delayThreshold)
                    {
                        autoConfiguration.state = autoConfiguration.next_state;
                        autoConfiguration.delayCounter = 0;
                    }
                    break;

                case checkmoving:
                    std::cout << "Checking if any marker has moved... " << std::endl;
                    vel.linear = 0;
                    vel.angular = 0;
                    pubCmdVel.publish(vel);
                    sizet = robVector.size();
                    movedRobotListLocal = myvision.getRobList();
                    //printf("Robot\t\tX_prima\tY_prima\tX_dopo\tY_dopo\tDist\n");
                    for (int k = 0; k < ROB_MAX; k++)
                    {
                        if (!VectorContains(indexVector, k))
                        {
                            /*distx = powf(robotListLocal.robList[k].coord.x - movedRobotListLocal.robList[k].coord.x, 2);
                            disty = powf(robotListLocal.robList[k].coord.y - movedRobotListLocal.robList[k].coord.y, 2);
                            dist = sqrtf(distx + disty);
                            printf("%d\t\t%5.2f\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n", k, robotListLocal.robList[k].coord.x, robotListLocal.robList[k].coord.y, movedRobotListLocal.robList[k].coord.x, movedRobotListLocal.robList[k].coord.y, dist);
                             */
                            if (robothasmoved(k, robotListLocal, movedRobotListLocal))
                            {
                                distx = powf(robotListLocal.robList[k].coord.x - movedRobotListLocal.robList[k].coord.x, 2);
                                disty = powf(robotListLocal.robList[k].coord.y - movedRobotListLocal.robList[k].coord.y, 2);
                                dist = sqrtf(distx + disty);
                                printf("%d\t\t%5.2f\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n", k, robotListLocal.robList[k].coord.x, robotListLocal.robList[k].coord.y, movedRobotListLocal.robList[k].coord.x, movedRobotListLocal.robList[k].coord.y, dist);
                                std::pair<std::string, int> loclpair(autoConfiguration.robName, k);
                                robVector.push_back(loclpair);
                                indexVector.push_back(k);
                            }

                        }
                    }
                    if (robVector.size() != sizet)
                        printf("Identified %s as id %d\n\n", robVector[robVector.size() - 1].first.c_str(), robVector[robVector.size() - 1].second);
                    else
                        printf("%s not identified.\n\n", autoConfiguration.robName.c_str());
                    autoConfiguration.state = nextrob;
                    break;

                case nextrob:
                    autoConfiguration.robCounter++;
                    if (autoConfiguration.robCounter < autoConfiguration.visionRobnum)
                        autoConfiguration.state = move;
                    else
                    {
                        autoConfiguration.state = configured;
                        std::cout << "Found " << robVector.size() << " robots." << std::endl;
                        std::cout << "Initiating TF publication for the following:" << std::endl;
                        for (int k = 0; k < robVector.size(); ++k)
                        {
                            std::cout << '\t' << robVector[k].first << std::endl;
                        }
                    }
                    break;

                case resumed:
                    break;
                case configured:
                    break;

            }
        }
        if ((autoConfiguration.state == configured) && (myvision.getTryRobList(robotListLocal)) == true)
        {
            //robotListLocal = myvision.getRobList();
            for (std::vector < std::pair < std::string, int >> ::iterator myiter = robVector.begin(); myiter != robVector.end(); ++myiter)
            {
                ss.str("");
                ss << "/" << (*myiter).first << "/base_link";
                int index = (*myiter).second;
                transform.setOrigin(tf::Vector3(robotListLocal.robList[index].coord.x/1000, robotListLocal.robList[index].coord.y/1000, 0));
                transform.setRotation(tf::Quaternion(0, 0, robotListLocal.robList[index].orientation/360*2*M_PI));
                br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", ss.str()));
                std::cout << ss.str() << "\ttf_trasl(" << robotListLocal.robList[index].coord.x/1000 <<"," << robotListLocal.robList[index].coord.y/1000 << "," << 0 << ")" << std::endl;
                std::cout << ss.str() << "\ttf_rot(" << 0 <<","<<  0 <<"," << robotListLocal.robList[index].orientation/360*2*M_PI << ")" << std::endl;
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
    obj.delayThreshold = 300;
}

bool robothasmoved( int k, saetta_vision::RobotList_t list1, saetta_vision::RobotList_t list2 )
{
    float distx, disty, dist;
    distx = powf(list1.robList[k].coord.x - list2.robList[k].coord.x, 2);
    disty = powf(list1.robList[k].coord.y - list2.robList[k].coord.y, 2);
    dist = sqrtf(distx + disty);
    if (dist >= 50)
        return true;
    return false;

}
