// byte countLoops = 0;
void loop() {
    btnGreen->loop();
    btnRed->loop();
    btnBlue->loop();
    btnEncoder->loop();
    rtrEncoder->loop();
    stateLoop();
    // countLoops++;
    //    if (countLoops > 30) {
    //      screenMonitor();
    //      countLoops = 0;
    //    }
}
