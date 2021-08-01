#ifndef EepromSettings_h
#define EepromSettings_h

#include <EEPROM.h>

class EepromSettings {
    typedef struct {
        String key;
        int length;
    } settingsMap;

    const String SSID_KEY = "ssid";
    const String SSID_PASS_KEY = "ssid_pass";

    // settings
    const settingsMap settings[2] {
        {SSID_KEY, 32},
        {SSID_PASS_KEY, 64}
    };

    const int SETTINGS_SIZE = sizeof(settings);

    static EepromSettings* _instance;
public:
    EepromSettings() {
        EEPROM.begin(getEepromLength());
        delay(10);
        EEPROM.commit();
    }

    static EepromSettings* instance()
    {
        if(!EepromSettings::_instance)
        {
            EepromSettings::_instance = new EepromSettings();
        }
        return EepromSettings::_instance;
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
            int endFrom = startFrom + startFrom;
            for (int i = startFrom; i < endFrom; ++i)
            {
                if (EEPROM.read(i) != NULL) {
                    value += char(EEPROM.read(i));
                }
            }
        }
        return value;
    }

    bool setValue(String key, String value) {
        int startFrom = getStartFromEeprom(key);
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

EepromSettings* EepromSettings::_instance;

#endif
