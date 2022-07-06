#ifndef STEERINGWHEEL
#define STEERINGWHEEL

#include <Arduino.h>

void rpmled(int nleds);

void initScreen(HardwareSerial &_serial);

void sendGear(int str);
void sendRPM(int str);
void sendCOLTMP(float str);

void sendTPS(float str);
void sendTrim1(float str);
void sendTrim2(float str);
void sendDTCcount(int str);

void enable();
void disable();

#endif