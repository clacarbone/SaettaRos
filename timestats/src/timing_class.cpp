#include <timing_class.hpp>
#include <ros/node_handle.h>
#include <ros/publisher.h>
#include <ros/init.h>


namespace TimeStatistics
{

    timestruct::timestruct()
    {
        _tvalue.tv_sec = 0;
        _tvalue.tv_nsec = 0;
    }

    timestruct::timestruct(timespec t)
    {
        _tvalue = t;
    }

    timestruct::timestruct(const TimeStatistics::timestruct &t)
    {
        _tvalue = t._tvalue;
    }

    timestruct::~timestruct()
    {
    };

    void timestruct::operator =(timespec t)
    {
        _tvalue.tv_sec = t.tv_sec;
        _tvalue.tv_nsec = t.tv_nsec;
    }

    void timestruct::operator=(timestruct t)
    {
        _tvalue = t;
    }

    timestruct::operator double()
    {
        return (((double) (_tvalue.tv_sec << 32) + _tvalue.tv_nsec) / 1000000000);

    }

    timestruct::operator float()
    {
        return (((float) (_tvalue.tv_sec << 32) + _tvalue.tv_nsec) / 1000000000);
    }

    timestruct::operator timespec()
    {
        return _tvalue;
    }

    void timestruct::set_nsec(long int nsec)
    {
        _tvalue.tv_nsec = nsec;
    }

    long int timestruct::get_nsec()
    {
        return _tvalue.tv_nsec;
    }

    void timestruct::set_sec(long int sec)
    {
        _tvalue.tv_sec = sec;
    }

    long int timestruct::get_sec()
    {
        return _tvalue.tv_sec;
    }

    double timestruct::ms()
    {
        return (this->operator double() * 1000.0);
    }

    double timestruct::us()
    {
        return (this->operator double() * 1000000.0);
    }

    void accumulator::calculations()
    {
        float ftemp = 0;
        long long int mmax = 0, mmin = 0, itemp;

        for (_iter = _pings.begin(); _iter != _pings.end(); _iter++)
        {
            ftemp += ((*_iter).get_sec() << 32) + (*_iter).get_nsec();
            itemp = ((*_iter).get_sec() << 32) + (*_iter).get_nsec();
            //std::cout << "\tping: " << (*_iter).ms() << std::endl;
            if (mmax == 0)
                mmax = ((*_iter).get_sec() << 32) + (*_iter).get_nsec();
            else
                if (itemp > mmax)
                mmax = itemp;

            if (mmin == 0)
                mmin = ((*_iter).get_sec() << 32) + (*_iter).get_nsec();
            else
                if (itemp < mmin)
                mmin = itemp;

        }
        //std::cout << "\tMax: " << mmax << std::endl << "\tMin: " << mmin << std::endl;
        ftemp = ftemp / _pings.size();
        _average.set_nsec(((long long int) ftemp) & 0x00000000ffffffff);
        _average.set_sec((((long long int) ftemp) & 0xffffffff00000000) >> 32);
        itemp = mmax - mmin;
        _jitter.set_nsec(itemp & 0x00000000ffffffff);
        _jitter.set_sec((itemp & 0xffffffff00000000) >> 32);

    }

    void accumulator::trim(void)
    {
        while (_pings.size() > _size) _pings.pop_back();
    }

    accumulator::accumulator()
    {
        _size = NODE_PING_LIST_SIZE;
    };

    accumulator::~accumulator()
    {
    };

    unsigned long int accumulator::getFilterSize()
    {
        return _size;
    }

    void accumulator::setFilterSize(unsigned long int size)
    {
        _size = size;
        trim();
        calculations();
    }

    std::string accumulator::getName()
    {
        return _name;
    };

    void accumulator::addMeasure(timespec t)
    {
        _pings.push_front(t);
        trim();
        calculations();
    };

    void accumulator::addMeasure(long long int t)
    {
        timespec tt;
        tt.tv_sec = (t & 0xffffffff00000000) >> 32;
        tt.tv_nsec = t & 0x00000000ffffffff;
        addMeasure(tt);
    };

    double accumulator::getPing()
    {
        _iter = _pings.begin();
        return (*_iter);
    };

    double accumulator::getPingMs()
    {
        _iter = _pings.begin();
        //return (((float) ((*_iter).tv_sec << 32) + (*_iter).tv_nsec) / 1000000);
        return (*_iter).ms();
    };

    timespec accumulator::getPingAverageT()
    {
        return _average;
    };

    double accumulator::getPingAverageMs()
    {
        //return (float) ((_average.tv_sec << 32) + _average.tv_nsec) / 1000000;
        return _average.ms();
    };

    timespec accumulator::getPingJitterT()
    {
        return _jitter;
    };

    double accumulator::getPingJitterMs()
    {
        //return (float) ((_jitter.tv_sec << 32) + _jitter.tv_nsec) / 1000000;
        return _jitter.ms();
    };
/*
    Manager::Manager(std::string pathtofile, ros::NodeHandle &handle)
    {
        this->parseConfig(pathtofile);
        this->_configpath.assign(pathtofile);
        this->_handle = &handle;
        _publisher = _handle->advertise<std_msgs::String > ("time_reports", 1);
    }

    Manager::~Manager()
    {
        _publisher.shutdown();
    }

    void Manager::timerCallbackPing()
    {
        for (_node_iter = _nodelist.begin(); _node_iter != _nodelist.end(); _node_iter++)
        {
            std_msgs::String newmsg;
            TimeStatisticsMsgs::Action ping;
            TimeStatisticsMsgs::Machine lclmachine;
            lclmachine.set_name((*_node_iter).getName());
            newmsg.data = ping.SerializeAsString();
            _publisher.publish(newmsg);
            ros::spinOnce();
        }
    }

    void Manager::timerCallbackTrace()
    {

    }

    void Manager::parseConfig(std::string pathtofile)
    {

    }

    void Manager::receiverCallback(std_msgs::String & msg)
    {
        TimeStatisticsMsgs::Action lclmsg;
        lclmsg.ParseFromString(msg.data);
        if (!lclmsg.header().compare("PING"))
        {
            if (lclmsg.machine(0).processed())
            {
                timespec t;
                clock_gettime(CLOCK_REALTIME, &t);
                if ((_nodelistmap_iter = this->_nodelistmap.find(lclmsg.machine(0).name())) != _nodelistmap.end())
                    (*_nodelistmap_iter).second.addMeasure(t);
            }

        }
        else if (!lclmsg.header().compare("TRACE"))
        {
            int count;
            for (int i = 0; i < lclmsg.machine_size(); i++)
                count += lclmsg.machine(i).processed();
            if (count == lclmsg.machine_size() - 1)
            {
                timespec t;
                clock_gettime(CLOCK_REALTIME, &t);
                std::cout << "Loop complete" << std::endl;
                if ((_pathlist_iter = this->_pathlist.find(lclmsg.path())) != _pathlist.end())
                    (*_pathlist_iter).second.addMeasure(t);
            }


        }

    }*/
}