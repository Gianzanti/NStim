void readConfigFromEEPROM() {
  byte valueRead;

  // READ FREQUENCY
  valueRead = EEPROM.read(FREQUENCY_ADDRESS);
  if (valueRead > 0 && valueRead < 100) {
    STIM_FREQUENCY = valueRead;
#if defined(DEBUG) && defined(DETAIL)
    Serial.print("FREQUENCY: [ADD:" + String(FREQUENCY_ADDRESS) + "]:");
    Serial.print("\t");
    Serial.print(String(valueRead, DEC) + " Hz");
    Serial.println("");
#endif
  } else {
    // setting default value
    STIM_FREQUENCY = 50;
    EEPROM.write(FREQUENCY_ADDRESS, STIM_FREQUENCY);
  }

  // READ PULSE WIDTH
  valueRead = EEPROM.read(PULSE_WIDTH_ADDRESS);
  if (valueRead > 0 && valueRead < 100) {
    STIM_PULSE_WIDTH = valueRead;
#if defined(DEBUG) && defined(DETAIL)
    Serial.print("PULSE WIDTH: [ADD:" + String(PULSE_WIDTH_ADDRESS) + "]:");
    Serial.print("\t");
    Serial.print(String(valueRead, DEC) + " ms");
    Serial.println("");
#endif

  } else {
    // setting default value
    STIM_PULSE_WIDTH = 50;
    EEPROM.write(PULSE_WIDTH_ADDRESS, STIM_PULSE_WIDTH);
  }

  // READ CURRENT
  valueRead = EEPROM.read(CURRENT_ADDRESS);
  if (valueRead > 0 && valueRead < 100) {
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
  if (valueRead > 0 && valueRead < 10) {
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
    STIM_LANGUAGE = (valueRead == 1);
#if defined(DEBUG) && defined(DETAIL)
    Serial.print("LANGUAGE: [ADD:" + String(LANGUAGE_ADDRESS) + "]:");
    Serial.print("\t");
    Serial.print(String(valueRead, DEC));
    Serial.println("");
#endif
  } else {
    // setting default value
    STIM_LANGUAGE = 0;
    EEPROM.write(LANGUAGE_ADDRESS, STIM_LANGUAGE);
  }

  // READ SOUND LEVEL
  valueRead = EEPROM.read(SOUNDLEVEL_ADDRESS);
  if (valueRead > 0 && valueRead < 100) {
    STIM_SOUNDLEVEL = valueRead;
#if defined(DEBUG) && defined(DETAIL)
    Serial.print("SOUND LEVEL: [ADD:" + String(SOUNDLEVEL_ADDRESS) + "]:");
    Serial.print("\t");
    Serial.print(String(valueRead, DEC));
    Serial.println("");
#endif

  } else {
    // setting default value
    STIM_SOUNDLEVEL = 3;
    EEPROM.write(SOUNDLEVEL_ADDRESS, STIM_SOUNDLEVEL);
  }

  // READ BACKLIGHT
  valueRead = EEPROM.read(BACKLIGHT_ADDRESS);
  if (valueRead > 0 && valueRead < 100) {
    STIM_BACKLIGHT = valueRead;
#if defined(DEBUG) && defined(DETAIL)
    Serial.print("BACKLIGHT: [ADD:" + String(BACKLIGHT_ADDRESS) + "]:");
    Serial.print("\t");
    Serial.print(String(valueRead, DEC));
    Serial.println("");
#endif
  } else {
    // setting default value
    STIM_BACKLIGHT = 3;
    EEPROM.write(BACKLIGHT_ADDRESS, STIM_BACKLIGHT);
  }

  EEPROM_READ = true;
}
