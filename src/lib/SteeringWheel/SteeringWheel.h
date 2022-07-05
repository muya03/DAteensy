#ifndef STEERINGWHEEL
#define STEERINGWHEEL

#include <Arduino.h>

void rpmled(int nleds);

void initScreen(HardwareSerial &_serial);

void sendGear(int str);
void sendRPM(int str);
void sendCOLTMP(float str);
void sendAIRTMP(float str);

int getGear();
#endif