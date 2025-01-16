#include"checksum8.h"
#define START_BYTE_1 0x20
#define START_BYTE_2 0x40

#define START_SIZE 2
#define SYSTEM_ID_AND_COMPONENT_ID 3
#define MESSAGE_TYPE 2
#define SIZE_BYTE 1
#define CHECKSUM_BYTTE 2

#define HEADER_BYTE_1_POS 0
#define HEADER_BYTE_2_POS 1
#define SYSID_BYTE_POS 2
#define SYSID_BYTE_POS_AND_CMPID_BYTE_POS 3
#define CMPID_BYTE_POS 4
#define MSGID_BYTE_1_POS 5
#define MSGID_BYTE_2_POS 6
#define PAYLOAD_SIZE_BYTE_POS 7

#define KBUS_MAX_PAYLOAD_LEN 255

#define KBUS_NON_PAYLOAD_BYTE (START_SIZE+SYSTEM_ID_AND_COMPONENT_ID+MESSAGE_TYPE+SIZE_BYTE+CHECKSUM_BYTTE)
#define KBUS_MAX_PACKET_LEN (KBUS_MAX_PAYLOAD_LEN+KBUS_NON_PAYLOAD_BYTE)


uint8_t error = 1;


void kbus::feed(uint8_t data,uint8_t* frame) {
  error = 0B1;
  static uint8_t frame_length = 0;
  static uint8_t frame_index = 0;
  if (frame_index == 0 && data != START_BYTE_1) {
    frame_index = 0;
    
    error |= 0B10;
    return;
  }
  if (frame_index == 1 && data == START_BYTE_1) {
    frame_index = 1;
    error |= 0B100;
    return;
  }
  if (frame_index == 1 && data != START_BYTE_2) {
    frame_index = 0;
    error |= 0B1000;
    return;
  }
  if (frame_index == PAYLOAD_SIZE_BYTE_POS) {//size index
    frame_length = data + KBUS_NON_PAYLOAD_BYTE-1;
  }

  frame[frame_index++] = data;


  if (frame_length > 0) {
    if (frame_index > frame_length) {
      uint16_t sum = (frame[frame_length - 1] << 8) | frame[frame_length];
      if (checksum8(frame, frame_length - 2) == sum) {
        error = 0;
      } else {
        error |= 0B10000;
      }
      frame_length = 0;
      frame_index = 0;
    }
  } else if (frame_index > PAYLOAD_SIZE_BYTE_POS) {
    frame_index = 0;
    frame_length = 0;
  }
}