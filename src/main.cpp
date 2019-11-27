#include <Arduino.h>
#include "RestApi.h"

int LED = 4;

RestApi restApi(80);

String led(char *params = "") {
    if (String(params).indexOf("on") != -1) {
        digitalWrite(LED, HIGH);
        return String(R"({"led": 1})");
    } else {
        digitalWrite(LED, LOW);
        return String(R"({"led": 0})");
    }
}

void setup() {
    // Start serial monitor
    Serial.begin(9600);

    // Define pins here
    pinMode(LED, OUTPUT);

    // Initialize server
    restApi.init();

    // Add routes
    restApi.addRoute(GET, "/led", &led);
}

void loop() {
    // Handle HTTP Connections
    restApi.run();
}
