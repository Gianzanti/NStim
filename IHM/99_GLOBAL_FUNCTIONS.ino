void greenLED() {
    analogWrite(LED_R, 255);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 255);
}

void redLED() {
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 255);
    analogWrite(LED_B, 255);
}

void blueLED() {
    analogWrite(LED_R, 255);
    analogWrite(LED_G, 255);
    analogWrite(LED_B, 0);
}

void yellowLED() {
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 255);
}

void whiteLED() {
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);
}

void stopStimulation() {
    STIM_ON = false;
    setSlave(SET_STIM_ON, STIM_ON);
    greenLED();
};

void increaseCurrentAtStimulus(){
  if (STIM_CURRENT + 1 <= STIM_CURRENT_MAX) {
    STIM_CURRENT += 1;

#if defined(LCM_ENABLED)
    VP_SetCurrent.write(STIM_CURRENT);
#endif

    EEPROM.write(CURRENT_ADDRESS, STIM_CURRENT);
    setSlave(SET_STIM_CURRENT, STIM_CURRENT);

#if defined(DEBUG)
    Serial.println("[CHANGE] Increased current to: " + String(STIM_CURRENT));
#endif  
  }
};

void decreaseCurrentAtStimulus(){
  if (STIM_CURRENT - 1 >= STIM_CURRENT_MIN) {
    STIM_CURRENT -= 1;

#if defined(LCM_ENABLED)
    VP_SetCurrent.write(STIM_CURRENT);
#endif

    EEPROM.write(CURRENT_ADDRESS, STIM_CURRENT);
    setSlave(SET_STIM_CURRENT, STIM_CURRENT);

#if defined(DEBUG)
    Serial.println("[CHANGE] Increased current to: " + String(STIM_CURRENT));
#endif    
  }  
};

void startStimulation() {
    blueLED();
    STIM_ON = true;

#if defined(LCM_ENABLED)
    VP_StimulusState.write(STIM_ON);
#endif

    setSlave(SET_STIM_ON, STIM_ON);
};

void monitorSlave() {
  askSlave(READ_STIM_MEASURED_CURRENT);
//  askSlave(READ_STIM_MEASURED_IMPEDANCE);
//  askSlave(READ_STIM_MEASURED_BATTERY);
//  askSlave(READ_STIM_ERROR);
}

void startConfig() {
    whiteLED();
    STIM_ON = false;
    setSlave(SET_STIM_ON, STIM_ON);
        
    STIM_CONFIG = true;
    STIM_CURRENT_SCREEN = SCREEN_CONFIG_WAVE_POLE;

#if defined(LCM_ENABLED)
    VP_StimulusState.write(STIM_ON);
#endif

};

void changeWavePole(){
    STIM_BIPOLAR = !STIM_BIPOLAR;

#if defined(LCM_ENABLED)
    VP_WaveBipolar.write(STIM_BIPOLAR);
#endif
    
    EEPROM.write(BIPOLAR_ADDRESS, STIM_BIPOLAR);
    setSlave(SET_STIM_BIPOLAR, STIM_BIPOLAR);

#if defined(DEBUG)
    if (STIM_BIPOLAR) {
      Serial.println(F("[CHANGE] Changed Wave Pole to Bipolar"));
    } else {
      Serial.println(F("[CHANGE] Changed Wave Pole to Monopolar"));
    }
#endif

}

void nextWaveConfig(){
  if (STIM_CURRENT_SCREEN + 1 <= SCREEN_CONFIG_WAVE_EXIT) {
    STIM_CURRENT_SCREEN += 1;
  } else {
    STIM_CURRENT_SCREEN = SCREEN_CONFIG_WAVE_POLE;
  }
}

void changeWavePhase(){
    STIM_NEGATIVE_PHASE = !STIM_NEGATIVE_PHASE;
#if defined(LCM_ENABLED)
      VP_WavePhase.write(STIM_NEGATIVE_PHASE);
#endif   

    EEPROM.write(NEGATIVE_PHASE_ADDRESS, STIM_NEGATIVE_PHASE);
    setSlave(SET_STIM_NEGATIVE_PHASE, STIM_NEGATIVE_PHASE);

#if defined(DEBUG)
    if (STIM_NEGATIVE_PHASE) {
      Serial.println(F("[CHANGE] Changed Wave Phase to Negative"));
    } else {
      Serial.println(F("[CHANGE] Changed Wave Phase to Positive"));
    }
#endif

};

void changeWaveTrain(){
    STIM_TRAIN = !STIM_TRAIN;
#if defined(LCM_ENABLED)
      VP_TrainEnabled.write(STIM_TRAIN);
#endif   

    EEPROM.write(TRAIN_ADDRESS, STIM_TRAIN);
    setSlave(SET_STIM_TRAIN, STIM_TRAIN);

#if defined(DEBUG)
    if (STIM_TRAIN) {
      Serial.println(F("[CHANGE] Changed Wave Pulse Train to Enabled"));
    } else {
      Serial.println(F("[CHANGE] Changed Wave Pulse Train to Disabled"));
    }
#endif

};

void configCurrent() {
  STIM_CURRENT_SCREEN = SCREEN_CONFIG_CURRENT;
}

void configPeriod() {
  STIM_CURRENT_SCREEN = SCREEN_CONFIG_PERIOD;
}

