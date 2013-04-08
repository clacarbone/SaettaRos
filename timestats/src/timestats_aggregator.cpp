#include <ros/ros.h>
#include <ros/subscriber.h>
#include <time.h>
#include <sstream>
#include <timing_class.hpp>
#include <timestats/AggregatedStats.h>
#include <timestats/Machine.h>
#include <timestats/Report.h>
#include <timestats/StatsInfo.h>

std::string MachineName;
int counter = 0, filter = 0;
std::stringstream ss;
struct timedata
{
    unsigned int filter_size;
    TimeStatistics::timestruct ping;
    TimeStatistics::timestruct jitter;
    std::string name;
};
std::map<std::string, timedata> _datamap;
std::map<std::string, timedata>::iterator _datamap_iter;
std::pair < std::map<std::string, timedata>::iterator, bool> returndata;
timestats::AggregatedStats newstatsmsg;
bool stats_ready = false;


void timerCallback(const ros::TimerEvent&)
{
    newstatsmsg.links.clear();
    for (_datamap_iter = _datamap.begin(); _datamap_iter != _datamap.end(); _datamap_iter++)
    {
        std::cout << "Station " << (*_datamap_iter).second.name << std::endl
                << "\tping: " << (*_datamap_iter).second.ping.ms() << std::endl
                << "\tjitter: " << (*_datamap_iter).second.jitter.ms() << std::endl;
        timestats::StatsInfo newstats;
        newstats.filter_size = (*_datamap_iter).second.filter_size;
        newstats.name.assign((*_datamap_iter).second.name);
        newstats.jitter = (*_datamap_iter).second.jitter.ms();
        newstats.ping = (*_datamap_iter).second.ping.ms();
        newstatsmsg.links.push_back(newstats);
    }
    if (_datamap.size() > 0)
    {
        std::cout << std::endl;
        stats_ready = true;
    }

}

void subscriberCallback(const timestats::StatsInfo& msg)
{    
        if ((_datamap_iter = _datamap.find(msg.name)) != _datamap.end())
        {
            (*_datamap_iter).second.ping.assignFromMs(msg.ping);
            (*_datamap_iter).second.jitter.assignFromMs(msg.jitter);
            (*_datamap_iter).second.name.assign(msg.name);
            (*_datamap_iter).second.filter_size = msg.filter_size;
        }
        else
        {
            timedata mydata;
            mydata.filter_size = msg.filter_size;
            mydata.name.assign(msg.name);
            mydata.ping.assignFromMs(msg.ping);
            mydata.jitter.assignFromMs(msg.jitter);
            _datamap.insert(std::make_pair(msg.name, mydata));
        }
    
}

int main(int argc, char** argv)
{

    ros::init(argc, argv, "Time_Stats_Aggregator");
    ros::NodeHandle n;
    ros::NodeHandle private_handle_("~");
    ros::Subscriber localsub;
    ros::Publisher localpub;
    localsub = n.subscribe("timestats_pool", 1, subscriberCallback);
    localpub = n.advertise<timestats::AggregatedStats > ("timestats_aggregated", 1);

    MachineName.assign(ros::this_node::getName());
    ros::Rate r(2000);
    ros::Timer timer = n.createTimer(ros::Duration(0.04), timerCallback);
    while (n.ok())
    {
        if (stats_ready == true)
        {
            stats_ready = false;
            localpub.publish(newstatsmsg);
        }
        ros::spinOnce();
        r.sleep();
    }

    return (EXIT_SUCCESS);
}

