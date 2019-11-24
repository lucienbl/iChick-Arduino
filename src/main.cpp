/*
  This a simple example of the aREST Library for Arduino (Uno/Mega/Due/Teensy)
  using the Ethernet library (for example to be used with the Ethernet shield).
  See the README file for more details.
  Written in 2014 by Marco Schwartz under a GPL license.
*/

// Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };
IPAddress ip(192,168,2,2);
EthernetServer server(80);

aREST rest = aREST();

// Variables to be exposed to the API
int temperature;
int humidity;

// Declare functions to be exposed to the API
int ledControl(String command);

void setup(void)
{
    Serial.begin(9600);

    // Init variables and expose them to REST API
    temperature = 24;
    humidity = 40;
    rest.variable("temperature", &temperature);
    rest.variable("humidity", &humidity);

    // Function to be exposed
    rest.function("led", ledControl);

    // Give name & ID to the device (ID should be 6 characters long)
    rest.set_id("1234");
    rest.set_name("Arduino");

    // Start the Ethernet connection and the server
    if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        Ethernet.begin(mac, ip);
    }
    server.begin();
    Serial.print("Server is at ");
    Serial.println(Ethernet.localIP());

    // pinModes
    pinMode(13, OUTPUT);

    // Start watchdog
    wdt_enable(WDTO_4S);
}

void loop() {
    EthernetClient client = server.available();
    rest.handle(client);
    wdt_reset();
}

int ledControl(String command) {
    int state = command.toInt();

    Serial.println(state);

    digitalWrite(13, state);
    return 1;
}