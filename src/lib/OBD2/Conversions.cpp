#include "OBD2.h"
uint16_t OBD2RPM(OBD2sensordata db){
    return (db.engine_rpmA*256+db.engine_rpmB)/4;
}

float OBD2TMP(uint8_t tmp){
        // coolant Temp
    return float(tmp) - 40.0;
}