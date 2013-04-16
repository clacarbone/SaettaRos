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
timestats::Report traceNew, traceReply;
timestats::StatsInfo statsmsg;
int counter = 0, filter = 0;
bool ping_submit_ready = false, ready_reply = false, master = false, stats_submit_ready = false;
std::stringstream ss;
TimeStatistics::accumulator *_node_data;

void timerCallback( const ros::TimerEvent& tevent )
{
    timespec timestruct;
    clock_gettime(CLOCK_REALTIME, &timestruct);
    traceNew.machine.clear();
    traceNew.header.assign(MachineName);
    traceNew.counter = counter;
    timestats::Machine localmachine;
    localmachine.name.assign(MachineName);
    localmachine.timestamp = ((((uint64_t) timestruct.tv_sec) << 32) + timestruct.tv_nsec);
    traceNew.machine.push_back(localmachine);
    ping_submit_ready = true;
    counter++;
}

void subscriberCallback( const timestats::Report& msg )
{
    timespec timestruct;
    clock_gettime(CLOCK_REALTIME, &timestruct);
    traceReply = msg;
    timestats::Machine localmachine;
    localmachine.name.assign(MachineName);
    localmachine.timestamp = (((uint64_t) timestruct.tv_sec << 32) + timestruct.tv_nsec);
    traceReply.machine.push_back(localmachine);
    if (master)
    {
        if (msg.counter + 2 >= counter)
        {
            std::cout << "Received trace id: " << traceReply.header << std::endl << "no: " << traceReply.counter << std::endl;
            uint64_t measure = (traceReply.machine.back()).timestamp - (traceReply.machine.front()).timestamp;
            if (measure < 1500000000)
            {
                _node_data->addMeasure((traceReply.machine.back()).timestamp - (traceReply.machine.front()).timestamp);
                std::cout << "Ping: " << _node_data->getPingAverageMs() << std::endl
                        << "Jitter: " << _node_data->getPingJitterMs() << std::endl << std::endl;
                statsmsg.name.assign(MachineName);
                if (_node_data->getPingAverageMs() > 100000)
                {
                    std::cout << "\tlast: " << (traceReply.machine.back()).timestamp << " | " << (((uint64_t) timestruct.tv_sec << 32) + timestruct.tv_nsec) << std::endl;
                    std::cout << "\tfirst: " << (traceReply.machine.front()).timestamp << std::endl;
                }
                statsmsg.ping = _node_data->getPingAverageMs();
                statsmsg.jitter = _node_data->getPingJitterMs();
                statsmsg.filter_size = static_cast<unsigned int> (_node_data->getFilterSize());
                stats_submit_ready = true;
            }
        }
    }
    else
        ready_reply = true;

}

int main( int argc, char** argv )
{

    ros::init(argc, argv, "Time_Stats_Generator");
    ros::NodeHandle n;
    ros::NodeHandle private_handle_("~");
    std::string topic_input, topic_output;
    double delay = 1;
    private_handle_.param("input", topic_input, std::string("input"));
    private_handle_.param("output", topic_output, std::string("output"));
    private_handle_.param("delay", delay, double(1));
    private_handle_.param("master", master, bool(false));
    private_handle_.param("filter", filter, int(20));
    MachineName.assign(ros::this_node::getName());
    ros::Rate r(2000);
    ros::Publisher localpub, statspub;
    ros::Subscriber localsub;
    localpub = n.advertise<timestats::Report > (topic_output.c_str(), 1);

    localsub = n.subscribe(topic_input.c_str(), 1, subscriberCallback);
    _node_data = new TimeStatistics::accumulator("node", filter);
    if (master == 1)
    {
        statspub = n.advertise<timestats::StatsInfo > ("timestats_pool", 1);
        std::cout << "Running as master" << std::endl << "Timer delay: " << delay << "s" << std::endl << "Node name: " << MachineName << std::endl;
        ros::Timer timer = n.createTimer(ros::Duration(delay), timerCallback);
        while (n.ok())
        {
            if (ping_submit_ready == true)
            {
                ping_submit_ready = false;
                localpub.publish(traceNew);
            }

            if (stats_submit_ready == true)
            {
                stats_submit_ready = false;
                statspub.publish(statsmsg);
            }
            ros::spinOnce();
            r.sleep();
        }
    }
    else
    {
        std::cout << "Running as relay" << std::endl;
        while (n.ok())
        {
            if (ready_reply == true)
            {
                ready_reply = false;
                ss.str("");
                localpub.publish(traceReply);
            }
            ros::spinOnce();
            r.sleep();
        }
    }
    return (EXIT_SUCCESS);
}
