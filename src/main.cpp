#include <Arduino.h>

#include "Controller.h"

Controller* controller;

void setup() {
    Serial.begin(115200);
    Serial.flush();
    Serial.println("Version:");
    Serial.println(__DATE__ __TIME__);
    controller = new Controller();
}

void loop() {
    controller->loop();
}