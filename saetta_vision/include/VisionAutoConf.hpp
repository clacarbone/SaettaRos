#ifndef VISION_AUTO_CONF_HPP
#define	VISION_AUTO_CONF_HPP
namespace saetta_vision
{
    enum AUTOCONF_STATES{
        init = 0,
        err,
        sleeping,
        processing,
        configured
    } autoconf_state_t;
    
    void AutoConfInit();
}

#endif //VISION_AUTO_CONF_HPP