void increaseCurrentAtConfig(){
  if (STIM_CURRENT + 1 <= STIM_CURRENT_MAX) {
    STIM_CURRENT += 1;

#if defined(LCM_ENABLED)
    VP_SetCurrent.write(STIM_CURRENT);
#endif

#if defined(DEBUG)
    Serial.println("[CHANGE] Increased current to: " + String(STIM_CURRENT));
#endif  
  }
};

void decreaseCurrentAtConfig(){
  if (STIM_CURRENT - 1 >= STIM_CURRENT_MIN) {
    STIM_CURRENT -= 1;

#if defined(LCM_ENABLED)
    VP_SetCurrent.write(STIM_CURRENT);
#endif

//    EEPROM.write(CURRENT_ADDRESS, STIM_CURRENT);
//    setSlave(SET_STIM_CURRENT, STIM_CURRENT);

#if defined(DEBUG)
    Serial.println("[CHANGE] Increased current to: " + String(STIM_CURRENT));
#endif    
  }
};

void setCurrentAtConfig(){

#if defined(LCM_ENABLED)
    VP_SetCurrent.write(STIM_CURRENT);
#endif

    EEPROM.write(CURRENT_ADDRESS, STIM_CURRENT);
    setSlave(SET_STIM_CURRENT, STIM_CURRENT);

#if defined(DEBUG)
    Serial.println("[SET] Set current to: " + String(STIM_CURRENT));
#endif

};

void configFrequency() {
  STIM_CURRENT_SCREEN = SCREEN_CONFIG_FREQUENCY;
}

void increasePeriodAtConfig(){
  if (STIM_PULSE_WIDTH + 1 <= STIM_PERIOD_MAX) {
    STIM_PULSE_WIDTH += 1;

#if defined(LCM_ENABLED)
    VP_SetPeriod.write(STIM_PULSE_WIDTH);
#endif

#if defined(DEBUG)
    Serial.println("[CHANGE] Increased Period to: " + String(STIM_PULSE_WIDTH));
#endif  
  }
};

void decreasePeriodAtConfig(){
  if (STIM_PULSE_WIDTH - 1 >= STIM_PERIOD_MIN) {
    STIM_PULSE_WIDTH -= 1;

#if defined(LCM_ENABLED)
    VP_SetPeriod.write(STIM_PULSE_WIDTH);
#endif

#if defined(DEBUG)
    Serial.println("[CHANGE] Increased Period to: " + String(STIM_PULSE_WIDTH));
#endif    
  }
};

void setPeriodAtConfig(){

#if defined(LCM_ENABLED)
    VP_SetPeriod.write(STIM_PULSE_WIDTH);
#endif

    EEPROM.write(PULSE_WIDTH_ADDRESS, STIM_PULSE_WIDTH);
    setSlave(SET_STIM_PULSE_WIDTH, STIM_PULSE_WIDTH);

#if defined(DEBUG)
    Serial.println("[SET] Set Period to: " + String(STIM_PULSE_WIDTH));
#endif

};






void configGeneral() {
  STIM_CURRENT_SCREEN = SCREEN_CONFIG_GENERAL;
}

void increaseFrequencyAtConfig(){
  if (STIM_FREQUENCY + 1 <= STIM_FREQUENCY_MAX) {
    STIM_FREQUENCY += 1;

#if defined(LCM_ENABLED)
    VP_SetFrequency.write(STIM_FREQUENCY);
#endif

#if defined(DEBUG)
    Serial.println("[CHANGE] Increased Frequency to: " + String(STIM_FREQUENCY));
#endif  
  }
};

void decreaseFrequencyAtConfig(){
  if (STIM_FREQUENCY - 1 >= STIM_FREQUENCY_MIN) {
    STIM_FREQUENCY -= 1;

#if defined(LCM_ENABLED)
    VP_SetFrequency.write(STIM_FREQUENCY);
#endif

#if defined(DEBUG)
    Serial.println("[CHANGE] Increased Frequency to: " + String(STIM_FREQUENCY));
#endif    
  }
};

void setFrequencyAtConfig(){

#if defined(LCM_ENABLED)
    VP_SetFrequency.write(STIM_FREQUENCY);
#endif

    EEPROM.write(FREQUENCY_ADDRESS, STIM_FREQUENCY);
    setSlave(SET_STIM_FREQUENCY, STIM_FREQUENCY);

#if defined(DEBUG)
    Serial.println("[SET] Set Frequency to: " + String(STIM_FREQUENCY));
#endif

};

void changeGeneralConfig(){
    STIM_CURRENT_SCREEN = 1;
    
    STIM_BIPOLAR = !STIM_BIPOLAR;

#if defined(LCM_ENABLED)
    VP_WaveBipolar.write(STIM_BIPOLAR);
#endif
    
    EEPROM.write(BIPOLAR_ADDRESS, STIM_BIPOLAR);
    setSlave(SET_STIM_BIPOLAR, STIM_BIPOLAR);

#if defined(DEBUG)
    if (STIM_BIPOLAR) {
      Serial.println(F("[CHANGE] Changed Wave Pole to Bipolar"));
    } else {
      Serial.println(F("[CHANGE] Changed Wave Pole to Monopolar"));
    }
#endif

};

void nextConfig(){
  if (STIM_CURRENT_SCREEN + 1 <= 10) {
    STIM_CURRENT_SCREEN += 1;
  } else {
    STIM_CURRENT_SCREEN = 7;
  }
};



//
//
//        rtrEncoder->setActionCW(changeGeneralConfig);
//        rtrEncoder->setActionCCW(changeGeneralConfig);
