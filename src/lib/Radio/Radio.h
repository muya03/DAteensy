#ifndef RADIO
#define RADIO

#include <Arduino.h>
#include <CRC32.h>

typedef struct Packet {
uint32_t rpm;
uint32_t crc;
uint32_t end;
} Packet;


void initRadio(HardwareSerial &_serial);
void sendPacket(Packet packet);

#endif