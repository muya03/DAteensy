#include "SDstore.h"
#include "lib/persistence/persistance.h"

String storeFile = "";

String generateFileName(){
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
}


void initSD(){
    if (!SD.begin(BUILTIN_SDCARD)) {
        #ifdef DEBUG
        Serial.println("Card failed, or not present");
    } else{
    Serial.println("card initialized.");
        #endif
    }

    storeFile = generateFileName();
        

}

