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
        clearAllActions();
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
        clearAllActions();
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
        clearAllActions();
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
        clearAllActions();
    }
}

void ST_READY() {
    if (currentState != lastState) {
        lastState = currentState;
        btnGreen->setActionDown(startStimulation);
        btnBlue->setActionDown(startConfig);
        STIM_CONFIG = false;
        greenLED();
#if defined(DEBUG)
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
        clearAllActions();
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
        clearAllActions();
    }
}

void ST_ERROR() {
    if (currentState != lastState) {
        lastState = currentState;

        stopStimulation();
        redLED();

        clearAllActions();

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
        clearAllActions();
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

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_WAVE_PHASE);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_PHASE);
        }
#endif
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_PHASE) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
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

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN);
        }
#endif

        btnBlue->setActionDown(configCurrent);
        btnEncoder->setActionDown(nextWaveConfig);
        rtrEncoder->setActionCW(changeWaveTrain);
        rtrEncoder->setActionCCW(changeWaveTrain);
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_TRAIN) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_WAVE_TRAIN_DETAILS_PULSES() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        if (STIM_TRAIN == true) {
            Serial.println(F("[INFO] Configuring Wave Pulse Train Details"));
            Serial.println(F("[INFO] Current selection: NUMBER OF PULSES"));
        }
#endif

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN_DETAILS_PULSE);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN_DETAILS_PULSE);
        }
#endif

        btnBlue->setActionDown(configCurrent);
        btnEncoder->setActionDown(nextWaveConfig);
        rtrEncoder->setActionCW(increasePulses);
        rtrEncoder->setActionCCW(decreasePulses);
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_TRAIN_PULSES) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        if (STIM_TRAIN == true) {
            Serial.println(F("[INFO] Configuring Wave Pulse Train Interval"));
            Serial.println(F("[INFO] Current selection: INTERVAL BETWEEN PULSES"));
        }
#endif

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL);
        }
#endif

        btnBlue->setActionDown(configCurrent);
        btnEncoder->setActionDown(nextWaveConfig);
        rtrEncoder->setActionCW(increaseInterval);
        rtrEncoder->setActionCCW(decreaseInterval);
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_TRAIN_INTERVAL) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_WAVE_TRAIN_DETAILS_EXIT() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        Serial.println(F("[INFO] Ready to exit Configuring Wave Pulse Train Details"));
#endif

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN_DETAILS_EXIT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN_DETAILS_EXIT);
        }
#endif

        btnBlue->setActionDown(configCurrent);
        btnEncoder->setActionDown(nextWaveConfig);
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_TRAIN_EXIT) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_WAVE_TRAIN_EXIT() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        Serial.println(F("[INFO] Ready to exit Configuring Wave Pulse Train"));
#endif

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN_EXIT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN_EXIT);
        }
#endif

        btnBlue->setActionDown(configCurrent);
        btnEncoder->setActionDown(exitWaveConfig);
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_WAVE_EXIT) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
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
        clearAllActions();
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
        clearAllActions();
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
        clearAllActions();
    }
}

void ST_CONFIG_GENERAL_USER() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        Serial.println(F("[INFO] General Configuration Select. Current Value: User Settings"));
#endif
        btnBlue->setActionDown(getReady);
        btnEncoder->setActionDown(enterConfigUser);
        rtrEncoder->setActionCW(changeGeneralConfig);
        rtrEncoder->setActionCCW(changeGeneralConfig);

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER);
        }
#endif
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_GENERAL_USER) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_GENERAL_MODE() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        Serial.println(F("[INFO] General Configuration Select. Current Value: Mode Select"));
#endif
        btnBlue->setActionDown(getReady);
        btnEncoder->setActionDown(enterConfigMode);
        rtrEncoder->setActionCW(changeGeneralConfig);
        rtrEncoder->setActionCCW(changeGeneralConfig);

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_MODE);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_MODE);
        }
#endif
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_GENERAL_MODE) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_GENERAL_USER_LANG() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        Serial.println(F("[INFO] Configuring User Setting Language"));
        if (STIM_LANGUAGE) {
            Serial.println(F("[INFO] Current selection: PORTUGUESE"));
        } else {
            Serial.println(F("[INFO] Current selection: ENGLISH"));
        }
#endif
        btnBlue->setActionDown(getReady);
        btnEncoder->setActionDown(enterVolumeSetting);
        rtrEncoder->setActionCW(changeLanguage);
        rtrEncoder->setActionCCW(changeLanguage);

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_LANG);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_LANG);
        }
#endif
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_GENERAL_USER_LANG) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_GENERAL_USER_SOUND() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        Serial.println("[INFO] Configuring User Setting Sound Level" + String(STIM_SOUNDLEVEL));
#endif
        btnBlue->setActionDown(getReady);
        btnEncoder->setActionDown(enterBackLightSetting);
        rtrEncoder->setActionCW(increaseSoundLevel);
        rtrEncoder->setActionCCW(decreaseSoundLevel);

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_SOUND);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_SOUND);
        }
#endif
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_GENERAL_USER_SOUND) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_GENERAL_USER_BACKLIGHT() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        Serial.println("[INFO] Configuring User Setting Backlight" + String(STIM_BACKLIGHT));
#endif
        btnBlue->setActionDown(getReady);
        btnEncoder->setActionDown(enterUserSettingExit);
        rtrEncoder->setActionCW(increaseBacklight);
        rtrEncoder->setActionCCW(decreaseBacklight);

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_BACKLIGHT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_BACKLIGHT);
        }
#endif
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_GENERAL_USER_BACKLIGHT) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_GENERAL_USER_EXIT() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG)
        Serial.println(F("[INFO] Ready to exit General User Settings"));
