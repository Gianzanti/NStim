void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_8, OUTPUT);
  pinMode(LED_7, OUTPUT);
  pinMode(LED_6, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(INPUT_ROTARY_CLK, INPUT);
  pinMode(INPUT_ROTARY_DAT, INPUT);

  analogWrite(LED_R, 125);
  analogWrite(LED_G, 125);
  analogWrite(LED_B, 125);
  digitalWrite(LED_8, HIGH);
  digitalWrite(LED_7, HIGH);
  digitalWrite(LED_6, HIGH);
  digitalWrite(LED_5, HIGH);
  digitalWrite(LED_4, HIGH);
  digitalWrite(LED_3, HIGH);

  Wire.begin();                // join i2c bus with address I2C_SLAVE_ADDRESS

//  softSerial.begin(BAUD_RATE);
//  delay(3000);  
  Serial.begin(BAUD_RATE);
  delay(3000);  
//  Lcm.begin();
//  delay(3000);

//  int version = Lcm.readFirmwareVersion();
//  debugPrintln("[INFO] LCM Firmware version:" + String(version));

  // setup serial monitor

  // Serial.println(saida);

  // int version = Lcm.readFirmwareVersion();
  // delay(1000);
  // Serial.println(version);

  // Lcm.changeBacklight(50);
  // delay(1000);

  // Lcm.changePicId(0);
  // delay(1000);

  // delay(2000);
  // Lcm.changeBacklight(10);
  // Lcm.changePicId(0);

  // Serial.println("LCM Set");
  // setup I2C
  // Wire.begin();

  // fsm.add_transition(&state_light_on, &state_light_off,FLIP_LIGHT_SWITCH,&on_trans_light_on_light_off);
  // fsm.add_transition(&state_light_off, &state_light_on,FLIP_LIGHT_SWITCH,&on_trans_light_off_light_on);

  btnStart->setActionDown(turnOnLed);
  btnStop->setActionDown(turnOffLed);
  rtrEncoder->setup();
  rtrEncoder->setActionCW(CW);
  rtrEncoder->setActionCCW(CCW);  
 
  currentState = SETUP_DONE;
};
