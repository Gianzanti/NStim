void ST_LOADING_FROM_EPROM() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG) && defined(DETAIL)
        Serial.println("[INFO] Loading data from EPROM");
#endif
        readConfigFromEEPROM();
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_MOUNT_SCREEN() {
    if (currentState != lastState) {
        lastState = currentState;

#if defined(DEBUG) && defined(DETAIL)
        Serial.println("[INFO] Mounting Screen");
#endif

#if defined(LCM_ENABLED)
        VP_VolumeLevel.write(STIM_SOUNDLEVEL);
        VP_StimulusMode.write(STIM_CORTICAL);
        VP_StimulusState.write(STIM_ON);
        VP_SetCurrent.write(STIM_CURRENT);
        VP_SetPulseWidth.write(STIM_PULSE_WIDTH);
        VP_SetFrequency.write(STIM_FREQUENCY);
        VP_Language.write(STIM_LANG_ENGLISH);
        VP_Backlight.write(STIM_BACKLIGHT);
        Lcm.changeBacklight(STIM_BACKLIGHT);
        VP_TrainPulses.write(STIM_TRAIN_COUNT);
        VP_TrainInterval.write(STIM_TRAIN_INTERVAL);
        VP_WaveBipolar.write(STIM_BIPOLAR);
        VP_WavePhase.write(STIM_NEGATIVE_PHASE);
        VP_TrainEnabled.write(STIM_TRAIN);

        VP_ErrorControl.write(STIM_ERROR);
        VP_BatteryLevel.write(STIM_MEASURED_BATTERY);
        VP_MeasuredImpedance.write(STIM_MEASURED_IMPEDANCE);
        VP_MeasuredCurrent.write(STIM_MEASURED_CURRENT);
        VP_MeasuredCharge.write(STIM_MEASURED_CHARGE);

        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_MAIN;
        } else {
            currentScreen = PID_EN_MAIN;
        }
        Lcm.changePicId(currentScreen);
#endif

        nextState = READY;
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_READY() {
    if (currentState != lastState) {
        lastState = currentState;
        greenLED();
        clearAllActions();
        btnGreen->setActionDown(setNextState, STIMULATING);
        btnBlue->setActionDown(setNextState, CONFIG_WAVE_POLE);

#if defined(DEBUG)
        Serial.println("[INFO] Ready to stimulate");
#endif
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_STIMULATING() {
    if (currentState != lastState) {
        lastState = currentState;

        clearAllActions();
        btnRed->setActionDown(setNextState, READY);
        rtrEncoder->setActionCW(increaseCurrentAtStimulus);
        rtrEncoder->setActionCCW(decreaseCurrentAtStimulus);
        blueLED();
        STIM_ON = true;

#if defined(LCM_ENABLED)
        VP_StimulusState.write(STIM_ON);
#endif

#if defined(DEBUG)
        Serial.println(F("[INFO] Stimulating"));
#endif
    }

    if (nextState != currentState) {
        currentState = nextState;
    }

    if (currentState != lastState) {
        STIM_ON = false;

#if defined(LCM_ENABLED)
        VP_StimulusState.write(STIM_ON);
#endif

        clearAllActions();
    }
}

void ST_ERROR() {
    if (currentState != lastState) {
        lastState = currentState;
        redLED();
        clearAllActions();

#if defined(DEBUG) && defined(DETAIL)
        Serial.println("[INFO] Error: " + String(STIM_ERROR));
#endif
    }

#if defined(LCM_ENABLED)
    VP_ErrorControl.write(STIM_ERROR);
    VP_BatteryLevel.write(STIM_MEASURED_BATTERY);
#endif

    // CHECK FOR STATE TRANSITIONS
    if (STIM_ERROR == 0) {
        nextState = READY;
    }

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }

    // if (currentState != lastState) {
    //     // LEAVING THE STATE, DO CLEAN UP STUFF
    // }
}

void ST_CONFIG_WAVE_POLE() {
    if (currentState != lastState) {
        lastState = currentState;
        whiteLED();
        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_PHASE);
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
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_WAVE_POLE;
        } else {
            currentScreen = PID_EN_CONFIG_WAVE_POLE;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_WaveBipolar.available()) {
        bool value = (VP_WaveBipolar.getData() == 1);
        if (value != STIM_BIPOLAR) {
            STIM_BIPOLAR = value;
            EEPROM.write(BIPOLAR_ADDRESS, STIM_BIPOLAR);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_WAVE_PHASE() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();
        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_TRAIN);
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
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_WAVE_PHASE;
        } else {
            currentScreen = PID_EN_CONFIG_WAVE_PHASE;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_WavePhase.available()) {
        bool value = (VP_WavePhase.getData() == 1);
        if (value != STIM_NEGATIVE_PHASE) {
            STIM_NEGATIVE_PHASE = value;
            EEPROM.write(NEGATIVE_PHASE_ADDRESS, STIM_NEGATIVE_PHASE);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_WAVE_TRAIN() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] Configuring Wave Form Pulse"));
        if (STIM_TRAIN == true) {
            Serial.println(F("[INFO] Current selection: PULSE TRAIN"));
        } else {
            Serial.println(F("[INFO] Current selection: PULSE UNIQUE"));
        }
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_WAVE_TRAIN;
        } else {
            currentScreen = PID_EN_CONFIG_WAVE_TRAIN;
        }
        Lcm.changePicId(currentScreen);
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        rtrEncoder->setActionCW(changeWaveTrain);
        rtrEncoder->setActionCCW(changeWaveTrain);
    }

    // PERFORM STATE TASKS
    if (STIM_TRAIN) {
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_TRAIN_PULSES);
    } else {
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_EXIT);
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_TrainEnabled.available()) {
        bool value = (VP_TrainEnabled.getData() == 1);
        if (value != STIM_TRAIN) {
            STIM_TRAIN = value;
            EEPROM.write(TRAIN_ADDRESS, STIM_TRAIN);
        }
    }
