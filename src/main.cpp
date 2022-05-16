#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "OBD2.h"
#include "settings.h"


FlexCAN_T4<CAN, RX_SIZE_256, TX_SIZE_16> myCAN;
OBD2sensordata OBD2db = {0};


void setup() {
    // init serial
    #ifdef DEBUG
    Serial.begin(115200); 
    #endif

    initOBD2(myCAN, OBD2db);

}

uint32_t elapsed = 0;

void loop() {

    OBD2events();

    // execute each second
    if (millis() - elapsed > 1000){
        Serial.print(OBD2db.DTC_CNT);
        if (OBD2db.MIL_on){
            Serial.println(" -> DTC ON");
        }      else{
            Serial.println(" -> DTC OFF");
        }
        elapsed = millis();
    }
}