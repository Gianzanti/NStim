void setup() {
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);

    pinMode(INPUT_ROTARY_CLK, INPUT);
    pinMode(INPUT_ROTARY_DAT, INPUT);

//    pinMode(SOFT_TX, OUTPUT);
//    pinMode(SOFT_RX, INPUT);

#if defined(I2C_ENABLED)
    Wire.begin(I2C_IHM_ADDRESS);        // join i2c bus with address I2C_IHM_ADDRESS
    Wire.onReceive(I2C_receivedData);   // interrupt handler for incoming messages
    Wire.onRequest(I2C_requestedData);  // register event
#endif

    Serial.begin(BAUD_RATE);
    
#if defined(LCM_ENABLED)
    softSerial.begin(BAUD_RATE);
    Lcm.begin();
    delay(1000);

    int version = Lcm.readFirmwareVersion();
    currentScreen = PID_INTRO;
    Lcm.changePicId(currentScreen);

#if defined(DEBUG)
    Serial.println("[INFO] LCM Firmware version:" + String(version));
#endif

#endif

    clearAllActions();
    currentState = LOADING_FROM_EPROM;

#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] NStim Setup Process Finished");
#endif
};
