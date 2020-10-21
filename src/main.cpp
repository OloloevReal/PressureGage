#include <Arduino.h>
#include <WiFi.h>
#include <utils.h>
#include <setup.h>


void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    print_wakeup_reason();  
}

void loop() {
}

