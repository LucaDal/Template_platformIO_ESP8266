#ifndef MyFirmware_H_
#define MyFirmware_H_

#include <Arduino.h>

struct firmware_t {
  String version;
  String md5_checksum;
};

typedef struct firmware_t Firmware;

#endif