#endif

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_WAVE_TRAIN_DETAILS_PULSES() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        if (STIM_TRAIN == true) {
            Serial.println(F("[INFO] Configuring Wave Pulse Train Details"));
            Serial.println(F("[INFO] Current selection: NUMBER OF PULSES"));
        }
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_WAVE_TRAIN_DETAILS_PULSE;
        } else {
            currentScreen = PID_EN_CONFIG_WAVE_TRAIN_DETAILS_PULSE;
        }
        Lcm.changePicId(currentScreen);
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_TRAIN_INTERVAL);
        rtrEncoder->setActionCW(increasePulses);
        rtrEncoder->setActionCCW(decreasePulses);
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_TrainPulses.available()) {
        byte value = VP_TrainPulses.getData();
        if (value != STIM_TRAIN_COUNT) {
            STIM_TRAIN_COUNT = value;
            EEPROM.write(TRAIN_COUNT_ADDRESS, STIM_TRAIN_COUNT);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        if (STIM_TRAIN == true) {
            Serial.println(F("[INFO] Configuring Wave Pulse Train Interval"));
            Serial.println(F("[INFO] Current selection: INTERVAL BETWEEN PULSES"));
        }
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL;
        } else {
            currentScreen = PID_EN_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL;
        }
        Lcm.changePicId(currentScreen);
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_TRAIN_EXIT);
        rtrEncoder->setActionCW(increaseInterval);
        rtrEncoder->setActionCCW(decreaseInterval);
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_TrainInterval.available()) {
        byte value = VP_TrainInterval.getData();
        if (value != STIM_TRAIN_INTERVAL) {
            STIM_TRAIN_INTERVAL = value;
            EEPROM.write(TRAIN_INTERVAL_ADDRESS, STIM_TRAIN_INTERVAL);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_WAVE_TRAIN_DETAILS_EXIT() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] Ready to exit Configuring Wave Pulse Train Details"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_WAVE_TRAIN_DETAILS_EXIT;
        } else {
            currentScreen = PID_EN_CONFIG_WAVE_TRAIN_DETAILS_EXIT;
        }
        Lcm.changePicId(currentScreen);
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_EXIT);
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_WAVE_TRAIN_EXIT() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] Ready to exit Configuring Wave Pulse Train"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_WAVE_TRAIN_EXIT;
        } else {
            currentScreen = PID_EN_CONFIG_WAVE_TRAIN_EXIT;
        }
        Lcm.changePicId(currentScreen);
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, READY);
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_CURRENT() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

        btnBlue->setActionDown(setNextState, CONFIG_PULSE_WIDTH);
        btnEncoder->setActionDown(setCurrentAtConfig, NULL);
        rtrEncoder->setActionCW(increaseCurrentAtConfig);
        rtrEncoder->setActionCCW(decreaseCurrentAtConfig);

