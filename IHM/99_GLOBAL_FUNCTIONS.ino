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

void clearAllActions() {
    btnGreen->clearAllActions();
    btnBlue->clearAllActions();
    btnEncoder->clearAllActions();
    btnRed->clearAllActions();
    rtrEncoder->clearAllActions();
}

void increaseCurrentAtStimulus() {
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

void decreaseCurrentAtStimulus() {
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

void setNextState(byte next) {
    nextState = next;
}

void monitorSlave() {
    askSlave(READ_STIM_MEASURED_CURRENT);
    //  askSlave(READ_STIM_MEASURED_IMPEDANCE);
    //  askSlave(READ_STIM_MEASURED_BATTERY);
    //  askSlave(READ_STIM_ERROR);

    // MONITORAR TODAS AS VARIÁVEIS DE UMA ÚNICA VEZ, MANDAR CASO ATUALIZADO JÁ PARA O DISPLAY E CHECAR SE TEM ERRO
    // COLOCAR ESSA ROTINA NO LOOP PRINCIPAL E CHECAR QUAIS VARIÁVEIS TRATAR, CONFORME O ESTADO ATUAL

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

    //    if (STIM_ERROR != 0) {
    //        currentState = WITH_ERROR;
    //    }
}

void changeWavePole() {
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

void changeWavePhase() {
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

void changeWaveTrain() {
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

void increaseCurrentAtConfig() {
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

void decreaseCurrentAtConfig() {
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

void setCurrentAtConfig() {
#if defined(LCM_ENABLED)
    VP_SetCurrent.write(STIM_CURRENT);
#endif

    EEPROM.write(CURRENT_ADDRESS, STIM_CURRENT);
    setSlave(SET_STIM_CURRENT, STIM_CURRENT);

#if defined(DEBUG)
    Serial.println("[SET] Set current to: " + String(STIM_CURRENT));
#endif
};

void increasePeriodAtConfig() {
    if (STIM_PERIOD + 1 <= STIM_PERIOD_MAX) {
        STIM_PERIOD += 1;

#if defined(LCM_ENABLED)
        VP_SetPeriod.write(STIM_PERIOD);
#endif

#if defined(DEBUG)
        Serial.println("[CHANGE] Increased Period to: " + String(STIM_PERIOD));
#endif
    }
};

void decreasePeriodAtConfig() {
    if (STIM_PERIOD - 1 >= STIM_PERIOD_MIN) {
        STIM_PERIOD -= 1;

#if defined(LCM_ENABLED)
        VP_SetPeriod.write(STIM_PERIOD);
#endif

#if defined(DEBUG)
        Serial.println("[CHANGE] Increased Period to: " + String(STIM_PERIOD));
#endif
    }
};

void setPeriodAtConfig() {
#if defined(LCM_ENABLED)
    VP_SetPeriod.write(STIM_PERIOD);
#endif

    EEPROM.write(PULSE_WIDTH_ADDRESS, STIM_PERIOD);
    setSlave(SET_STIM_PERIOD, STIM_PERIOD);

#if defined(DEBUG)
    Serial.println("[SET] Set Period to: " + String(STIM_PERIOD));
#endif
};

void increaseFrequencyAtConfig() {
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

void decreaseFrequencyAtConfig() {
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

void setFrequencyAtConfig() {
#if defined(LCM_ENABLED)
    VP_SetFrequency.write(STIM_FREQUENCY);
#endif

    EEPROM.write(FREQUENCY_ADDRESS, STIM_FREQUENCY);
    setSlave(SET_STIM_FREQUENCY, STIM_FREQUENCY);

#if defined(DEBUG)
    Serial.println("[SET] Set Frequency to: " + String(STIM_FREQUENCY));
#endif
};

void increasePulses() {
    if (STIM_TRAIN_COUNT + 1 <= STIM_TRAIN_COUNT_MAX) {
        STIM_TRAIN_COUNT += 1;

#if defined(LCM_ENABLED)
        VP_TrainPulses.write(STIM_TRAIN_COUNT);
#endif

        EEPROM.write(TRAIN_COUNT_ADDRESS, STIM_TRAIN_COUNT);
        setSlave(SET_STIM_TRAIN_COUNT, STIM_TRAIN_COUNT);

#if defined(DEBUG)
        Serial.println("[CHANGE] Increased number of pulses to: " + String(STIM_TRAIN_COUNT));
#endif
    }
};

void decreasePulses() {
    if (STIM_TRAIN_COUNT - 1 >= STIM_TRAIN_COUNT_MIN) {
        STIM_TRAIN_COUNT -= 1;

#if defined(LCM_ENABLED)
        VP_SetFrequency.write(STIM_TRAIN_COUNT);
#endif

        EEPROM.write(TRAIN_COUNT_ADDRESS, STIM_TRAIN_COUNT);
        setSlave(SET_STIM_TRAIN_COUNT, STIM_TRAIN_COUNT);

#if defined(DEBUG)
        Serial.println("[CHANGE] Decreased number of pulses to: " + String(STIM_TRAIN_COUNT));
#endif
    }
};

void increaseInterval() {
    if (STIM_TRAIN_INTERVAL + 1 <= STIM_TRAIN_INTERVAL_MAX) {
        STIM_TRAIN_INTERVAL += 1;

#if defined(LCM_ENABLED)
        VP_TrainPulses.write(STIM_TRAIN_INTERVAL);
#endif

        EEPROM.write(TRAIN_INTERVAL_ADDRESS, STIM_TRAIN_INTERVAL);
        setSlave(SET_STIM_TRAIN_INTERVAL, STIM_TRAIN_INTERVAL);

#if defined(DEBUG)
        Serial.println("[CHANGE] Increased interval between pulses to: " + String(STIM_TRAIN_INTERVAL));
#endif
    }
};

void decreaseInterval() {
    if (STIM_TRAIN_INTERVAL - 1 >= STIM_TRAIN_INTERVAL_MIN) {
        STIM_TRAIN_INTERVAL -= 1;

#if defined(LCM_ENABLED)
        VP_TrainPulses.write(STIM_TRAIN_INTERVAL);
#endif

        EEPROM.write(TRAIN_INTERVAL_ADDRESS, STIM_TRAIN_INTERVAL);
        setSlave(SET_STIM_TRAIN_INTERVAL, STIM_TRAIN_INTERVAL);

#if defined(DEBUG)
        Serial.println("[CHANGE] Decreased interval between pulses to: " + String(STIM_TRAIN_INTERVAL));
#endif
    }
}

void changeLanguage() {
    STIM_LANG_ENGLISH = !STIM_LANG_ENGLISH;
#if defined(LCM_ENABLED)
    VP_Language.write(STIM_LANG_ENGLISH);
    if (!STIM_LANG_ENGLISH) {
        Lcm.changePicId(PID_BR_CONFIG_GENERAL_USER_LANG);
    } else {
        Lcm.changePicId(PID_EN_CONFIG_GENERAL_USER_LANG);
    }
#endif

    EEPROM.write(LANGUAGE_ADDRESS, STIM_LANG_ENGLISH);

#if defined(DEBUG)
    if (!STIM_LANG_ENGLISH) {
        Serial.println(F("[CHANGE] Changed Language to Portuguese"));
    } else {
        Serial.println(F("[CHANGE] Changed Language to English"));
    }
#endif
}

void increaseSoundLevel() {
    if (STIM_SOUNDLEVEL + 1 <= STIM_SOUNDLEVEL_MAX) {
        STIM_SOUNDLEVEL += 1;

#if defined(LCM_ENABLED)
        VP_VolumeLevel.write(STIM_SOUNDLEVEL);
#endif

        EEPROM.write(SOUNDLEVEL_ADDRESS, STIM_SOUNDLEVEL);

#if defined(DEBUG)
        Serial.println("[CHANGE] Increased sound level to: " + String(STIM_SOUNDLEVEL));
#endif
    }
};

void decreaseSoundLevel() {
    if (STIM_SOUNDLEVEL - 1 >= STIM_SOUNDLEVEL_MIN) {
        STIM_SOUNDLEVEL -= 1;

#if defined(LCM_ENABLED)
        VP_VolumeLevel.write(STIM_SOUNDLEVEL);
#endif

        EEPROM.write(SOUNDLEVEL_ADDRESS, STIM_SOUNDLEVEL);

#if defined(DEBUG)
        Serial.println("[CHANGE] Increased sound level to: " + String(STIM_SOUNDLEVEL));
#endif
    }
};

void increaseBacklight() {
    if (STIM_BACKLIGHT + 1 <= STIM_BACKLIGHT_MAX) {
        STIM_BACKLIGHT += 1;

#if defined(LCM_ENABLED)
        Lcm.changeBacklight(STIM_BACKLIGHT);
        VP_Backlight.write(STIM_BACKLIGHT);
#endif

        EEPROM.write(BACKLIGHT_ADDRESS, STIM_BACKLIGHT);

#if defined(DEBUG)
        Serial.println("[CHANGE] Increased backlight to: " + String(STIM_BACKLIGHT));
#endif
    }
};

void decreaseBacklight() {
    if (STIM_BACKLIGHT - 1 >= STIM_BACKLIGHT_MIN) {
        STIM_BACKLIGHT -= 1;

#if defined(LCM_ENABLED)
        Lcm.changeBacklight(STIM_BACKLIGHT);
        VP_Backlight.write(STIM_BACKLIGHT);
#endif

        EEPROM.write(BACKLIGHT_ADDRESS, STIM_BACKLIGHT);

#if defined(DEBUG)
        Serial.println("[CHANGE] Increased backlight to: " + String(STIM_BACKLIGHT));
#endif
    }
};

void setCorticalMode() {
    STIM_CORTICAL = true;

#if defined(LCM_ENABLED)
    VP_StimulusMode.write(STIM_CORTICAL);
#endif

    EEPROM.write(TYPE_CORTICAL_ADDRESS, STIM_CORTICAL);

#if defined(DEBUG)
    Serial.println(F("[CHANGE] Changed Stimulation Mode to Cortical"));
#endif
}

void setLocalizationMode() {
    STIM_CORTICAL = false;

#if defined(LCM_ENABLED)
    VP_StimulusMode.write(STIM_CORTICAL);
#endif

    EEPROM.write(TYPE_CORTICAL_ADDRESS, STIM_CORTICAL);

#if defined(DEBUG)
    Serial.println(F("[CHANGE] Changed Stimulation Mode to Localization"));
#endif
}
