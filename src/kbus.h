#pragma once

#include <stdint.h>

class kbus {
public:
  void feed(uint8_t data,uint8_t* frame);
  uint8_t getframelength();
  uint8_t geterror();
  uint8_t getframe(uint8_t* frame, uint16_t pos);
  uint8_t getaddr(uint8_t* frame);
  uint8_t getsize(uint8_t* frame);
  uint16_t getchecksum(uint8_t* frame);
  
  
  uint16_t getSysid(uint8_t* frame);
  uint8_t getRW(uint8_t* frame);
  uint16_t getMsgid(uint8_t* frame);
  
};




#include "kbus.h"
#include "protocol.h"

uint8_t kbus::geterror() {
  uint8_t error_flag = error;//STATIC AAKKANAM
  if(!error){
    error = 0B1;
  }
  return error_flag;
}

// uint8_t kbus::done() {
//   uint8_t done_flag = 0;//STATIC AAKKANAM
//   if (!error) {
//     done_flag = 1;
//     error = 0B1;
//   }
//   return done_flag;
// }

uint8_t kbus::getframe(uint8_t* frame, uint16_t pos) {
  return frame[pos];
}
uint8_t kbus::getaddr(uint8_t* frame) {
  return frame[3];
}
uint8_t kbus::getsize(uint8_t* frame) {
  return frame[PAYLOAD_SIZE_BYTE_POS];
}
uint16_t kbus::getchecksum(uint8_t* frame) {
  return (frame[frame[PAYLOAD_SIZE_BYTE_POS]+KBUS_NON_PAYLOAD_BYTE - 2] << 8) | (frame[frame[PAYLOAD_SIZE_BYTE_POS]+KBUS_NON_PAYLOAD_BYTE-1]);
}

uint16_t kbus::getSysid(uint8_t* frame){
  return;
}
uint8_t kbus::getRW(uint8_t* frame){
  return;
}
uint16_t kbus::getMsgid(uint8_t* frame){
  return frame;
}