#ifndef ICHICK_ARDUINO_RESTAPI_H
#define ICHICK_ARDUINO_RESTAPI_H

#include <EthernetServer.h>
#include "RestSettings.h"

struct Routes {
    char *method;
    char *name;
    String (*callback)(char *params);
};

class RestApi {
    EthernetServer server = EthernetServer(80);

public:
    explicit RestApi(int port);

    void init();
    void run();
    void addRoute(char *method, char *route, String (*f)(char *string));

private:
    Routes routes_[ROUTES_TOTAL]{};
    uint8_t routesIndex_{};

    static String buildJsonResponse(const String& json);

};


#endif //ICHICK_ARDUINO_RESTAPI_H
