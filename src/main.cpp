#include <Arduino.h>
#include "ServerHelper.h"

ServerHelper serverHelper(80);

void setup() {
    Serial.begin(9600);
    serverHelper.init();
}

void loop() {
    serverHelper.handleConnexion();
}
