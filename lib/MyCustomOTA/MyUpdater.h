#ifndef MYUPDATER_H
#define MYUPDATER_H

#include "MyUpdater.h"
#include <ESP8266httpUpdate.h>

class MyUpdater{
    public:
        MyUpdater(String md5Checksum);
        bool startUpdate(HTTPClient &https, String currentFirmwareVersion);
};

#endif