#endif

#if defined(LCM_ENABLED)
        if (STIM_LANGUAGE == 0) {
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_EXIT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_EXIT);
        }
#endif

        btnBlue->setActionDown(getReady);
        btnEncoder->setActionDown(enterGeneralMode);
    }

    // PERFORM STATE TASKS
    if (STIM_CURRENT_SCREEN != SCREEN_CONFIG_GENERAL_USER_EXIT) {
        currentState = getNextStateFromScreen();
    }

    if (currentState != lastState) {
        clearAllActions();
    }
}

void ST_CONFIG_GENERAL_MODE_CORTICAL() {}

void ST_CONFIG_GENERAL_MODE_LOCALIZATION() {}

void ST_CONFIG_GENERAL_MODE_EXIT() {}

void ST_CONFIG_GENERAL_EXIT() {}

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

        case CONFIG_WAVE_TRAIN_PULSES:
            ST_CONFIG_WAVE_TRAIN_DETAILS_PULSES();
            break;

        case CONFIG_WAVE_TRAIN_INTERVAL:
            ST_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL();
            break;

        case CONFIG_WAVE_TRAIN_EXIT:
            ST_CONFIG_WAVE_TRAIN_DETAILS_EXIT();
            break;

        case CONFIG_WAVE_EXIT:
            ST_CONFIG_WAVE_TRAIN_EXIT();
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

        case CONFIG_GENERAL_USER:
            ST_CONFIG_GENERAL_USER();
            break;

        case CONFIG_GENERAL_USER_LANG:
            ST_CONFIG_GENERAL_USER_LANG();
            break;

        case CONFIG_GENERAL_USER_SOUND:
            ST_CONFIG_GENERAL_USER_SOUND();
            break;

        case CONFIG_GENERAL_USER_BACKLIGHT:
            ST_CONFIG_GENERAL_USER_BACKLIGHT();
            break;

        case CONFIG_GENERAL_USER_EXIT:
            ST_CONFIG_GENERAL_USER_EXIT();
            break;

        case CONFIG_GENERAL_MODE:
            ST_CONFIG_GENERAL_MODE();
            break;

        case CONFIG_GENERAL_MODE_CORTICAL:
            ST_CONFIG_GENERAL_MODE_CORTICAL();
            break;

        case CONFIG_GENERAL_MODE_LOCALIZATION:
            ST_CONFIG_GENERAL_MODE_LOCALIZATION();
            break;

        case CONFIG_GENERAL_MODE_EXIT:
            ST_CONFIG_GENERAL_MODE_EXIT();
            break;

        case CONFIG_GENERAL_EXIT:
            ST_CONFIG_GENERAL_EXIT();
            break;

        default:
#if defined(DEBUG)
            Serial.println("[ERROR] STATE UNKNOWN" + String(currentState));
#endif
            break;
    }
}

byte getNextStateFromScreen() {
    switch (STIM_CURRENT_SCREEN) {
        case SCREEN_CONFIG_WAVE_POLE:
            return CONFIG_WAVE_POLE;

        case SCREEN_CONFIG_WAVE_PHASE:
            return CONFIG_WAVE_PHASE;

        case SCREEN_CONFIG_WAVE_TRAIN:
            return CONFIG_WAVE_TRAIN;

        case SCREEN_CONFIG_WAVE_TRAIN_PULSES:
            return CONFIG_WAVE_TRAIN_PULSES;

        case SCREEN_CONFIG_WAVE_TRAIN_INTERVAL:
            return CONFIG_WAVE_TRAIN_INTERVAL;

        case SCREEN_CONFIG_WAVE_TRAIN_EXIT:
            return CONFIG_WAVE_TRAIN_EXIT;

        case SCREEN_CONFIG_WAVE_EXIT:
            return CONFIG_WAVE_EXIT;

        case SCREEN_CONFIG_CURRENT:
            return CONFIG_CURRENT;

        case SCREEN_CONFIG_PERIOD:
            return CONFIG_PERIOD;

        case SCREEN_CONFIG_FREQUENCY:
            return CONFIG_FREQUENCY;

        case SCREEN_READY:
            return READY;

        case SCREEN_CONFIG_GENERAL_USER:
            return CONFIG_GENERAL_USER;

        case SCREEN_CONFIG_GENERAL_MODE:
            return CONFIG_GENERAL_MODE;

        case SCREEN_CONFIG_GENERAL_USER_LANG:
            return CONFIG_GENERAL_USER_LANG;

        case SCREEN_CONFIG_GENERAL_USER_SOUND:
            return CONFIG_GENERAL_USER_SOUND;

        case SCREEN_CONFIG_GENERAL_USER_BACKLIGHT:
            return CONFIG_GENERAL_USER_BACKLIGHT;

        case SCREEN_CONFIG_GENERAL_USER_EXIT:
            return CONFIG_GENERAL_USER_EXIT;

        case SCREEN_CONFIG_GENERAL_MODE_CORTICAL:
            return CONFIG_GENERAL_MODE_CORTICAL;

        case SCREEN_CONFIG_GENERAL_MODE_LOCALIZATION:
            return CONFIG_GENERAL_MODE_LOCALIZATION;

        case SCREEN_CONFIG_GENERAL_MODE_EXIT:
            return CONFIG_GENERAL_MODE_EXIT;

        case SCREEN_CONFIG_GENERAL_EXIT:
            return CONFIG_GENERAL_EXIT;

        default:
#if defined(DEBUG)
            Serial.println("[ERROR] SCREEN UNKNOWN" + String(STIM_CURRENT_SCREEN));
#endif
            return READY;
    }
}
