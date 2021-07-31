#ifndef EepromSettings_h
#define EepromSettings_h

#include <EEPROM.h>

typedef struct {
    String key;
    int length;
} settingsMap;

const String SSID_KEY = "ssid";
const String SSID_PASS_KEY = "ssid_pass";

// settings
const settingsMap settings[] {
    {SSID_KEY, 32},
    {SSID_PASS_KEY, 64}
};

const int SETTINGS_SIZE = sizeof(settings);

class EepromSettings {
public:

};

#endif
