#ifndef PERSISTANCE
#define PERSISTANCE

#include <Arduino.h>
#include <EEPROM.h>
#include "settings.h"

uint32_t readCounter(uint32_t base_address);
void increaseTimeCounter(uint32_t base_address);


#endif