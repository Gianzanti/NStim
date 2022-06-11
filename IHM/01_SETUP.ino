void setup() {
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    //  pinMode(LED_8, OUTPUT);
    //  pinMode(LED_7, OUTPUT);
    //  pinMode(LED_6, OUTPUT);
    //  pinMode(LED_5, OUTPUT);
    //  pinMode(LED_4, OUTPUT);
    //  pinMode(LED_3, OUTPUT);
    pinMode(INPUT_ROTARY_CLK, INPUT);
    pinMode(INPUT_ROTARY_DAT, INPUT);

    //  analogWrite(LED_R, 255);
    //  analogWrite(LED_G, 0);
    //  analogWrite(LED_B, 255);
    //  digitalWrite(LED_8, HIGH);
    //  digitalWrite(LED_7, HIGH);
    //  digitalWrite(LED_6, HIGH);
    //  digitalWrite(LED_5, HIGH);
    //  digitalWrite(LED_4, HIGH);
    //  digitalWrite(LED_3, HIGH);
    //  blueLED();

#if defined(I2C_ENABLED)
    Wire.begin();  // join i2c bus with address I2C_SLAVE_ADDRESS
#endif

#if defined(LCM_ENABLED)
    softSerial.begin(BAUD_RATE);
    delay(1000);
    Lcm.begin();
    delay(3000);
    int version = Lcm.readFirmwareVersion();
    Lcm.changePicId(PID_INTRO);

#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] LCM Firmware version:" + String(version));
#endif

#endif

    Serial.begin(BAUD_RATE);
    delay(1000);

    clearAllActions();
    currentState = LOADING_FROM_EPROM;

#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] NStim Setup Process Finished");
#endif
};
