void loop() {
    btnGreen->loop();
    btnRed->loop();
    btnBlue->loop();
    btnEncoder->loop();
    rtrEncoder->loop();
    stateLoop();

    //  // *******************************************************************************************************************************************
    //  // * Seleção do tipo de controle do servo
    //  // *******************************************************************************************************************************************
    //  if (ledButton.available())
    //  { // Verifica se chegou algo no objeto ledButton
    //    debugPrintln("ledButton available");
    //    // Serial.println("picId" + picId);
    //    int LED_estado = ledButton.getData(); // Verifica se o valor recebido pelo
    //    debugPrintln("got data");
    //    if (LED_estado == HIGH)
    //    { // "ledButton" é igual a ledOn
    //      debugPrintln("turn on led...");
    //      digitalWrite(LED_3, LOW); // Liga o LED
    //      // Lcm.changePicId(0);
    //      // delay(1000);
    //    }
    //    if (LED_estado == LOW)
    //    { // Verifica se o valor recebido pelo "ledButton" é igual a "ledOff"
    //      debugPrintln("turn off led...");
    //      digitalWrite(LED_3, HIGH); // Desliga o LED
    //      // Lcm.calibrate();
    //      // delay(1000);
    //    }
    //  }
}
