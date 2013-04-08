/* 
 * File:   timing_class.hpp
 * Author: erupter
 *
 * Created on April 4, 2013, 12:20 PM
 */

#ifndef TIMING_CLASS_HPP
#define	TIMING_CLASS_HPP

#include <string>
#include <list>
#include <time.h>
#include <ros/ros.h>
#include <ros/subscriber.h>
#include <ros/publisher.h>
#define NODE_PING_LIST_SIZE 20

namespace TimeStatistics
{

    class timestruct
    {
    private:
        timespec _tvalue;
    public:

        timestruct();
        timestruct(timespec t);
        timestruct(const TimeStatistics::timestruct &t);
        virtual ~timestruct();
        void operator =(timespec t);
        void operator =(timestruct t);
        template <typename T>
        void assignFromMs(T num)
        {
            num=num*1000000;
            _tvalue.tv_sec = ((static_cast<unsigned long int>(num)) & 0xffffffff00000000) >> 32;
            _tvalue.tv_nsec = ((static_cast<unsigned long int>(num)) & 0x00000000ffffffff);
        }
        operator double();
        operator float();
        operator timespec();
        void set_nsec(long int nsec);
        long int get_nsec();
        void set_sec(long int sec);
        long int get_sec();
        double ms();
        double us();

    };

    class accumulator
    {
    private:
        std::string _name;
        std::list<timestruct> _pings;
        std::list<timestruct>::iterator _iter;
        timestruct _jitter;
        timestruct _average;
        long unsigned int _size;

        void calculations();
        void trim (void);

    public:
        accumulator();
        template <typename T>
        accumulator(T name)
        {
            _name.assign(name);
            _size = NODE_PING_LIST_SIZE;
        };
        
        template <typename T>
        accumulator(T name, long unsigned int size)
        {
            _name.assign(name);
            _size = size;
        };

        virtual ~accumulator();        
        unsigned long int getFilterSize();        
        void setFilterSize( unsigned long int size);
        std::string getName();
        template <typename T>
        void setName(T t1)
        {
            _name.assign(t1);
        };

        void addMeasure(timespec t);
        void addMeasure(long long int t);
        double getPing();
        double getPingMs();
        timespec getPingAverageT();
        double getPingAverageMs();
        timespec getPingJitterT();
        double getPingJitterMs();
        
    };

    /*class Responder
    {
    private:
        std::string _name;
        void (*_send) (std_msgs::String msg);

        void Actor(std::string msgstr)
        {
            TimeStatisticsMsgs::Action origmsg, newmsg;
            origmsg.ParseFromString(msgstr);
            newmsg.set_header(origmsg.header());
            if (!origmsg.header().compare("PING"))
            {
                if (!_name.compare(origmsg.machine(0).name()))
                {
                    TimeStatisticsMsgs::Machine * lclmachine = newmsg.add_machine();
                    lclmachine->set_name(_name);
                    lclmachine->set_processed(true);
                }

            }
            else if (!origmsg.header().compare("TRACE"))
            {
                for (int i = 0; i < origmsg.machine_size(); i++)
                {
                    if (_name.compare(origmsg.machine(i).name()))
                    {
                        TimeStatisticsMsgs::Machine * lclmachine = newmsg.add_machine();
                        lclmachine->CopyFrom(origmsg.machine(i));
                    }
                    else if (!(_name.compare(origmsg.machine(i).name())))
                    {
                        TimeStatisticsMsgs::Machine * lclmachine = newmsg.add_machine();
                        lclmachine->CopyFrom(origmsg.machine(i));
                        lclmachine->set_processed(true);
                    }
                }
            }
            std_msgs::String rosmsg;
            rosmsg.data = newmsg.SerializeAsString();
            _send(rosmsg);
        }

    public:

        Responder(std::string name, void (*fcnptr)(std_msgs::String msg))
        {
            this->_name = name;
            this->_send = fcnptr;
        }

        virtual ~Responder();

        void receiverCallback(std_msgs::String & msg)
        {
            Actor(msg.data);
        }
    };

    class path
    {
    private:

        std::list < std::string > _nodelist;
        TimeStatistics::accumulator _timedata;
    public:
        path();
        virtual ~path();

        void add_node(std::string name)
        {
            _nodelist.push_back(name);
        }

    };

    class Manager
    {
    private:
        std::map<std::string, TimeStatistics::accumulator> _nodelistmap;
        std::map<std::string, TimeStatistics::accumulator>::iterator _nodelistmap_iter;
        void (*_send) (std_msgs::String msg);
        ros::NodeHandle *_handle;
        ros::Publisher _publisher;
        std::list<TimeStatistics::accumulator> _nodelist;
        std::list<TimeStatistics::accumulator>::iterator _node_iter;
        std::string _name;
        std::string _configpath;
        //std::list<TimeStatistics::path> _pathlist;
        //std::list<TimeStatistics::path>::iterator _path_iter;
        std::map<std::string, TimeStatistics::accumulator> _pathlist;
        std::map<std::string, TimeStatistics::accumulator>::iterator _pathlist_iter;

    public:
        Manager(std::string pathtofile, ros::NodeHandle &handle);
        virtual ~Manager();
        void timerCallbackPing();
        void timerCallbackTrace();
        void parseConfig(std::string pathtofile);
        void receiverCallback(std_msgs::String & msg);
    };*/
}
#endif	/* TIMING_CLASS_HPP */

