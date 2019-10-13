#ifndef ICHICK_ARDUINO_SERVERHELPER_H
#define ICHICK_ARDUINO_SERVERHELPER_H

#include <EthernetServer.h>

class ServerHelper {
    EthernetServer server = EthernetServer(80);

public:
    explicit ServerHelper(int port);
    void init();
    void handleConnexion();

private:
    static String buildJsonResponse(const String& json);
};


#endif //ICHICK_ARDUINO_SERVERHELPER_H
