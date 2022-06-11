//==================================================================================================================
/*

  Gerenciamento da geração de sinal e controle de potência

  Na geração de sinal, o sistema irá gerar uma tensão através do DAC indo para o VCO e

  MCU: Atmega328P AU 32 pinos
  Clock: Externo 16 MHz
  Ciclo de Máquina:

  Compilador:  Arduino v 1.8.15

  Autor: Renan Araujo de Matos  Data: Novembro 2021

*/
//==================================================================================================================
#define DEBUG

#include <Wire.h>
#define MCP4725 0x61
#define DAC_PIN A0
#define FEEDBACK_CURRENT_PIN A7

#define CONTROL_BYTE 0b01000000
#define BAUD_RATE 115200
#define ZERO_DAC 4095

float currentSetpoint = 15.0;
bool stimulationIsOn = false;

unsigned int STDAC = 0;
unsigned int DCSTIM = 0;
float ACSTIM = 0.0;
float CTODAC = 0.0;  // current to DAC

void setup() {
    Wire.begin();
    Serial.begin(BAUD_RATE);
    pinMode(DAC_PIN, OUTPUT);
    pinMode(FEEDBACK_CURRENT_PIN, INPUT);
    digitalWrite(DAC_PIN, HIGH);
    writeToDAC(ZERO_DAC);

#if defined(DEBUG)
    Serial.println("[INFO] Signal Manager Started");
#endif

    setStimulusCurrent(currentSetpoint);
    stimulationIsOn = true;
}

void loop() {
    if (stimulationIsOn) {
        getFeedbackCurrent();
        float errorCurrent = currentSetpoint - ACSTIM;
        if (errorCurrent > 0.00) {
            writeToDAC(STDAC++);
        } else if (errorCurrent < 0.00) {
            writeToDAC(STDAC--);
        }
        delay(10);
    }
}

void setStimulusCurrent(float current) {
    if (current > 0.0 && current < 21.0) {
        CTODAC = ((current * 0.196) + 1.08) * 819;
        STDAC = CTODAC;
#if defined(DEBUG)
        Serial.println("Current Set Point: " + String(current) + " mA");
        Serial.println("STDAC: " + String(STDAC));
#endif
    }
}

void getFeedbackCurrent() {
    DCSTIM = analogRead(FEEDBACK_CURRENT_PIN);
    ACSTIM = DCSTIM * 0.004882812 * 1000 / 244;
#if defined(DEBUG)
    Serial.println("DCSTIM: " + String(DCSTIM) + " | ACSTIM: " + String(ACSTIM) + " mV");
#endif
}

void writeToDAC(unsigned int value) {
    byte MSB = value >> 4;
    byte LSB = value << 4;
    Wire.beginTransmission(MCP4725);  // Joins I2C bus with MCP4725 with 0x61 address
    Wire.write(CONTROL_BYTE);         // Sends the control byte to I2C
    Wire.write(MSB);                  // Sends the MSB to I2C
    Wire.write(LSB);                  // Sends the LSB to I2C
    Wire.endTransmission();           // Ends the transmission
}
