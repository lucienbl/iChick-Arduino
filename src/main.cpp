#include <Arduino.h>
#include "RestApi.h"
#include <dht11.h>
#include <Wire.h>

// Ports
#define WATER_SENSOR A0
#define LED 4
#define DHT11_PIN 3

// Actions
#define ACTION_DOOR_OPEN 1
#define ACTION_DOOR_CLOSE 2

// Cards
#define CARD_MAIN_2 4

RestApi restApi(80);
dht11 DHT11;


void i2cTransmit(int address, int data) {
    Wire.beginTransmission(address);
    Wire.write(data);
    Wire.endTransmission();
}

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

String openDoor(char *params = "") {
    i2cTransmit(CARD_MAIN_2, ACTION_DOOR_OPEN);

    return String(R"({"success": true})");
}

String closeDoor(char *params = "") {
    i2cTransmit(CARD_MAIN_2, ACTION_DOOR_CLOSE);

    return String(R"({"success": true})");
}

void setup() {
    // Start serial monitor
    Serial.begin(9600);

    // Start card serial
    Wire.begin();

    // Initialize server
    restApi.init();

    // Add routes
    restApi.addRoute(GET, "/led", &led);
    restApi.addRoute(GET, "/waterLevel", &waterLevel);
    restApi.addRoute(GET, "/dht11", &dht11);
    restApi.addRoute(GET, "/openDoor", &openDoor);
    restApi.addRoute(GET, "/closeDoor", &closeDoor);
}

void loop() {
    // Handle HTTP Connections
    restApi.run();

    // Read temp
    DHT11.read(DHT11_PIN);
}
