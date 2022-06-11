void readConfigFromEEPROM() {
    byte valueRead;

    // READ FREQUENCY
    valueRead = EEPROM.read(FREQUENCY_ADDRESS);
    if (valueRead >= STIM_FREQUENCY_MIN && valueRead <= STIM_FREQUENCY_MAX) {
        STIM_FREQUENCY = valueRead;
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("FREQUENCY: [ADD:" + String(FREQUENCY_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC) + " Hz");
        Serial.println("");
#endif
    } else {
        // setting default value
        STIM_FREQUENCY = STIM_FREQUENCY_MAX / 2;
        EEPROM.write(FREQUENCY_ADDRESS, STIM_FREQUENCY);
    }

    // READ PERIOD
    valueRead = EEPROM.read(PERIOD_ADDRESS);
    if (valueRead > 0 && valueRead < 100) {
        STIM_PERIOD = valueRead;
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("PERIOD: [ADD:" + String(PERIOD_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC) + " ms");
        Serial.println("");
#endif

    } else {
        // setting default value
        STIM_PERIOD = 50;
        EEPROM.write(PERIOD_ADDRESS, STIM_PERIOD);
    }

    // READ CURRENT
    valueRead = EEPROM.read(CURRENT_ADDRESS);
    if (valueRead >= STIM_CURRENT_MIN && valueRead <= STIM_CURRENT_MAX) {
        STIM_CURRENT = valueRead;
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("CURRENT: [ADD:" + String(CURRENT_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC) + " mA");
        Serial.println("");
#endif

    } else {
        // setting default value
        STIM_CURRENT = 5;
        EEPROM.write(CURRENT_ADDRESS, STIM_CURRENT);
    }

    // READ STIM_TRAIN_COUNT
    valueRead = EEPROM.read(TRAIN_COUNT_ADDRESS);
    if (valueRead >= STIM_TRAIN_COUNT_MIN && valueRead <= STIM_TRAIN_COUNT_MAX) {
        STIM_TRAIN_COUNT = valueRead;
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("TRAIN COUNT: [ADD:" + String(TRAIN_COUNT_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC) + " pulses");
        Serial.println("");
#endif
    } else {
        // setting default value
        STIM_TRAIN_COUNT = 3;
        EEPROM.write(TRAIN_COUNT_ADDRESS, STIM_TRAIN_COUNT);
    }

    // READ STIM_TRAIN_INTERVAL
    valueRead = EEPROM.read(TRAIN_INTERVAL_ADDRESS);
    if (valueRead >= STIM_TRAIN_INTERVAL_MIN && valueRead <= STIM_TRAIN_INTERVAL_MAX) {
        STIM_TRAIN_INTERVAL = valueRead;
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("TRAIN INTERVAL: [ADD:" + String(TRAIN_INTERVAL_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC) + " pulses");
        Serial.println("");
#endif
    } else {
        // setting default value
        STIM_TRAIN_INTERVAL = STIM_TRAIN_INTERVAL_MAX / 2;
        EEPROM.write(TRAIN_INTERVAL_ADDRESS, STIM_TRAIN_INTERVAL);
    }

    // READ STIM_BIPOLAR
    valueRead = EEPROM.read(BIPOLAR_ADDRESS);
    if (valueRead == 0 || valueRead == 1) {
        STIM_BIPOLAR = (valueRead == 1);
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("BIPOLAR MODE: [ADD:" + String(BIPOLAR_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC));
        Serial.println("");
#endif
    } else {
        // setting default value
        STIM_BIPOLAR = 0;
        EEPROM.write(BIPOLAR_ADDRESS, STIM_BIPOLAR);
    }

    // READ STIM_TRAIN
    valueRead = EEPROM.read(TRAIN_ADDRESS);
    if (valueRead == 0 || valueRead == 1) {
        STIM_TRAIN = (valueRead == 1);
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("TRAIN PULSE MODE: [ADD:" + String(TRAIN_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC));
        Serial.println("");
#endif
    } else {
        // setting default value
        STIM_TRAIN = 0;
        EEPROM.write(TRAIN_ADDRESS, STIM_TRAIN);
    }

    // READ STIM_NEGATIVE_PHASE
    valueRead = EEPROM.read(NEGATIVE_PHASE_ADDRESS);
    if (valueRead == 0 || valueRead == 1) {
        STIM_NEGATIVE_PHASE = (valueRead == 1);
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("NEGATIVE PHASE: [ADD:" + String(NEGATIVE_PHASE_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC));
        Serial.println("");
#endif
    } else {
        // setting default value
        STIM_NEGATIVE_PHASE = 0;
        EEPROM.write(NEGATIVE_PHASE_ADDRESS, STIM_NEGATIVE_PHASE);
    }

    // READ STIM_CORTICAL
    valueRead = EEPROM.read(TYPE_CORTICAL_ADDRESS);
    if (valueRead == 0 || valueRead == 1) {
        STIM_CORTICAL = (valueRead == 1);
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("CORTICAL MODE: [ADD:" + String(TYPE_CORTICAL_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC));
        Serial.println("");
#endif

    } else {
        // setting default value
        STIM_CORTICAL = 0;
        EEPROM.write(TYPE_CORTICAL_ADDRESS, STIM_CORTICAL);
    }

    // READ LANGUAGE
    valueRead = EEPROM.read(LANGUAGE_ADDRESS);
    if (valueRead == 0 || valueRead == 1) {
        STIM_LANG_ENGLISH = (valueRead == 1);
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("LANGUAGE: [ADD:" + String(LANGUAGE_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC));
        Serial.println("");
#endif
    } else {
        // setting default value
        STIM_LANG_ENGLISH = 0;
        EEPROM.write(LANGUAGE_ADDRESS, STIM_LANG_ENGLISH);
    }

    // READ SOUND LEVEL
    valueRead = EEPROM.read(SOUNDLEVEL_ADDRESS);
    if (valueRead >= STIM_SOUNDLEVEL_MIN && valueRead <= STIM_SOUNDLEVEL_MAX) {
        STIM_SOUNDLEVEL = valueRead;
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("SOUND LEVEL: [ADD:" + String(SOUNDLEVEL_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC));
        Serial.println("");
#endif

    } else {
        // setting default value
        STIM_SOUNDLEVEL = STIM_SOUNDLEVEL_MAX/2;
        EEPROM.write(SOUNDLEVEL_ADDRESS, STIM_SOUNDLEVEL);
    }

    // READ BACKLIGHT
    valueRead = EEPROM.read(BACKLIGHT_ADDRESS);
    if (valueRead >= STIM_BACKLIGHT_MIN && valueRead <= STIM_BACKLIGHT_MAX) {
        STIM_BACKLIGHT = valueRead;
#if defined(DEBUG) && defined(DETAIL)
        Serial.print("BACKLIGHT: [ADD:" + String(BACKLIGHT_ADDRESS) + "]:");
        Serial.print("\t");
        Serial.print(String(valueRead, DEC));
        Serial.println("");
#endif
    } else {
        // setting default value
        STIM_BACKLIGHT = STIM_BACKLIGHT_MAX/2;
        EEPROM.write(BACKLIGHT_ADDRESS, STIM_BACKLIGHT);
    }

    // allow change in the current state
    nextState = MOUNT_SCREEN;
}
