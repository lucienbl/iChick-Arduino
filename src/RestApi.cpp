#include <Arduino.h>
#include "RestApi.h"
#include <Ethernet.h>
#include <Log.h>

RestApi::RestApi(int port) {
    server = EthernetServer(port);
}

void RestApi::init() {
    LOG_N("Initializing server...");

    byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0xDF, 0xAB};
    EthernetClass::begin(mac);

    LOG("Server successfully started on: ");
    LOG_N(EthernetClass::localIP());

    server.begin();
}

void RestApi::run() {
    EthernetClient client = server.available();

    if (client) {
        if (client.connected()) {
            char route[ROUTES_LENGHT] = {0};
            bool routePrepare = false;
            bool routeCatchFinished = false;
            uint8_t r = 0;

            char query[QUERY_LENGTH] = {0};
            bool queryPrepare = false;
            bool queryCatchFinished = false;
            uint8_t q = 0;

            char method[METHODS_LENGTH] = {0};
            bool methodCatchFinished = false;
            uint8_t m = 0;

            bool currentLineIsBlank = true;
            char c;
            while ( client.connected() && client.available() ) {
                c = client.read();

                // Start end of line process ////////////////
                if (c == '\n' && currentLineIsBlank) {
                    // Here is where the parameters of other HTTP Methods will be.
                    while(client.available() && client.connected())
                        query[q++] = (client.read());

                    break;
                }

                if (c == '\n')
                    currentLineIsBlank = true; // you're starting a new line
                else if (c != '\r')
                    currentLineIsBlank = false; // you've gotten a character on the current line
                // End end of line process //////////////////

                // Start route catch process ////////////////
                if(c == '/' && !routePrepare)
                    routePrepare = true;

                if((c == ' ' || c == '?') && routePrepare)
                    routeCatchFinished = true;

                if(routePrepare && !routeCatchFinished)
                    route[r++] = c;
                // End route catch process //////////////////

                // Start query catch process ////////////////
                if(c == ' ' && queryPrepare)
                    queryCatchFinished = true;

                if(queryPrepare && !queryCatchFinished)
                    query[q++] = c;

                if(c == '?' && !queryPrepare)
                    queryPrepare = true;
                // End query catch process /////////////////

                // Start method catch process ///////////////
                if(c == ' ' && !methodCatchFinished)
                    methodCatchFinished = true;

                if(!methodCatchFinished)
                    method[m++] = c;
                // End method catch process /////////////////
            }

            boolean routeFound = false;

            for (int i = 0; i < routesIndex_; i++) {
                // Check if the routes names matches
                if (strncmp(route, routes_[i].name, sizeof(routes_[i].name)) != 0) continue;

                // Check if the HTTP METHOD matters for this route
                if (strncmp(routes_[i].method, "*", sizeof(routes_[i].method)) != 0) {
                    // If it matters, check if the methods matches
                    if (strncmp(method, routes_[i].method, sizeof(routes_[i].method)) != 0) continue;
                }

                // Route callback (function)
                routeFound = true;
                client.print(buildJsonResponse(routes_[i].callback(query)));
            }

            if (!routeFound) client.print(buildJsonResponse(R"({"status": "error", "code": 404, "message": "Not Found"})"));
            client.stop();
        }
    }
}

void RestApi::addRoute(char * method, char * route, String (*f)(char *string) ) {
    routes_[routesIndex_].method   = method;
    routes_[routesIndex_].name     = route;
    routes_[routesIndex_].callback = f;

    LOG("   ");
    LOG(routes_[routesIndex_].method);
    for (int i = 10 - String(routes_[routesIndex_].method).length(); i > 0; i--){
        LOG(" ");
    }
    LOG(routes_[routesIndex_].name);
    LOG("\n");
    routesIndex_++;
}

String RestApi::buildJsonResponse(const String& json) {
    String response;
    response += "HTTP/1.1 200 OK\n";
    response += "Content-Type: application/json;charset=utf-8\n";
    response += "Access-Control-Allow-Origin: *\n";
    response += "Server: Arduino\n";
    response += "Connnection: close\n\n";
    response += json;
    response += "\n";

    return response;
}