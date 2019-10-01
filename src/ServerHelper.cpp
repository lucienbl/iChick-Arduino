#include <Arduino.h>
#include "ServerHelper.h"
#include <Ethernet.h>
#include "Router.h"

ServerHelper::ServerHelper(int port) {
    server = EthernetServer(port);
}

void ServerHelper::init() {
    Serial.println("Initializing server...");

    byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0xDF, 0xAB};
    Ethernet.begin(mac);

    Serial.print("Server successfully started on: ");
    Serial.println(Ethernet.localIP());

    server.begin();
}

void ServerHelper::handleConnexion() {
    EthernetClient client = server.available();
    Router router;

    if (client) {
        if (client.connected()) {
            String httpHeader;
            while (client.available()) {
                char c = client.read();
                httpHeader += c;
            }

            client.print(buildJsonResponse(router.route(httpHeader)));

            client.stop();
        }
    }
}

String ServerHelper::buildJsonResponse(String json) {
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