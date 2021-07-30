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
    EepromSettings() {
        EEPROM.begin(getEepromLength());
        delay(10);
        EEPROM.commit();
    }

    String getSsidKey() {
        return SSID_KEY;
    }

    String getSsidPasswordKey() {
        return SSID_PASS_KEY;
    }

    String getValue(String key) {
        String value = "";
        int startFrom = getStartFromEeprom(key);
        if (startFrom != -1) {
            int endFrom = startFrom + settings[i].length;
            for (int j = startFrom; j < endFrom; ++j)
            {
                if (EEPROM.read(i) != NULL) {
                    value += char(EEPROM.read(i));
                }
            }
        }
        return value;
    }

    bool setValue(String key, String value) {
        String startFrom = getStartFromEeprom(key);
        if (startFrom != -1) {
            for (int i = 0; i < value.length(); ++i)
            {
                int position = startFrom + i;
                EEPROM.write(position, value[i]);
                Serial.println(value[i]);
            }
            return true;
        }
        return false;
    }

    void hardReset() {
        for (int i = 0 ; i < EEPROM.length() ; i++) {
          EEPROM.write(i, 0);
        }
    }

protected:
    int getStartFromEeprom(String key) {
        int startFrom = 0;
        for (int i = 0; i <= SETTINGS_SIZE; i++) {
            if (settings[i].key == key) {
                return startFrom;
            } else {
                startFrom += settings[i].length;
            }
        }
        return -1;
    }

    int getEepromLength() {
        int length = 0;
        for (int i = 0; i <= SETTINGS_SIZE; i++) {
            length += settings[i].length;
        }
        return length;
    }
};

#endif