void ST_SETUP_DONE() {
  if (currentState != lastState) {
    // ENTERING THE STATE, DO INITIALIZATION STUFF
    lastState = currentState;
#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] NStim Setup Process Finished");
#endif
  }

  // PERFORM STATE TASKS

  // CHECK FOR STATE TRANSITIONS
  currentState = LOADING_FROM_EPROM;

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_LOADING_FROM_EPROM() {
  if (currentState != lastState) {
    // ENTERING THE STATE, DO INITIALIZATION STUFF
    lastState = currentState;

#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] Loading data from EPROM");
#endif
    readConfigFromEEPROM();
  }

  // PERFORM STATE TASKS

  // CHECK FOR STATE TRANSITIONS
  if (EEPROM_READ == 1) {
    currentState = SET_SLAVE_STATE;
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_SET_SLAVE_STATE() {
  if (currentState != lastState) {
    // ENTERING THE STATE, DO INITIALIZATION STUFF
    lastState = currentState;

#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] Setting Slave Unit");
#endif

    STIM_ON = false;
    setSlave(SET_STIM_ON, STIM_ON);
    setSlave(SET_STIM_FREQUENCY, STIM_FREQUENCY);
    setSlave(SET_STIM_PULSE_WIDTH, STIM_PULSE_WIDTH);
    setSlave(SET_STIM_BIPOLAR, STIM_BIPOLAR);
    setSlave(SET_STIM_TRAIN, STIM_TRAIN);
    setSlave(SET_STIM_TRAIN_COUNT, STIM_TRAIN_COUNT);
    setSlave(READ_STIM_NEGATIVE_PHASE, STIM_NEGATIVE_PHASE);
    setSlave(SET_STIM_TYPE_CORTICAL, STIM_CORTICAL);
    setSlave(SET_STIM_CURRENT, STIM_CURRENT);
  }

  // PERFORM STATE TASKS

  // CHECK FOR STATE TRANSITIONS
  currentState = MOUNT_SCREEN;

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_MOUNT_SCREEN() {
  if (currentState != lastState) {
    // ENTERING THE STATE, DO INITIALIZATION STUFF
    lastState = currentState;
#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] Mounting Screen");
#endif

#if defined(LCM_ENABLED)
    VP_VolumeLevel.write(STIM_SOUNDLEVEL);
    VP_StimulusMode.write(STIM_CORTICAL);
    VP_StimulusState.write(STIM_ON);
    VP_SetCurrent.write(STIM_CURRENT);
    VP_SetPeriod.write(1 / STIM_FREQUENCY);
    VP_SetFrequency.write(STIM_FREQUENCY);
    VP_Language.write(STIM_LANGUAGE);
    VP_Backlight.write(STIM_BACKLIGHT);
    Lcm.changeBacklight(STIM_BACKLIGHT);
    VP_TrainPulses.write(STIM_TRAIN_COUNT);
    VP_TrainInterval.write(STIM_PULSE_WIDTH);
    VP_WaveBipolar.write(STIM_BIPOLAR);
    VP_WavePhase.write(STIM_NEGATIVE_PHASE);
    VP_TrainEnabled.write(STIM_TRAIN);

    VP_ErrorControl.write(STIM_ERROR);
    VP_BatteryLevel.write(STIM_MEASURED_BATTERY);
    VP_MeasuredImpedance.write(STIM_MEASURED_IMPEDANCE);
    VP_MeasuredCurrent.write(STIM_MEASURED_CURRENT);
    VP_MeasuredCharge.write(0);

    if (STIM_LANGUAGE == 0) {
      Lcm.changePicId(PID_BR_MAIN);
    } else {
      Lcm.changePicId(PID_EN_MAIN);
    }
#endif

  }

  // PERFORM STATE TASKS

  // CHECK FOR STATE TRANSITIONS
  currentState = READY;

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_READY() {
  if (currentState != lastState) {
    lastState = currentState;
    btnGreen->setActionDown(startStimulation);
    btnBlue->setActionDown(startConfig);
    greenLED();
#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] Ready to stimulate");
#endif
  }

  // PERFORM STATE TASKS
  if (STIM_ON == true) {
    currentState = STIMULATING;
  } else if (STIM_CONFIG == true) {
    currentState = CONFIG_WAVE_POLE;
  }

  // CHECK FOR STATE TRANSITIONS

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_STIMULATING() {
  if (currentState != lastState) {
    lastState = currentState;

    btnRed->setActionDown(stopStimulation);
    rtrEncoder->setActionCW(increaseCurrentAtStimulus);
    rtrEncoder->setActionCCW(decreaseCurrentAtStimulus);

#if defined(DEBUG)
    Serial.println(F("[INFO] Stimulating"));
#endif
  }

  // PERFORM STATE TASKS
  monitorSlave();

#if defined(LCM_ENABLED)
  VP_StimulusState.write(STIM_ON);
  VP_SetCurrent.write(STIM_CURRENT);
  VP_SetPeriod.write(1 / STIM_FREQUENCY);
  VP_SetFrequency.write(STIM_FREQUENCY);

  VP_ErrorControl.write(STIM_ERROR);
  VP_BatteryLevel.write(STIM_MEASURED_BATTERY);
  VP_MeasuredImpedance.write(STIM_MEASURED_IMPEDANCE);
  VP_MeasuredCurrent.write(STIM_MEASURED_CURRENT);
  VP_MeasuredCharge.write(0);
#endif

  // CHECK FOR STATE TRANSITIONS
  if (STIM_ON == false) {
    currentState = READY;
  }

  if (STIM_ERROR != 0) {
    currentState = WITH_ERROR;
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_ERROR() {
  if (currentState != lastState) {
    lastState = currentState;

    stopStimulation();
    redLED();

    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();

#if defined(DEBUG) && defined(DETAIL)
    Serial.println("[INFO] Error: " + String(STIM_ERROR));
#endif
  }

  // PERFORM STATE TASKS
  monitorSlave();

#if defined(LCM_ENABLED)
  VP_ErrorControl.write(STIM_ERROR);
  VP_BatteryLevel.write(STIM_MEASURED_BATTERY);
#endif

  // CHECK FOR STATE TRANSITIONS
  if (STIM_ERROR == 0) {
    currentState = READY;
  }

  // if (currentState != lastState) {
  //     // LEAVING THE STATE, DO CLEAN UP STUFF
  // }
}

void ST_CONFIG_WAVE_POLE() {
  if (currentState != lastState) {
    lastState = currentState;

    btnBlue->setActionDown(configCurrent);
    btnEncoder->setActionDown(nextWaveConfig);
    rtrEncoder->setActionCW(changeWavePole);
    rtrEncoder->setActionCCW(changeWavePole);

#if defined(DEBUG)
    Serial.println(F("[INFO] Configuring Wave Form Poles"));
    if (STIM_BIPOLAR == true) {
      Serial.println(F("[INFO] Current selection: BIPOLAR"));
    } else {
      Serial.println(F("[INFO] Current selection: MONOPOLAR"));
    }
#endif

#if defined(LCM_ENABLED)
    if (STIM_LANGUAGE == 0) {
      Lcm.changePicId(PID_BR_CONFIG_WAVE_POLE);
    } else {
      Lcm.changePicId(PID_EN_CONFIG_WAVE_POLE);
    }
#endif

  }

  // PERFORM STATE TASKS
  if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_POLE) {
    currentState = getNextStateFromScreen();
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_CONFIG_WAVE_PHASE() {
  if (currentState != lastState) {
    lastState = currentState;

    btnBlue->setActionDown(configCurrent);
    btnEncoder->setActionDown(nextWaveConfig);
    rtrEncoder->setActionCW(changeWavePhase);
    rtrEncoder->setActionCCW(changeWavePhase);

#if defined(DEBUG)
    Serial.println(F("[INFO] Configuring Wave Form Phase"));
    if (STIM_NEGATIVE_PHASE == true) {
      Serial.println(F("[INFO] Current selection: NEGATIVE"));
    } else {
      Serial.println(F("[INFO] Current selection: POSITIVE"));
    }
#endif

    //        if (LCM_ENABLED) {
    //            if (STIM_LANGUAGE == 0) {
    //                Lcm.changePicId(PID_Config_br);
    //            } else {
    //                Lcm.changePicId(PID_Config_en);
    //            }
    //        }

  }

  // PERFORM STATE TASKS
  if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_PHASE) {
    currentState = getNextStateFromScreen();
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_CONFIG_WAVE_TRAIN() {
  if (currentState != lastState) {
    lastState = currentState;

#if defined(DEBUG)
    Serial.println(F("[INFO] Configuring Wave Form Pulse"));
    if (STIM_TRAIN == true) {
      Serial.println(F("[INFO] Current selection: PULSE TRAIN"));
    } else {
      Serial.println(F("[INFO] Current selection: PULSE UNIQUE"));
    }
#endif

    btnBlue->setActionDown(configCurrent);
    btnEncoder->setActionDown(nextWaveConfig);
    rtrEncoder->setActionCW(changeWaveTrain);
    rtrEncoder->setActionCCW(changeWaveTrain);

    //        if (LCM_ENABLED) {
    //            if (STIM_LANGUAGE == 0) {
    //                Lcm.changePicId(PID_Config_br);
    //            } else {
    //                Lcm.changePicId(PID_Config_en);
    //            }
    //        }

  }

  // PERFORM STATE TASKS
  if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_TRAIN) {
    currentState = getNextStateFromScreen();
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_CONFIG_CURRENT() {
  if (currentState != lastState) {
    lastState = currentState;

    btnBlue->setActionDown(configPeriod);
    btnEncoder->setActionDown(setCurrentAtConfig);
    rtrEncoder->setActionCW(increaseCurrentAtConfig);
    rtrEncoder->setActionCCW(decreaseCurrentAtConfig);

#if defined(DEBUG)
    Serial.println("[INFO] Configuring Current. Current Value: " + String(STIM_CURRENT));
#endif

#if defined(LCM_ENABLED)
    if (STIM_LANGUAGE == 0) {
      Lcm.changePicId(PID_BR_CONFIG_CURRENT);
    } else {
      Lcm.changePicId(PID_EN_CONFIG_CURRENT);
    }
#endif

  }

  // PERFORM STATE TASKS
  if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_CURRENT) {
    currentState = getNextStateFromScreen();
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_CONFIG_PERIOD() {
  if (currentState != lastState) {
    lastState = currentState;

    btnBlue->setActionDown(configFrequency);
    btnEncoder->setActionDown(setPeriodAtConfig);
    rtrEncoder->setActionCW(increasePeriodAtConfig);
    rtrEncoder->setActionCCW(decreasePeriodAtConfig);

#if defined(DEBUG)
    Serial.println("[INFO] Configuring Period. Current Value: " + String(STIM_PULSE_WIDTH));
#endif

#if defined(LCM_ENABLED)
    if (STIM_LANGUAGE == 0) {
      Lcm.changePicId(PID_BR_CONFIG_PERIOD);
    } else {
      Lcm.changePicId(PID_EN_CONFIG_PERIOD);
    }
#endif

  }

  // PERFORM STATE TASKS
  if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_PERIOD) {
    currentState = getNextStateFromScreen();
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_CONFIG_FREQUENCY() {
  if (currentState != lastState) {
    lastState = currentState;

    btnBlue->setActionDown(configGeneral);
    btnEncoder->setActionDown(setFrequencyAtConfig);
    rtrEncoder->setActionCW(increaseFrequencyAtConfig);
    rtrEncoder->setActionCCW(decreaseFrequencyAtConfig);

#if defined(DEBUG)
    Serial.println("[INFO] Configuring Frequency. Current Value: " + String(STIM_FREQUENCY));
#endif

#if defined(LCM_ENABLED)
    if (STIM_LANGUAGE == 0) {
      Lcm.changePicId(PID_BR_CONFIG_FREQUENCY);
    } else {
      Lcm.changePicId(PID_EN_CONFIG_FREQUENCY);
    }
#endif

  }

  // PERFORM STATE TASKS
  if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_FREQUENCY) {
    currentState = getNextStateFromScreen();
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void ST_CONFIG_GENERAL() {
  if (currentState != lastState) {
    lastState = currentState;

#if defined(DEBUG)
    Serial.println(F("[INFO] General Configuration Select. Current Value: User Settings"));
#endif
    btnBlue->setActionDown(startConfig);
    //        btnEncoder->setActionDown(enterConfig);
    //        rtrEncoder->setActionCW(changeGeneralConfig);
    //        rtrEncoder->setActionCCW(changeGeneralConfig);

#if defined(LCM_ENABLED)
    if (STIM_LANGUAGE == 0) {
      Lcm.changePicId(PID_BR_CONFIG_FREQUENCY);
    } else {
      Lcm.changePicId(PID_EN_CONFIG_FREQUENCY);
    }
#endif

  }

  // PERFORM STATE TASKS
  if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_GENERAL) {
    currentState = getNextStateFromScreen();
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
  }
}

void stateLoop() {
  switch (currentState) {
    case SETUP_DONE:
      ST_SETUP_DONE();
      break;

    case LOADING_FROM_EPROM:
      ST_LOADING_FROM_EPROM();
      break;

    case SET_SLAVE_STATE:
      ST_SET_SLAVE_STATE();
      break;

    case MOUNT_SCREEN:
      ST_MOUNT_SCREEN();
      break;

    case READY:
      ST_READY();
      break;

    case STIMULATING:
      ST_STIMULATING();
      break;

    case WITH_ERROR:
      ST_ERROR();
      break;

    case CONFIG_WAVE_POLE:
      ST_CONFIG_WAVE_POLE();
      break;

    case CONFIG_WAVE_PHASE:
      ST_CONFIG_WAVE_PHASE();
      break;

    case CONFIG_WAVE_TRAIN:
      ST_CONFIG_WAVE_TRAIN();
      break;
      
    case CONFIG_CURRENT:
      ST_CONFIG_CURRENT();
      break;

    case CONFIG_PERIOD:
      ST_CONFIG_PERIOD();
      break;

    case CONFIG_FREQUENCY:
      ST_CONFIG_FREQUENCY();
      break;

    case CONFIG_GENERAL:
      ST_CONFIG_GENERAL();
      break;

    default:
      break;
  }
}

byte getNextStateFromScreen(){
  switch (STIM_CURRENT_SCREEN) {
   
    case SCREEN_CONFIG_WAVE_POLE:
      return CONFIG_WAVE_POLE;

    case SCREEN_CONFIG_WAVE_PHASE:
      return CONFIG_WAVE_PHASE;

    case SCREEN_CONFIG_WAVE_TRAIN:
      return CONFIG_WAVE_TRAIN;

    case SCREEN_CONFIG_WAVE_TRAIN_DETAILS:
      return CONFIG_WAVE_TRAIN_DETAILS;

    case SCREEN_CONFIG_WAVE_EXIT:
      return CONFIG_WAVE_TRAIN_DETAILS;
      
    case SCREEN_CONFIG_CURRENT:
      return CONFIG_CURRENT;

    case SCREEN_CONFIG_PERIOD:
      return CONFIG_PERIOD;

    case SCREEN_CONFIG_FREQUENCY:
      return CONFIG_FREQUENCY;

    case SCREEN_CONFIG_GENERAL:
      return CONFIG_GENERAL;

    default:
      return 0;
  }  
}
