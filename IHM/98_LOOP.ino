void loop() {
    btnGreen->loop();
    btnRed->loop();
    btnBlue->loop();
    btnEncoder->loop();
    rtrEncoder->loop();
    stateLoop();
    screenMonitor();
}
