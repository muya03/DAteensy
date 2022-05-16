#include <Arduino.h>
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> myCan;


void setup() {

    // CAN works at 500kb/s
    myCan.setBaudRate(500000);

    


}



void loop() {


}