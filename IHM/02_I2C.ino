void setSlave(byte cmd, byte value) {
#if defined(I2C_ENABLED)
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write(cmd);
    Wire.write(value);
    Wire.endTransmission();
#endif
}

void askSlave(byte cmd) {
#if defined(I2C_ENABLED)
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write(cmd);
    Wire.endTransmission();
    Wire.requestFrom(I2C_SLAVE_ADDRESS, 2);

    if (I2C_ANSWER_SIZE <= Wire.available()) {  // if answer size bytes were received
        int retCmd = Wire.read();
        int readValue = 0x00;
        if (retCmd == cmd) {
            readValue = Wire.read();

            switch (retCmd) {
                case READ_STIM_MEASURED_CURRENT:
                    STIM_MEASURED_CURRENT = readValue;
#if defined(DEBUG) && defined(DETAIL)
                    Serial.println("READ_STIM_MEASURED_CURRENT: " + String(STIM_MEASURED_CURRENT));
#endif
                    break;

                case READ_STIM_MEASURED_IMPEDANCE:
                    STIM_MEASURED_IMPEDANCE = readValue;
#if defined(DEBUG) && defined(DETAIL)
                    Serial.println("READ_STIM_MEASURED_CURRENT: " + String(STIM_MEASURED_IMPEDANCE));
#endif
                    break;

                case READ_STIM_MEASURED_BATTERY:
                    STIM_MEASURED_BATTERY = readValue;
#if defined(DEBUG) && defined(DETAIL)
                    Serial.println("READ_STIM_MEASURED_CURRENT: " + String(STIM_MEASURED_BATTERY));
#endif
                    break;

                case READ_STIM_ERROR:
                    STIM_ERROR = readValue;
#if defined(DEBUG) && defined(DETAIL)
                    Serial.println("READ_STIM_ERROR: " + String(STIM_ERROR));
#endif
                    break;

                default:
#if defined(DEBUG) && defined(DETAIL)
                    Serial.println("UNRECOGNIZED REQUEST: " + String(retCmd));
#endif
                    break;
            }
        }
    }
#endif
}
