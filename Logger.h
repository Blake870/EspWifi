#ifndef Logger_h
#define Logger_h

#include <EEPROM.h>

class Logger {

    static Logger* _instance;
public:
    Logger() {

    }

    static Logger* instance()
    {
        if(!Logger::_instance)
        {
            Logger::_instance = new Logger();
        }
        return Logger::_instance;
    }

protected:

};

Logger* Logger::_instance;

#endif
