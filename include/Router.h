#ifndef ICHICK_ARDUINO_ROUTER_H
#define ICHICK_ARDUINO_ROUTER_H

#include <Arduino.h>

class Router {
    String m_Header;

public:
    Router();
    String route(const String& header);

private:
    bool isPath(const String& path);
};


#endif //ICHICK_ARDUINO_ROUTER_H
