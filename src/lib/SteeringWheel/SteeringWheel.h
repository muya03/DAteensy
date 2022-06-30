#ifndef STEERINGWHEEL
#define STEERINGWHEEL

#include <Arduino.h>

void rpmled(int nleds);

void initScreen(HardwareSerial &_serial);

void sendGear(int str);
void sendRPM(int str);

int getGear();
#endif