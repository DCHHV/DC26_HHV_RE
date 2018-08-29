#include <TinyWireM.h>
#define dw digitalWrite
#define so sizeof

uint8_t EA=80; int i;
uint8_t SV[] = {0xff, 0xff, 0xff, 0x73, 0x65, 0x74, 0x20, 0x61, 0x64,
                0x64, 0x72, 0x65, 0x73, 0x73, 0x20, 0x30, 0x78, 0x35,
                0x41, 0x20, 0x74, 0x6f, 0x20, 0x30, 0x78, 0x32, 0x36}; // setup value
void s() {
  TinyWireM.begin();
  delay(100);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
}
void wE(int deviceaddress, unsigned int eeaddress, byte data ) {
  TinyWireM.beginTransmission(deviceaddress);
  TinyWireM.send((int)(eeaddress >> 8));
  TinyWireM.send((int)(eeaddress & 0xFF));
  TinyWireM.send(data);
  TinyWireM.endTransmission();
  delay(5);
}

byte rE(int deviceaddress, unsigned int eeaddress ) {
  byte rdata = 0xFF;
  TinyWireM.beginTransmission(deviceaddress);
  TinyWireM.send((int)(eeaddress >> 8));   // MSB
  TinyWireM.send((int)(eeaddress & 0xFF)); // LSB
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(deviceaddress,1);
  if (TinyWireM.available()) rdata = TinyWireM.receive();
  return rdata;
}
