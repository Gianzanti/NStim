// BYTE READ REGISTERS
#define READ_STIM_ON 0x01             // 1: stimulation is on | 0: stimulation is off
#define READ_STIM_FREQUENCY 0x02      // 0 - 100: operation frequency (Hz)
#define READ_STIM_PULSE_WIDTH 0x03    // 0 - 100: pulse width (s)
#define READ_STIM_BIPOLAR 0x04        // 0: monopolar mode | 1: bipolar mode
#define READ_STIM_TRAIN 0x05          // 0: pulse train disabled | 1: pulse train enabled
#define READ_STIM_TRAIN_COUNT 0x06    // 0 - 100: total number of pulses, if enabled
#define READ_STIM_NEGATIVE_PHASE 0x07  // 0: positive | 1: negative
#define READ_STIM_TYPE_CORTICAL 0x08  // 0: localization | 1: cortical
#define READ_STIM_CURRENT 0xA9        // 0 - 100: operation current (mA)

// BYTE SET REGISTERS
#define SET_STIM_ON 0xA1              // 1: stimulation is on | 0: stimulation is off
#define SET_STIM_FREQUENCY 0xA2       // 0 - 100: operation frequency (Hz)
#define SET_STIM_PULSE_WIDTH 0xA3     // 0 - 100: pulse width (ms)
#define SET_STIM_BIPOLAR 0xA4         // 0: monopolar mode | 1: bipolar mode
#define SET_STIM_TRAIN 0xA5           // 0: pulse train disabled | 1: pulse train enabled
#define SET_STIM_TRAIN_COUNT 0xA6     // 0 - 100: total number of pulses, if enabled
#define SET_STIM_NEGATIVE_PHASE 0xA7  // 0: positive | 1: negative
#define SET_STIM_TYPE_CORTICAL 0xA8   // 0: localization | 1: cortical
#define SET_STIM_CURRENT 0xA9         // 0 - 100: operation current (mA)

// BYTE COMMUNICATIONS
#define ERROR_NO_REGISTER 0xFF         // The register doesn't exist

void setSlave(byte cmd, byte value) {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(cmd);
  Wire.write(value);
  Wire.endTransmission();
}

void askSlave(byte cmd) {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(cmd);
  Wire.endTransmission();

  Wire.requestFrom(I2C_SLAVE_ADDRESS, 2);

  if (I2C_ANSWER_SIZE <= Wire.available()) { // if answer size bytes were received
    int retCmd = Wire.read();
    int readValue = 0x00;

    if (retCmd == cmd) {
      readValue = Wire.read();

      switch (retCmd) {
        case READ_STIM_ON:
          debugPrintln("READ_STIM_ON: " + String(readValue));
          break;

        case READ_STIM_FREQUENCY:
          debugPrintln("READ_STIM_FREQUENCY: " + String(readValue));
          break;

        case READ_STIM_PULSE_WIDTH:
          debugPrintln("READ_STIM_PULSE_WIDTH: " + String(readValue));
          break;

        case READ_STIM_BIPOLAR:
          debugPrintln("READ_STIM_MODE: " + String(readValue));
          break;

        case READ_STIM_TRAIN:
          debugPrintln("READ_STIM_TRAIN: " + String(readValue));
          break;

        case READ_STIM_TRAIN_COUNT:
          debugPrintln("READ_STIM_TRAIN_COUNT: " + String(readValue));
          break;

        case READ_STIM_NEGATIVE_PHASE:
          debugPrintln("READ_STIM_NEGATIVE_PHASE: " + String(readValue));
          break;

        case READ_STIM_TYPE_CORTICAL:
          debugPrintln("READ_STIM_TYPE_CORTICAL: " + String(readValue));
          break;
          
        default:
          debugPrintln("UNRECOGNIZED REQUEST: " + String(retCmd));
          break;
      }
    }
  }
}
