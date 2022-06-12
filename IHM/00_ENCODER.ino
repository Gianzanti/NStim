class Rotary {
   private:
    void (*rotaryActionCCW)(byte) = NULL;
    void (*rotaryActionCW)(byte) = NULL;
    byte argCCW = NULL;
    byte argCW = NULL;

    unsigned long dbDelay;
    unsigned long lastDebounceTime;

    byte currentState;
    byte previousState;
    int encoder_direction;

   public:
    Rotary() {
        currentState = 0;
        previousState = 0;
        encoder_direction = 0;
        lastDebounceTime = 0;
        dbDelay = 100;
    }

    void setup() {
        previousState = analogRead(INPUT_ROTARY_CLK) > 512;
    }

    void loop() {
        byte dat = 0;
        currentState = analogRead(INPUT_ROTARY_CLK) > 512;
        dat = analogRead(INPUT_ROTARY_DAT) > 512;

        if (currentState != previousState) {
            if ((millis() - lastDebounceTime) > dbDelay) {
                lastDebounceTime = millis();
                if (dat != currentState) {
                    encoder_direction = 1;  // CW
#if defined(DEBUG) && defined(DETAIL)
                    Serial.println(F("[INFO] Rotating clockwise"));
#endif
                    if (rotaryActionCW != NULL) {
                        rotaryActionCW(argCW);
                    }
                } else {
                    encoder_direction = -1;  // CCW
#if defined(DEBUG) && defined(DETAIL)
                    Serial.println(F("[INFO] Rotating counter clockwise"));
#endif
                    if (rotaryActionCCW != NULL) {
                        rotaryActionCCW(argCCW);
                    }
                }
            }
            previousState = currentState;
        }
    }

    void setActionCW(void (*func)(byte), byte arg = NULL) {
        if (func != NULL) {
            rotaryActionCW = func;
            argCW = arg;
        }
    }

    void setActionCCW(void (*func)(byte), byte arg = NULL) {
        if (func != NULL) {
            rotaryActionCCW = func;
            argCCW = arg;
        }
    }

    void clearActionCW() {
        rotaryActionCW = NULL;
        argCW = NULL;
    }

    void clearActionCCW() {
        rotaryActionCCW = NULL;
        argCCW = NULL;
    }

    void clearAllActions() {
        clearActionCW();
        clearActionCCW();
    }
};

Rotary *rtrEncoder = new Rotary();  // ROTARY ENCODER
