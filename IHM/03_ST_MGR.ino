void ST_LOADING_FROM_EPROM() {
    if (currentState != lastState) {
        // ENTERING THE STATE, DO INITIALIZATION STUFF
        lastState = currentState;

#if defined(DEBUG) && defined(DETAIL)
        Serial.println("[INFO] Loading data from EPROM");
#endif
        readConfigFromEEPROM();
    }

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_SET_SLAVE_STATE() {
    if (currentState != lastState) {
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

        nextState = MOUNT_SCREEN;
    }

    // CHECK FOR STATE TRANSITIONS
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
        VP_SetPeriod.write(1 / STIM_FREQUENCY);
        VP_SetFrequency.write(STIM_FREQUENCY);
        VP_Language.write(STIM_LANG_ENGLISH);
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

        if (!STIM_LANG_ENGLISH) {
            Lcm.changePicId(PID_BR_MAIN);
        } else {
            Lcm.changePicId(PID_EN_MAIN);
        }
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

    // PERFORM STATE TASKS

    // CHECK FOR STATE TRANSITIONS
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
        setSlave(SET_STIM_ON, STIM_ON);

#if defined(DEBUG)
        Serial.println(F("[INFO] Stimulating"));
#endif
    }

    // PERFORM STATE TASKS
    monitorSlave();

    //#if defined(LCM_ENABLED)
    //    VP_StimulusState.write(STIM_ON);
    ////    VP_SetCurrent.write(STIM_CURRENT);
    ////    VP_SetPeriod.write(1 / STIM_FREQUENCY);
    ////    VP_SetFrequency.write(STIM_FREQUENCY);
    //
    ////    VP_ErrorControl.write(STIM_ERROR);
    ////    VP_BatteryLevel.write(STIM_MEASURED_BATTERY);
    //    VP_MeasuredImpedance.write(STIM_MEASURED_IMPEDANCE);
    //    VP_MeasuredCurrent.write(STIM_MEASURED_CURRENT);
    //    VP_MeasuredCharge.write(0);
    //#endif

    // CHECK FOR STATE TRANSITIONS
    //    if (STIM_ON == false) {
    //        currentState = READY;
    //    }

    //    if (STIM_ERROR != 0) {
    //        currentState = WITH_ERROR;
    //    }

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }

    if (currentState != lastState) {
        STIM_ON = false;
        setSlave(SET_STIM_ON, STIM_ON);

#if defined(LCM_ENABLED)
        VP_StimulusState.write(STIM_ON);
#endif

        clearAllActions();
    }
}

void ST_ERROR() {
    if (currentState != lastState) {
        lastState = currentState;

        //        stopStimulation();
        redLED();

        clearAllActions();

#if defined(DEBUG) && defined(DETAIL)
        Serial.println("[INFO] Error: " + String(STIM_ERROR));
#endif
    }

    // PERFORM STATE TASKS
    //    monitorSlave();
    //
    //#if defined(LCM_ENABLED)
    //    VP_ErrorControl.write(STIM_ERROR);
    //    VP_BatteryLevel.write(STIM_MEASURED_BATTERY);
    //#endif

    // CHECK FOR STATE TRANSITIONS
    //    if (STIM_ERROR == 0) {
    //        currentState = READY;
    //    }

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
            Lcm.changePicId(PID_BR_CONFIG_WAVE_POLE);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_POLE);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_WAVE_PHASE);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_PHASE);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN);
        }
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
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN_DETAILS_PULSE);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN_DETAILS_PULSE);
        }
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_TRAIN_INTERVAL);
        rtrEncoder->setActionCW(increasePulses);
        rtrEncoder->setActionCCW(decreasePulses);
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL);
        }
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_TRAIN_EXIT);
        rtrEncoder->setActionCW(increaseInterval);
        rtrEncoder->setActionCCW(decreaseInterval);
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN_DETAILS_EXIT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN_DETAILS_EXIT);
        }
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, CONFIG_WAVE_EXIT);
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_WAVE_TRAIN_EXIT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_WAVE_TRAIN_EXIT);
        }
#endif

        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setNextState, READY);
    }

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_CURRENT() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();

        btnBlue->setActionDown(setNextState, CONFIG_PERIOD);
        btnEncoder->setActionDown(setCurrentAtConfig, NULL);
        rtrEncoder->setActionCW(increaseCurrentAtConfig);
        rtrEncoder->setActionCCW(decreaseCurrentAtConfig);

#if defined(DEBUG)
        Serial.println("[INFO] Configuring Current. Current Value: " + String(STIM_CURRENT));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            Lcm.changePicId(PID_BR_CONFIG_CURRENT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_CURRENT);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_PERIOD);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_PERIOD);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_FREQUENCY);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_FREQUENCY);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_MODE);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_MODE);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_LANG);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_LANG);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_SOUND);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_SOUND);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_BACKLIGHT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_BACKLIGHT);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_EXIT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_EXIT);
        }
#endif

        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, CONFIG_GENERAL_MODE);
    }

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_MODE_CORTICAL() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();
        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setCorticalMode);
        rtrEncoder->setActionCW(setNextState, CONFIG_GENERAL_MODE_LOCALIZATION);
        rtrEncoder->setActionCCW(setNextState, CONFIG_GENERAL_MODE_LOCALIZATION);

#if defined(DEBUG)
        Serial.println(F("[INFO] Current Localization Mode: Cortical"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_MODE_CORTICAL);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_MODE_CORTICAL);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }
}

void ST_CONFIG_GENERAL_MODE_LOCALIZATION() {
    if (currentState != lastState) {
        lastState = currentState;
        clearAllActions();
        btnBlue->setActionDown(setNextState, CONFIG_CURRENT);
        btnEncoder->setActionDown(setLocalizationMode);
        rtrEncoder->setActionCW(setNextState, CONFIG_GENERAL_MODE_CORTICAL);
        rtrEncoder->setActionCCW(setNextState, CONFIG_GENERAL_MODE_CORTICAL);

#if defined(DEBUG)
        Serial.println(F("[INFO] Current Localization Mode: Localization"));
#endif

#if defined(LCM_ENABLED)
        if (!STIM_LANG_ENGLISH) {
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_MODE_LOCALIZATION);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_MODE_LOCALIZATION);
        }
#endif
    }

    // CHECK FOR STATE TRANSITIONS
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
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_MODE_EXIT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_MODE_EXIT);
        }
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
            Lcm.changePicId(PID_BR_CONFIG_GENERAL_EXIT);
        } else {
            Lcm.changePicId(PID_EN_CONFIG_GENERAL_EXIT);
        }
#endif

        btnBlue->setActionDown(setNextState, READY);
        btnEncoder->setActionDown(setNextState, READY);
    }

    // CHECK FOR STATE TRANSITIONS
    if (nextState != currentState) {
        currentState = nextState;
    }
}

void stateLoop() {
    switch (currentState) {
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
