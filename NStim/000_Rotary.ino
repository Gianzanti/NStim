class Rotary
{
  private:
    void (*rotaryActionCCW)(int) = NULL;
    void (*rotaryActionCW)(int) = NULL;

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
    // ~Rotary();

    void setup() {
      previousState = analogRead(INPUT_ROTARY_CLK) > 512;
    }

    void loop() {
      byte dat = 0;
      currentState = analogRead(INPUT_ROTARY_CLK) > 512;
      dat = analogRead(INPUT_ROTARY_DAT) > 512;

      if (currentState != previousState) {
        if ( (millis() - lastDebounceTime) > dbDelay ) {
          lastDebounceTime = millis();
          if ( dat != currentState ) {
            encoder_direction = 1;   // CW
            debugPrintln("Rotating clockwise");
            if (rotaryActionCW != NULL) {
              rotaryActionCW(encoder_direction);
            }
          } else {
            encoder_direction = -1;    // CCW
            debugPrintln("Rotating counter clockwise");
            if (rotaryActionCCW != NULL) {
              rotaryActionCCW(encoder_direction);
            }
          }
        }
        previousState = currentState;
      }
    }

    void setActionCW(void (*func)(int)) {
      if (func != NULL) {
        rotaryActionCW = func;
      }
    }

    void setActionCCW(void (*func)(int)) {
      if (func != NULL) {
        rotaryActionCCW = func;
      }
    }
};

Rotary *rtrEncoder = new Rotary();     // ROTARY ENCODER
