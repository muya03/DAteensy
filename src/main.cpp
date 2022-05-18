#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "lib/OBD2/OBD2.h"
#include "lib/persistence/persistance.h"
#include "settings.h"

#include <SD.h>

OBD2sensordata OBD2db = {0};


String storeFile = "";
String storeLine = "";

void setup() {
    // init serial
    #ifdef DEBUG
    Serial.begin(115200); 
    #endif

    initOBD2(OBD2db);



  // Initialize SD 
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
  } else{
  Serial.println("card initialized.");
  }

    // set store filename
    
    uint32_t current_time = readCounter(EEPROM_base_address);
    storeFile = String(current_time);
    String originalName = String(current_time);
    
    char buffer[32];
    storeFile.toCharArray(buffer, 32);

    uint8_t i = 1;
//    while (SD.exists(buffer)){
    while (true) {
            Serial.println(storeFile);
            delay(1000);
            storeFile = originalName + "." + i;

            storeFile.toCharArray(buffer, 32);
        i++;
    }

    //clear eeprom
}


uint32_t elapsed_minute = 0;
uint32_t elapsed_second = 0;
uint32_t elapsed_100ms = 0;

void loop() {
    //OBD2events();


    // execute each second
    if (millis() - elapsed_second > 1000){
        elapsed_second = millis();
    }

    // execute each minute
    if (millis() - elapsed_minute > 60* 1000){

        Serial.println(readCounter(EEPROM_base_address));
        Serial.println("---------------------");
        increaseTimeCounter(EEPROM_base_address);

        elapsed_minute = millis();
    }

}