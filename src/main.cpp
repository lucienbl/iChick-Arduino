#include <Arduino.h>
#include "RestApi.h"
#include <dht11.h>

#define WATER_SENSOR A0
#define LED 4
#define DHT11_PIN 3

RestApi restApi(80);
dht11 DHT11;

String led(char *params = "") {
    if (String(params).indexOf("on") != -1) {
        digitalWrite(LED, HIGH);
        return String(R"({"led": 1})");
    } else {
        digitalWrite(LED, LOW);
        return String(R"({"led": 0})");
    }
}

String waterLevel(char *params = "") {
    return "{\"value\": " + String(analogRead(WATER_SENSOR)) + "}";
}

String dht11(char *params = "") {
    return "{\"humidity\": " + String((float) DHT11.humidity, 2) + ", \"temperature\": " + String((float) DHT11.temperature, 2) + "}";
}

void setup() {
    // Start serial monitor
    Serial.begin(9600);

    // Initialize server
    restApi.init();

    // Add routes
    restApi.addRoute(GET, "/led", &led);
    restApi.addRoute(GET, "/waterLevel", &waterLevel);
    restApi.addRoute(GET, "/dht11", &dht11);
}

void loop() {
    // Handle HTTP Connections
    restApi.run();

    // Read temp
    DHT11.read(DHT11_PIN);
}
