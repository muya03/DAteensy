#ifndef SDSTORE
#define SDSTORE

#include <Arduino.h>
#include <SD.h>

void initSD();
void saveLine(String data);

String storeFile;



#endif