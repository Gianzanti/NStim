#define DEFAULT_DELAY 30

class Button {
   private:
    unsigned long lastDebounceTime;
    int state;
    int lastState;
    int pin;
    unsigned long dbDelay;
    void (*btnActionDown)(byte) = NULL;
    void (*btnActionUp)(byte) = NULL;
    byte argDown = NULL;
    byte argUp = NULL;

   public:
    Button(int btnPin, unsigned long debounceDelay = DEFAULT_DELAY) {
        pin = btnPin;
        lastDebounceTime = 0;
        lastState = HIGH;
        state = HIGH;
        dbDelay = debounceDelay;
        pinMode(pin, INPUT);
    }

    void loop() {
        int reading = digitalRead(pin);

        if (reading != lastState) {
            lastDebounceTime = millis();
        }

        if ((millis() - lastDebounceTime) > dbDelay) {
            if (reading != state) {
                state = reading;

#if defined(DEBUG) && defined(DETAIL)
                Serial.println("[INFO] Button at " + String(pin) + " | State: " + String(state));
#endif

                if (state == HIGH && btnActionUp != NULL) {
                    btnActionUp(argUp);
                }

                if (state == LOW && btnActionDown != NULL) {
                    btnActionDown(argDown);
                }
            }
        }
        lastState = reading;
    }

    void setActionDown(void (*func)(byte), byte arg = NULL) {
        if (func != NULL) {
            btnActionDown = func;
            argDown = arg;
        }
    }

    void setActionUp(void (*func)(byte), byte arg = NULL) {
        if (func != NULL) {
            btnActionUp = func;
            argUp = arg;
        }
    }

    void clearActionUp() {
        btnActionUp = NULL;
        argUp = NULL;
    }

    void clearActionDown() {
        btnActionDown = NULL;
        argDown = NULL;
    }

    void clearAllActions() {
        clearActionUp();
        clearActionDown();
    }
};

Button *btnGreen = new Button(BTN_START);      // GREEN BUTTON
Button *btnRed = new Button(BTN_STOP);         // RED BUTTON
Button *btnBlue = new Button(BTN_ONOFF);       // BLUE BUTTON
Button *btnEncoder = new Button(BTN_ENCODER);  // ENCODER BUTTON
