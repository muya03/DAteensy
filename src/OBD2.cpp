#include "OBD2.h"

uint8_t idList[17] = {0x01, 0x03, 0x04, 0x05, 0x07, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x11, 0x14, 0x21, 0x33, 0x42, 0x45, 0x4d };
uint8_t idIndex = 0;
uint32_t time_received = 0;


FlexCAN_T4<CAN, RX_SIZE_256, TX_SIZE_16> *_can;
OBD2sensordata *_db;


void initOBD2(FlexCAN_T4<CAN, RX_SIZE_256, TX_SIZE_16> &can, OBD2sensordata &database){
    // passed by reference, so other parts of the  code can access
    _can = &can;
    _db = &database;


    _can->begin();
    _can->setBaudRate(500000);
    _can->setMaxMB(20);
    _can->enableFIFO();
    _can->enableFIFOInterrupt();
    _can->onReceive(FIFO,receivedOBD2callback);

    // set up FIFO filter (OBD2)

    _can->setFIFOFilter(REJECT_ALL);
    _can->setFIFOFilter(0, 0x7E8, STD);
}



void askPID(uint8_t id){ 
    CAN_message_t askPID;
    askPID.id = 0x7DF;

    askPID.buf[0] = 0x02; // message length
    askPID.buf[1] = 0x01; // service 0x01 (current data)
    askPID.buf[2] = id;

    askPID.len = 8;

    _can->write(askPID);

}

void receivedOBD2callback(const CAN_message_t &msg){
    uint8_t length = msg.buf[0];
    uint8_t service = msg.buf[1]; // 41: actual data, 43: dtc
    uint8_t pid = msg.buf[2];
    uint8_t A = msg.buf[3];
    uint8_t B = msg.buf[4];

    if (service == 0x41){ // we've got a response
        switch(pid){
            case MONITOR_STATUS_SINCE_DTCS_CLEARED:
                _db->MIL_on =  A & 0b10000000 ;
                _db->DTC_CNT = B & 0b01111111 ;
                break;

        }


    }


    askPID(idList[idIndex]);
    idIndex = (idIndex + 1) % sizeof(idList);
    time_received = millis();
}

void OBD2events(){
    // process can callbacks
    _can->events();

    // restart asking if not response 
    if (millis()-time_received > 1000){
        #ifdef DEBUG
        Serial.println("Failed to receive response, restarting");
        #endif

        askPID(MONITOR_STATUS_SINCE_DTCS_CLEARED);
        time_received = millis();
    }
}