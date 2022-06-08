#include "SteeringWheel.h"
HardwareSerial* ser;

void rpmled(int nleds){
  switch (nleds){
    case 0:
    analogWrite(33,0);
    break;
    case 1:
    analogWrite(33, 10);
    break;
    case 2:
    analogWrite(33,25);
    break;
    case 3:
    analogWrite(33,43);
    break;
    case 4:
    analogWrite(33,55);
    break;
    case 5:
    analogWrite(33,74);
    break;
    case 6:
    analogWrite(33, 87);
    break;
    case 7:
    analogWrite(33,100);
    break;
    case 8:
    analogWrite(33,117);
    break;
    case 9:
    analogWrite(33,130);
    break;
    case 10:
    analogWrite(33,150);
    break;


  }
}



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
    
    ser->printf("gear.txt=\"%d\"",str);
    endMessage();
}

void sendRPM(int str) {
    
    ser->printf("rpm.txt=\"%d\"",str);
    endMessage();

}