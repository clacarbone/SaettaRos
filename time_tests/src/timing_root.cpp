#include <ros/ros.h>
#include <ros/subscriber.h>
#include <std_msgs/String.h>
#include <time.h>
#include <sstream>
#include <timetests.pb.h>
std::string MachineName;
TimeTests::Traceroute traceNew, traceReply;
int counter = 0;
bool ready_publish = false, ready_reply = false, master = 0;
std::stringstream ss;

#define NODE_PING_LIST_SIZE 20

class node {
private:
    std::string _name;
    std::list<timespec> _pings;
    std::list<timespec>::iterator _iter;
    timespec _jitter;
    timespec _average;
    void calculations() { 
        float ftemp=0;
        long long int mmax=0, mmin=0, itemp;
        for (_iter = _pings.begin(); _iter != _pings.end(); _iter++)
        {
            ftemp+=((*_iter).tv_sec << 32) + (*_iter).tv_nsec;
            itemp = ((*_iter).tv_sec << 32) + (*_iter).tv_nsec;
            if (mmax == 0)
                mmax = ((*_iter).tv_sec << 32) + (*_iter).tv_nsec;
            else
                if (itemp > mmax)
                    mmax = itemp;
                        
            if (mmin == 0)
                mmin = ((*_iter).tv_sec << 32) + (*_iter).tv_nsec;
            else
                if (itemp < mmin)
                    mmin = itemp;
        }
        ftemp = ftemp / _pings.size();
        _average.tv_nsec = ((long long int)ftemp) & 0x00000000ffffffff;
        _average.tv_sec = (((long long int)ftemp) & 0xffffffff00000000) >> 32;
        itemp = mmax - mmin;
        _jitter.tv_nsec = itemp & 0x00000000ffffffff;
        _jitter.tv_sec = (itemp & 0xffffffff00000000) >> 32;
    }
    
    
public:
    node(){};
    virtual ~node(){};
    std::string getName() { return _name;};
    template <typename T>
    void setName(T t1) {_name.assign(t1);};
    void addMeasure(timespec t) { _pings.push_front(t); if (_pings.size() >= NODE_PING_LIST_SIZE) _pings.pop_back(); calculations();};
    timespec getPingAverage() { return _average;};
    timespec getPingJitter() { return _jitter;};
};

void timerCallback(const ros::TimerEvent&)
{
    traceNew.Clear();
    timespec timestruct;
    clock_gettime(CLOCK_REALTIME, &timestruct);
    ss.str("");
    ss << MachineName << "_" << std::to_string(counter);
    traceNew.set_header(ss.str());
    //std::cout << "New ping from id: " << newtrace.header() << std::endl;
    TimeTests::Machine *localmachine = traceNew.add_machine();
    localmachine->set_machine_name(MachineName);
    localmachine->set_timestamp((timestruct.tv_sec << 32) + timestruct.tv_nsec);
    //std::cout << "Timestamp: " << localmachine->timestamp() << std::endl;
    //std::cout << "No of machines: " << newtrace.machine_size() << std::endl << std::endl;
    ready_publish = true;
    counter++;
}

void subscriberCallback(const std_msgs::String& msg)
{
    timespec timestruct;
    clock_gettime(CLOCK_REALTIME, &timestruct);
    traceReply.ParseFromString(msg.data);
    TimeTests::Machine * localmachine = traceReply.add_machine();
    localmachine->set_machine_name(MachineName);
    localmachine->set_timestamp((timestruct.tv_sec << 32) + timestruct.tv_nsec);
    if (master == 1)
    {
        std::cout << "Received trace id: " << traceReply.header() << std::endl;
        long long int passed = traceReply.machine(traceReply.machine_size()-1).timestamp() - traceReply.machine(0).timestamp();
        std::cout << "Time1: " << traceReply.machine(0).timestamp() << std::endl << "Time2: " << traceReply.machine(traceReply.machine_size()-1).timestamp() << std::endl;
        std::cout << "Transit time: " << (float)passed/1000000 << "ms" << std::endl;
        /*std::cout << "Number of relays: " << traceReply.machine_size() << std::endl;
        for (int i = 0; i < traceReply.machine_size(); i++)
        {
            std::cout << "\t machine " << i << std::endl
                    << "\t name: " << traceReply.machine(i).machine_name() << std::endl
                    << "\t timestamp :" << traceReply.machine(i).timestamp() << std::endl;
        }
        std::cout << std::endl << std::endl;*/

    }
    else
        ready_reply = true;

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "TimeTests");
    ros::NodeHandle n;
    ros::NodeHandle private_handle_("~");
    std::string topic_input, topic_output;
    int delay = 1;

    private_handle_.param("input", topic_input, std::string("input"));
    private_handle_.param("output", topic_output, std::string("output"));
    private_handle_.param("delay", delay, int(1));
    private_handle_.param("master", master, bool(false));
    MachineName.assign(ros::this_node::getName());
    ros::Rate r(2000);
    ros::Publisher localpub;
    ros::Subscriber localsub;
    localpub = n.advertise<std_msgs::String > (topic_output.c_str(), 1);
    localsub = n.subscribe(topic_input.c_str(), 1, subscriberCallback);

    if (master == 1)
    {
        std_msgs::String msg;
        std::cout << "Running as master" << std::endl << "Timer delay: " << delay << "s" << std::endl << "Node name: " << MachineName << std::endl;
        ros::Timer timer = n.createTimer(ros::Duration(delay), timerCallback);
        while (n.ok())
        {
            if (ready_publish == true)
            {
                ready_publish = false;
                ss.str("");
                traceNew.SerializeToString(&msg.data);                
                localpub.publish(msg);
            }
            ros::spinOnce();
            r.sleep();
        }
    }
    else
    {
        std_msgs::String msg;
        std::cout << "Running as relay" << std::endl;
        while (n.ok())
        {
            if (ready_reply == true)
            {
                ready_reply = false;
                ss.str("");
                traceReply.SerializeToString(&msg.data);                                
                localpub.publish(msg);
            }
            ros::spinOnce();
            r.sleep();
        }
    }
    return (EXIT_SUCCESS);
}
