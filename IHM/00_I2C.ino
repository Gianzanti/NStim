byte I2C_CommIndex = 0;
bool I2C_CommStarted = false;
bool I2C_CommFinished = true;

void I2C_receivedData() {
#if defined(I2C_ENABLED)
    while (Wire.available()) {
        byte value = Wire.read();
        if (!I2C_CommStarted && I2C_CommFinished && value == 255) {
            I2C_CommIndex = 1;
            I2C_CommStarted = true;
            I2C_CommFinished = false;
        } else if (I2C_CommIndex > 4 && I2C_CommStarted) {
            I2C_CommFinished = true;
        }

        switch (I2C_CommIndex) {
            case 1:  // ERROR
                STIM_ERROR = Wire.read();
                if (STIM_ERROR > 0) {
                    nextState = WITH_ERROR;
                }
                I2C_CommIndex++;
                break;

            case 2:  // STIMON
                STIM_ON = (Wire.read() == 1);
                I2C_CommIndex++;
                break;

            case 3:  // STIM_MEASURED_BATTERY
                STIM_MEASURED_BATTERY = Wire.read();
                I2C_CommIndex++;
                break;

            case 4:  // STIM_MEASURED_CURRENT
                STIM_MEASURED_CURRENT = Wire.read();
                I2C_CommIndex++;
                break;

            default:
                break;
        }
    }

    if (I2C_CommFinished) {
        // tratar erro
        I2C_CommIndex = 0;
        I2C_CommStarted = false;
    }
#endif
}

void I2C_requestedData() {
    // mount state byte
    byte BYTE_1 = 0b00000000;
    // BIT 7- STIM_ON (0 - OFF | 1 - ON)
    // BIT 5- STIM_BIPOLAR ( 0 - MONOPOLAR | 1 - BIPOLAR)
    // BIT 3- STIM_NEGATIVE_PHASE ( 0 - POSTIIVE | 1 - NEGATIVE)
    // BIT 1- STIM_TRAIN  ( 0 - UNIQUE PULSE | 1 - PULSE TRAIN)
    BYTE_1 |= STIM_ON << 7;
    BYTE_1 |= STIM_BIPOLAR << 5;
    BYTE_1 |= STIM_NEGATIVE_PHASE << 3;
    BYTE_1 |= STIM_TRAIN << 1;

    // BYTE 2 -> Current (1 to 200) will be divided by 10
    byte BYTE_2 = STIM_CURRENT;
    // BYTE 3 -> Frequency MSB (1 to 1000)
    byte BYTE_3 = (byte)((STIM_FREQUENCY >> 8) & 0xFF);
    // BYTE 4 -> Frequency LSB (1 to 1000)
    byte BYTE_4 = (byte)(STIM_FREQUENCY & 0xFF);
    // BYTE 5 -> Pulse Width (1 a 99) will be divided by 10
    byte BYTE_5 = STIM_PULSE_WIDTH;
    // BYTE 6 -> Train Pulses (1 a 10)
    byte BYTE_6 = STIM_TRAIN_COUNT;
    // BYTE 7 -> Train Interval MSB(1 a 1000)
    byte BYTE_7 = (byte)((STIM_TRAIN_INTERVAL >> 8) & 0xFF);
    // BYTE 8 -> Train Interval LSB(1 a 1000)
    byte BYTE_8 = (byte)(STIM_TRAIN_INTERVAL & 0xFF);

#if defined(I2C_ENABLED)
    Wire.write(0b11111111);
    Wire.write(BYTE_1);
    Wire.write(BYTE_2);
    Wire.write(BYTE_3);
    Wire.write(BYTE_4);
    Wire.write(BYTE_5);
    Wire.write(BYTE_6);
    Wire.write(BYTE_7);
    Wire.write(BYTE_8);
#endif
}
