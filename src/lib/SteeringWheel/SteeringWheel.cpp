#include "SteeringWheel.h"
HardwareSerial* ser;

void rpmled(int nleds){
  switch (nleds){
    case 0:
    analogWrite(33,0);
    break;
    case 1:
    analogWrite(33, 120);
    break;
    case 2:
    analogWrite(33,180);
    break;
    case 3:
    analogWrite(33,227);
    break;
    case 4:
    analogWrite(33,240);
    break;
    case 5:
    analogWrite(33,245);
    break;
    case 6:
    analogWrite(33, 248);
    break;
    case 7:
    analogWrite(33,250);
    break;
    case 8:
    analogWrite(33,252);
    break;
    case 9:
    analogWrite(33,253);
    break;
    case 10:
    analogWrite(33,255);
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
    
    ser->write("gear.txt=\"%d\"",str);
    endMessage();
}

void sendRPM(int str) {
    
    ser->write("rpm.txt=\"%d\"",str);
    endMessage();

}