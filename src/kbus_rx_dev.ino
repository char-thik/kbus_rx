#include "kbus.h"

// Header(2 Bytes), SYSTEM_ID_AND_COMPONENT_ID (3 Bytes), MESSAGE_TYPE (2 Bytes), SIZE_BYTE 1, Data(0-255 Bytes), CHECKSUM_BYTE (2 Bytes)

// #define PRINT
// uint8_t kbus_sample[12]={0x20,0x40,0x20,0x20,0x00,0x00,0xA0};
uint8_t kbus_sample[15]={0x20,0x40,0x20,0x20,0x00,0x00,0x00,0x05,'A','B','C','D','E',0x01,0xF4};
unsigned long timer,timer1;
int bytespersecond, bytespersecond1,is;
kbus kbus;
uint8_t frame[255] = { 0 };
void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  pinMode(13,1);
}

void loop() {
  if (millis()-timer1>=100) {

        
    timer1=millis();
    bytespersecond++;
    uint8_t temp = kbus_sample[is];
    is++;
    if(is>=15){
      is=0;
    }
    if(digitalRead(2)==HIGH){
      kbus.feed(temp,frame);
      digitalWrite(13,1);
    }
    else{
      digitalWrite(13,0);
    }
    
  }
  if (!kbus.geterror()) {
    Serial.print("HEADER : ");
    Serial.print(kbus.getframe(frame,0));
    Serial.write(',');
    Serial.print(kbus.getframe(frame,1));
    Serial.print("   11_BIT_SYSID : ");
    Serial.print(kbus.getframe(frame,2));
    Serial.print("   1_BIT_R/W : ");
    Serial.print(kbus.getframe(frame,3));
    Serial.print("   12_BIT_CMPID : ");
    Serial.write(kbus.getframe(frame,4));
    Serial.print("   16_BIT_MSGID : ");
    Serial.write(kbus.getframe(frame,5));
    Serial.write(kbus.getframe(frame,6));
    Serial.print("   SIZE : ");
    Serial.print(kbus.getsize(frame));  //Frame pos =7
    Serial.print("   DATA : ");
    for (int i = 0; i < kbus.getsize(frame); i++) {
      Serial.write(char(kbus.getframe(frame,8+i)));
    }
    Serial.print("   CHECKSUM : ");
    Serial.println((kbus.getchecksum(frame)),HEX);
  }
  int er = kbus.geterror();
  if (er & 0B10) {
    Serial.print("HEADER 1 error\n");
  }
  if (er & 0B100) {
    Serial.print("HEADER 1 repeat error\n");
  }
  if (er & 0B1000) {
    Serial.print("HEADER 2 error\n");
  }
  if (er & 0B10000) {
    Serial.print("CHECKSUM error\n");
  }
  if (millis() - timer >= 1000) {
    bytespersecond1 = bytespersecond;
    bytespersecond = 0;
    timer = millis();
  }
}
