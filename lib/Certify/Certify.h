#ifndef CERTIFY_H
#define CERTIFY_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//certify that the connection has it's fingerprint and is verified.
class Certify {
    private:
        const char *host = "";
        char *fingerPrint = NULL;
        const char * getFingerprintFromServer();
    public:
        Certify(const char *host);
        const char * getFingerprint();
        const char * getNewFingerprint();
};

#endif