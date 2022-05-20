#include <Arduino.h>

#include "lib/OBD2/OBD2.h"
#include "lib/persistence/persistance.h"
#include "lib/SD/SDstore.h"

#include "lib/SteeringWheel/SteeringWheel.h"

#include "settings.h"

OBD2sensordata OBD2db = {0};


void setup() {
    // init serial
    #ifdef DEBUG
    Serial.begin(115200); 
    #endif

    initScreen(ScreenUART);

    initOBD2(OBD2db);
    initSD();
}


uint32_t elapsed_minute = 0;
uint32_t elapsed_second = 0;
uint32_t elapsed_100ms = 0;

void loop() {
    // execute always

    OBD2events();

    // update screen
    sendRPM(OBD2RPM(OBD2db));
    // update rpm leds

    // execute each 100ms
    if (millis() - elapsed_100ms > 100){
        // print data to sd
        String to_save = "";
        to_save += OBD2toCSV(OBD2db);
        saveLine(to_save);
        elapsed_100ms = millis();
    }

    // execute each second
    if (millis() - elapsed_second > 1000){
        // send data over radio

        
        elapsed_second = millis();
    }

    // execute each minute
    if (millis() - elapsed_minute > 60* 1000){

        // increase time alive counter
        increaseTimeCounter(EEPROM_base_address);
        elapsed_minute = millis();
        // increase engine on time??

    }

}