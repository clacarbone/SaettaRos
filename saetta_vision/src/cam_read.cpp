// Use the image_transport classes instead.
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/CvBridge.h>
#include <Vision2.hpp>

#include "Vision.hpp"

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    sensor_msgs::CvBridge bridge;
    try
    {
        cvShowImage("view", bridge.imgMsgToCv(msg, "bgr8"));
    }
    catch (sensor_msgs::CvBridgeException& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}
class myclass
{
public:
    myclass() {};
    virtual ~myclass() {};
    void imageCallback(const sensor_msgs::ImageConstPtr& msg)
    {
        sensor_msgs::CvBridge bridge;
        try
        {
            cvShowImage("view", bridge.imgMsgToCv(msg, "bgr8"));
        }
        catch (sensor_msgs::CvBridgeException& e)
        {
            ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
        }
    }
};

int main(int argc, char **argv)
{
    saetta_vision_two::VisionConfig myconfig;
    saetta_vision_two::Vision myvision(myconfig);
    myclass* test = new myclass();
    ros::init(argc, argv, "image_listener");
    ros::NodeHandle nh;
    cvNamedWindow("view");
    cvStartWindowThread();
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("camera/image", 1, (void*)&myclass::imageCallback, test);
    ros::spin();
    cvDestroyWindow("view");
}