#if defined(DEBUG)
        Serial.println("[INFO] Configuring Current. Current Value: " + String(STIM_CURRENT));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_CURRENT;
        } else {
            currentScreen = PID_EN_CONFIG_CURRENT;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_SetCurrent.available()) {
        byte value = VP_SetCurrent.getData();
        if (value != STIM_CURRENT) {
            STIM_CURRENT = value;
            EEPROM.write(CURRENT_ADDRESS, STIM_CURRENT);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_PERIOD() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

        btnBlue->setActionDown(setNextState, CONFIG_FREQUENCY);
        btnEncoder->setActionDown(setPeriodAtConfig, NULL);
        rtrEncoder->setActionCW(increasePeriodAtConfig);
        rtrEncoder->setActionCCW(decreasePeriodAtConfig);

#if defined(DEBUG)
        Serial.println("[INFO] Configuring Period. Current Value: " + String(STIM_PULSE_WIDTH));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_PULSE_WIDTH;
        } else {
            currentScreen = PID_EN_CONFIG_PULSE_WIDTH;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_SetPulseWidth.available()) {
        unsigned int value = VP_SetPulseWidth.getData();
        if (value != STIM_PULSE_WIDTH) {
            STIM_PULSE_WIDTH = value;
            EEPROM.write(PULSE_WIDTH_ADDRESS, STIM_PULSE_WIDTH);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_FREQUENCY() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();
        btnBlue->setActionDown(setNextState, CONFIG_GENERAL_USER);
        btnEncoder->setActionDown(setFrequencyAtConfig, NULL);
        rtrEncoder->setActionCW(increaseFrequencyAtConfig);
        rtrEncoder->setActionCCW(decreaseFrequencyAtConfig);

#if defined(DEBUG)
        Serial.println("[INFO] Configuring Frequency. Current Value: " + String(STIM_FREQUENCY));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_FREQUENCY;
        } else {
            currentScreen = PID_EN_CONFIG_FREQUENCY;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_SetFrequency.available()) {
        unsigned int value = VP_SetFrequency.getData();
        if (value != STIM_FREQUENCY) {
            STIM_FREQUENCY = value;
            EEPROM.write(FREQUENCY_ADDRESS, STIM_FREQUENCY);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_USER() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] General Configuration Select. Current Value: User Settings"));
#endif
        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, CONFIG_GENERAL_USER_LANG);
        rtrEncoder->setActionCW(setNextState, CONFIG_GENERAL_MODE);
        rtrEncoder->setActionCCW(setNextState, CONFIG_GENERAL_MODE);

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_USER;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_USER;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_MODE() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] General Configuration Select. Current Value: Mode Select"));
#endif
        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, CONFIG_GENERAL_MODE_CORTICAL);
        rtrEncoder->setActionCW(setNextState, CONFIG_GENERAL_USER);
        rtrEncoder->setActionCCW(setNextState, CONFIG_GENERAL_USER);

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_MODE;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_MODE;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_USER_LANG() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] Configuring User Setting Language"));
        if (!STIM_LANG_ENGLISH) {
            Serial.println(F("[INFO] Current selection: PORTUGUESE"));
        } else {
            Serial.println(F("[INFO] Current selection: ENGLISH"));
        }
