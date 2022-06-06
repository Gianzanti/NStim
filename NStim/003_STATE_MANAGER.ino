
void ST_SETUP_DONE() {
  if (currentState != lastState) {
    // ENTERING THE STATE, DO INITIALIZATION STUFF
    lastState = currentState;
    debugPrintln("[INFO] NStim Setup Process Finished");
  }

  // PERFORM STATE TASKS

  // CHECK FOR STATE TRANSITIONS
  currentState = LOADING_FROM_EPROM;

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
  }
}

void ST_LOADING_FROM_EPROM() {
  if (currentState != lastState) {
    // ENTERING THE STATE, DO INITIALIZATION STUFF
    lastState = currentState;
    debugPrintln("[INFO] Loading data from EPROM");
    readConfigFromEEPROM();
  }

  // PERFORM STATE TASKS

  // CHECK FOR STATE TRANSITIONS
  if (EEPROM_READ == 1) {
    currentState = SET_SLAVE_STATE;
  }

  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
  }
}

void ST_SET_SLAVE_STATE() {
  if (currentState != lastState) {
    // ENTERING THE STATE, DO INITIALIZATION STUFF
    lastState = currentState;
    debugPrintln("[INFO] Setting Slave Unit");

    setSlave(SET_STIM_ON, 0);
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
  }
}

void ST_MOUNT_SCREEN() {
  if (currentState != lastState) {
    // ENTERING THE STATE, DO INITIALIZATION STUFF
    lastState = currentState;
    debugPrintln("[INFO] Mounting Screen");
  }

  // PERFORM STATE TASKS

  // CHECK FOR STATE TRANSITIONS
  currentState = READY;
  
  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
  }
}

void ST_READY(){
  if (currentState != lastState) {
    lastState = currentState;
    debugPrintln("[INFO] Ready to stimulate");

    btnStart->setActionDown(turnOnLed);
    btnStop->setActionDown(turnOffLed);
    rtrEncoder->setActionCW(CW);
    rtrEncoder->setActionCCW(CCW);  
  }

  // PERFORM STATE TASKS

  // CHECK FOR STATE TRANSITIONS
  currentState = READY;
  
  if (currentState != lastState) {
    // LEAVING THE STATE, DO CLEAN UP STUFF
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
      
    default:
      break;

  }
      
}
