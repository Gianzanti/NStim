
void readConfigFromEEPROM() {
  byte FREQUENCY_ADDRESS = 0x02;
  byte PULSE_WIDTH_ADDRESS = 0x03;
  byte BIPOLAR_ADDRESS = 0x04;
  byte TRAIN_ADDRESS = 0x05;
  byte TRAIN_COUNT_ADDRESS = 0x06;
  byte NEGATIVE_PHASE_ADDRESS = 0x07;
  byte TYPE_CORTICAL_ADDRESS = 0x08;
  byte CURRENT_ADDRESS = 0x09;

  byte valueRead;
  
  // READ FREQUENCY
  valueRead = EEPROM.read(FREQUENCY_ADDRESS);
  if (valueRead > 0 && valueRead < 100) {
    STIM_FREQUENCY = valueRead;
    debugPrint("FREQUENCY: [ADD:" + String(FREQUENCY_ADDRESS) + "]:");
    debugPrint("\t");
    debugPrint(String(valueRead, DEC) + " Hz");
    debugPrintln("");
  } else {
    // setting default value
    STIM_FREQUENCY = 50;
    EEPROM.write(FREQUENCY_ADDRESS, STIM_FREQUENCY);
  }
  
  // READ PULSE WIDTH
  valueRead = EEPROM.read(PULSE_WIDTH_ADDRESS);
  if (valueRead > 0 && valueRead < 100) {
    STIM_PULSE_WIDTH = valueRead;
    debugPrint("PULSE WIDTH: [ADD:" + String(PULSE_WIDTH_ADDRESS) + "]:");
    debugPrint("\t");
    debugPrint(String(valueRead, DEC) + " ms");
    debugPrintln("");
  } else {
    // setting default value
    STIM_PULSE_WIDTH = 50;
    EEPROM.write(PULSE_WIDTH_ADDRESS, STIM_PULSE_WIDTH);
  }

  // READ CURRENT
  valueRead = EEPROM.read(CURRENT_ADDRESS);
  if (valueRead > 0 && valueRead < 100) {
    STIM_CURRENT = valueRead;
    debugPrint("CURRENT: [ADD:" + String(CURRENT_ADDRESS) + "]:");
    debugPrint("\t");
    debugPrint(String(valueRead, DEC) + " mA");
    debugPrintln("");
  } else {
    // setting default value
    STIM_CURRENT = 5;
    EEPROM.write(CURRENT_ADDRESS, STIM_CURRENT);
  }

  // READ STIM_TRAIN_COUNT
  valueRead = EEPROM.read(TRAIN_COUNT_ADDRESS);
  if (valueRead > 0 && valueRead < 10) {
    STIM_TRAIN_COUNT = valueRead;
    debugPrint("TRAIN COUNT: [ADD:" + String(TRAIN_COUNT_ADDRESS) + "]:");
    debugPrint("\t");
    debugPrint( String(valueRead, DEC) );
    debugPrintln("");
  } else {
    // setting default value
    STIM_TRAIN_COUNT = 3;
    EEPROM.write(TRAIN_COUNT_ADDRESS, STIM_TRAIN_COUNT);
  }

  // READ STIM_BIPOLAR
  valueRead = EEPROM.read(BIPOLAR_ADDRESS);
  if (valueRead == 0 || valueRead == 1) {
    STIM_BIPOLAR = (valueRead == 1);
    debugPrint("BIPOLAR MODE: [ADD:" + String(BIPOLAR_ADDRESS) + "]:");
    debugPrint("\t");
    debugPrint( String(STIM_BIPOLAR) );
    debugPrintln("");
  } else {
    // setting default value
    STIM_BIPOLAR = 0;
    EEPROM.write(BIPOLAR_ADDRESS, STIM_BIPOLAR);
  }

  // READ STIM_TRAIN
  valueRead = EEPROM.read(TRAIN_ADDRESS);
  if (valueRead == 0 || valueRead == 1) {
    STIM_TRAIN = (valueRead == 1);
    debugPrint("TRAIN PULSE MODE: [ADD:" + String(TRAIN_ADDRESS) + "]:");
    debugPrint("\t");
    debugPrint( String(STIM_TRAIN) );
    debugPrintln("");
  } else {
    // setting default value
    STIM_TRAIN = 0;
    EEPROM.write(TRAIN_ADDRESS, STIM_TRAIN);
  }

  // READ STIM_NEGATIVE_PHASE
  valueRead = EEPROM.read(NEGATIVE_PHASE_ADDRESS);
  if (valueRead == 0 || valueRead == 1) {
    STIM_NEGATIVE_PHASE = (valueRead == 1);
    debugPrint("NEGATIVE PHASE: [ADD:" + String(NEGATIVE_PHASE_ADDRESS) + "]:");
    debugPrint("\t");
    debugPrint( String(STIM_NEGATIVE_PHASE) );
    debugPrintln("");
  } else {
    // setting default value
    STIM_NEGATIVE_PHASE = 0;
    EEPROM.write(NEGATIVE_PHASE_ADDRESS, STIM_NEGATIVE_PHASE);
  }

  // READ STIM_CORTICAL
  valueRead = EEPROM.read(TYPE_CORTICAL_ADDRESS);
  if (valueRead == 0 || valueRead == 1) {
    STIM_CORTICAL = (valueRead == 1);
    debugPrint("CORTICAL MODE: [ADD:" + String(TYPE_CORTICAL_ADDRESS) + "]:");
    debugPrint("\t");
    debugPrint( String(STIM_CORTICAL) );
    debugPrintln("");
  } else {
    // setting default value
    STIM_CORTICAL = 0;
    EEPROM.write(TYPE_CORTICAL_ADDRESS, STIM_CORTICAL);
  }

  EEPROM_READ = true;
}
