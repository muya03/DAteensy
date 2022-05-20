#include "SteeringWheel.h"
HardwareSerial* ser;

void initScreen(HardwareSerial &_serial){
    ser = &_serial;
    ser->begin(115200);

}

void endMessage(){
    byte a = 0xff;
    for ( int i = 0; i < 3; i ++ ){
    ser->write(a);
    }
}


void sendGear(int str) {
    
    ser->write("rpm.txt=\"%f\"",str);
    endMessage();
}

void sendRPM(int str) {
    
    ser->write("rpm.txt=\"%f\"",str);
    endMessage();

}