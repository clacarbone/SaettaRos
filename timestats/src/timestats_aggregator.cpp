#include <ros/ros.h>
#include <ros/subscriber.h>
#include <time.h>
#include "timing_class.hpp"
#include <timestats/AggregatedStats.h>
#include <timestats/Machine.h>
#include <timestats/Report.h>
#include <timestats/StatsInfo.h>
#include <timestats/extStatsInfo.h>

std::string MachineName;
int counter = 0, filter = 0;
double filt_coeff = 1.0;
std::map<std::string, double> mapPingFiltered;
std::map<std::string, double>::iterator mapiterPingFiltered;
std::map<std::string, double> mapJitterFiltered;
std::map<std::string, double>::iterator mapiterJitterFiltered;
class timedata
{
public:
    unsigned int mov_aver_size;
    double filt_coeff;
    TimeStatistics::timestruct ping;
    TimeStatistics::timestruct jitter;
    TimeStatistics::timestruct pingaver;
    TimeStatistics::timestruct jitteraver;
    std::string name;

    template <typename T>
    void addPingMs(T value)
    {
        ping.assignFromMs(value);
        pingaver.assignFromMs((pingaver.ms() * filt_coeff) + (1-filt_coeff) * value);
    }
    template <typename T>
    void addJitterMs(T value)
    {
        jitter.assignFromMs(value);
        jitteraver.assignFromMs((jitteraver.ms() * filt_coeff) + (1-filt_coeff) * value);
    }    
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
        timestats::extStatsInfo newstats;
        newstats.mov_aver_size = (*_datamap_iter).second.mov_aver_size;
        newstats.name.assign((*_datamap_iter).second.name);
        newstats.jitter = (*_datamap_iter).second.jitter.ms();
        newstats.ping = (*_datamap_iter).second.ping.ms();
        newstats.ping_LP = (*_datamap_iter).second.pingaver.ms();
        newstats.jitter_LP = (*_datamap_iter).second.jitteraver.ms();
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
            (*_datamap_iter).second.mov_aver_size = msg.filter_size;
        }
        else
        {
            timedata mydata;
            mydata.filt_coeff = filt_coeff;
            mydata.mov_aver_size = msg.filter_size;
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
    private_handle_.param("filt_coeff", filt_coeff, double(1.0));
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

