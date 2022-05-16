#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "OBD2.h"
#include "settings.h"



OBD2sensordata OBD2db = {0};


void setup() {
    // init serial
    #ifdef DEBUG
    Serial.begin(115200); 
    #endif

    initOBD2(OBD2db);

}

uint32_t elapsed_second = 0;
uint32_t elapsed_100ms = 0;
uint32_t counter = 0;
void loop() {
    OBD2events();



    // execute each second
    if (millis() - elapsed_second > 1000){
        printOBD2ALL(OBD2db);
        Serial.println("-----------------------------");
        Serial.println(counter);
        counter += 1;
        Serial.println("-----------------------------");

        elapsed_second = millis();
    }
}