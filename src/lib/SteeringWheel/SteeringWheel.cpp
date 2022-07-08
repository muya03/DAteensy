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

void rpmledInverse(int nleds){
  switch (nleds){
    case 0:
    analogWrite(33,255);
    break;
    case 1:
    analogWrite(33, 135);
    break;
    case 2:
    analogWrite(33,120);
    break;
    case 3:
    analogWrite(33,110);
    break;
    case 4:
    analogWrite(33,87);
    break;
    case 5:
    analogWrite(33,74);
    break;
    case 6:
    analogWrite(33, 55);
    break;
    case 7:
    analogWrite(33,40);
    break;
    case 8:
    analogWrite(33,30);
    break;
    case 9:
    analogWrite(33,10);
    break;
    case 10:
    analogWrite(33,0);
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

    if (str == 255){
        ser->printf("gear.txt=\"E\"");
        endMessage();
        return;
    }
    
    ser->printf("gear.txt=\"%d\"",str);
    endMessage();
}

void sendRPM(int str) {
    
    ser->printf("rpm.txt=\"%d\"",str);
    endMessage();

}

void sendCOLTMP(float str) {
    
    ser->printf("watertemp.txt=\"%1f\"",str);
    endMessage();

}

void sendTPS(float str){
    uint8_t str_int = round(str);
    ser->printf("throttlebar.val=%d",str_int);
    endMessage();
    ser->printf("throttle.txt=\"%2f\"%",str);
    endMessage();

}

void sendTrim1(float str){
    ser->printf("minifold1.txt=\"%3f\"",str);
    endMessage();

}

void sendTrim2(float str){
    ser->printf("minifold2.txt=\"%3f\"",str);
    endMessage();
}

void sendDTCcount(int str){
    ser->printf("dtc.txt=\"%d\"",str);
    endMessage();
}

void sendPressure1(int str){
    ser->printf("presion1.txt=\"%d\"",str);
    endMessage();

}

void sendPressure2(int str){
    ser->printf("presion2.txt=\"%d\"",str);
    endMessage();

}


void enable(){
    ser->printf("page 1");
    endMessage();
}

void disable(){
    ser->printf("page 0");
    endMessage();

}