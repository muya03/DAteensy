#include "OBD2.h"

uint8_t idList[17] = {0x01, 0x03, 0x04, 0x05, 0x07, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x11, 0x14, 0x21, 0x33, 0x42, 0x45, 0x4d };
uint8_t idIndex = 0;
uint32_t time_received = 0;


FlexCAN_T4<CAN, RX_SIZE_256, TX_SIZE_16> myCan;
OBD2sensordata *_db;


void initOBD2(OBD2sensordata &database){
    // passed by reference, so other parts of the  code can access
    _db = &database;


    myCan.begin();
    myCan.setBaudRate(500000);
    myCan.setMaxMB(20);
    myCan.enableFIFO();
    myCan.enableFIFOInterrupt();
    myCan.onReceive(FIFO,receivedOBD2callback);

    // set up FIFO filter (OBD2)
    myCan.setFIFOFilter(REJECT_ALL);
    myCan.setFIFOFilter(0, 0x7E8, STD);
}



void askPID(uint8_t id){ 
    CAN_message_t askPID;
    askPID.id = 0x7DF;

    askPID.buf[0] = 0x02; // message length
    askPID.buf[1] = 0x01; // service 0x01 (current data)
    askPID.buf[2] = id;

    askPID.len = 8;
    myCan.write(askPID);

}

void receivedOBD2callback(const CAN_message_t &msg){
    #ifdef DEBUG
 //   Serial.print("Callback reached: ");
    #endif
    uint8_t length = msg.buf[0];
    uint8_t service = msg.buf[1]; // 41: actual data, 43: dtc
    uint8_t pid = msg.buf[2];
    uint8_t A = msg.buf[3];
    uint8_t B = msg.buf[4];

    if (service == 0x41){ // we've got a response
        switch(pid){
            case MONITOR_STATUS_SINCE_DTCS_CLEARED:
                _db->MIL_on =  A & 0b10000000 ;
                _db->DTC_CNT = A & 0b01111111 ;
                break;

            case FUEL_SYSTEM_STATUS:
                _db->Fuel_system_status = A;
                break;

            case CALCULATED_ENGINE_LOAD:
                _db->Calculated_Engine_load = A;
                break;

            case ENGINE_COOLANT_TEMPERATURE:
            case LONG_TERM_FUEL_TRIM_BANK_1:
            case INTAKE_MANIFOLD_ABSOLUTE_PRESSURE:
            case ENGINE_RPM:
            case VEHICLE_SPEED:
            case TIMING_ADVANCE:
            case AIR_INTAKE_TEMPERATURE:
            case THROTTLE_POSITION:
            case OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM:

            case DISTANCE_TRAVELED_WITH_MIL_ON:
            case ABSOLULTE_BAROMETRIC_PRESSURE:

            case CONTROL_MODULE_VOLTAGE:
            case RELATIVE_THROTTLE_POSITION:
            case TIME_RUN_WITH_MIL_ON:     


        }
    }


    askPID(idList[idIndex]);
    idIndex = (idIndex + 1) % sizeof(idList);
    time_received = millis();
}

void OBD2events(){
    // process can callbacks
    myCan.events();

    // restart asking if not response 
    if (millis()-time_received > 1000){
        #ifdef DEBUG
        Serial.println("Failed to receive response, restarting");
        #endif

        askPID(MONITOR_STATUS_SINCE_DTCS_CLEARED);
        time_received = millis();
    }
}