#endif
        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, CONFIG_GENERAL_USER_SOUND);
        rtrEncoder->setActionCW(changeLanguage);
        rtrEncoder->setActionCCW(changeLanguage);

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_USER_LANG;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_USER_LANG;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_Language.available()) {
        bool value = (VP_Language.getData() == 1);
        if (value != STIM_LANG_ENGLISH) {
            STIM_LANG_ENGLISH = value;
            EEPROM.write(LANGUAGE_ADDRESS, STIM_LANG_ENGLISH);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_USER_SOUND() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println("[INFO] Configuring User Setting Sound Level" + String(STIM_SOUNDLEVEL));
#endif

        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, CONFIG_GENERAL_USER_BACKLIGHT);
        rtrEncoder->setActionCW(increaseSoundLevel);
        rtrEncoder->setActionCCW(decreaseSoundLevel);

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_USER_SOUND;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_USER_SOUND;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_VolumeLevel.available()) {
        byte value = VP_VolumeLevel.getData();
        if (value != STIM_SOUNDLEVEL) {
            STIM_SOUNDLEVEL = value;
            EEPROM.write(SOUNDLEVEL_ADDRESS, STIM_SOUNDLEVEL);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_USER_BACKLIGHT() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println("[INFO] Configuring User Setting Backlight" + String(STIM_BACKLIGHT));
#endif
        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, CONFIG_GENERAL_USER_EXIT);
        rtrEncoder->setActionCW(increaseBacklight);
        rtrEncoder->setActionCCW(decreaseBacklight);

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_USER_BACKLIGHT;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_USER_BACKLIGHT;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_Backlight.available()) {
        byte value = VP_Backlight.getData();
        if (value != STIM_BACKLIGHT) {
            STIM_BACKLIGHT = value;
            VP_Backlight.write(STIM_BACKLIGHT);
            EEPROM.write(BACKLIGHT_ADDRESS, STIM_BACKLIGHT);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_USER_EXIT() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] Ready to exit General User Settings"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_USER_EXIT;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_USER_EXIT;
        }
        Lcm.changePicId(currentScreen);
#endif

        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, CONFIG_GENERAL_MODE);
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_MODE_CORTICAL() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();
        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setCorticalMode);
        rtrEncoder->setActionCW(setNextState, CONFIG_GENERAL_MODE_LOCALIZATION);
        rtrEncoder->setActionCCW(setNextState, CONFIG_GENERAL_MODE_LOCALIZATION);

#if defined(DEBUG)
        Serial.println(F("[INFO] Current Localization Mode: Cortical"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_MODE_CORTICAL;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_MODE_CORTICAL;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_StimulusMode.available()) {
        bool value = VP_StimulusMode.getData() == 1;
        if (value != STIM_CORTICAL) {
            STIM_CORTICAL = value;
            EEPROM.write(TYPE_CORTICAL_ADDRESS, STIM_CORTICAL);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_MODE_LOCALIZATION() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();
        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setLocalizationMode);
        rtrEncoder->setActionCW(setNextState, CONFIG_GENERAL_MODE_CORTICAL);
        rtrEncoder->setActionCCW(setNextState, CONFIG_GENERAL_MODE_CORTICAL);

#if defined(DEBUG)
        Serial.println(F("[INFO] Current Localization Mode: Localization"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_MODE_LOCALIZATION;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_MODE_LOCALIZATION;
        }
        Lcm.changePicId(currentScreen);
#endif
    }

#if defined(LCM_ENABLED)
    // monitor changes directly on touch screen
    if (VP_StimulusMode.available()) {
        bool value = VP_StimulusMode.getData() == 1;
        if (value != STIM_CORTICAL) {
            STIM_CORTICAL = value;
            EEPROM.write(TYPE_CORTICAL_ADDRESS, STIM_CORTICAL);
        }
    }
#endif

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_MODE_EXIT() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] Ready to exit General Localization Settings"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_MODE_EXIT;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_MODE_EXIT;
        }
        Lcm.changePicId(currentScreen);
#endif

        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, CONFIG_GENERAL_EXIT);
    }

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_EXIT() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

#if defined(DEBUG)
        Serial.println(F("[INFO] Ready to exit General Settings"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            currentScreen = PID_BR_CONFIG_GENERAL_EXIT;
        } else {
            currentScreen = PID_EN_CONFIG_GENERAL_EXIT;
        }
        Lcm.changePicId(currentScreen);
#endif

        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, READY);
    }

    if (nextState != currentState) {
        currentState = nextState;
    }
}

void stateLoop() {
    switch (currentState) {
        case LOADING_FROM_EPROM:
            ST_LOADING_FROM_EPROM();
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

        case CONFIG_PULSE_WIDTH:
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
