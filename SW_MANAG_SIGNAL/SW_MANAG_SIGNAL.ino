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
#include<Wire.h>
#define MCP4725 0x61
#define DAC0    A0
#define DAC1    A1
#define DAC2    A2
#define CSTIM   A7

float currentSetpoint = 15;
float currentError = 0;
unsigned int STDAC = 0;
unsigned int flagSTIM = 0;
unsigned int DCSTIM = 0;
float ACSTIM = 0;
unsigned int adc;
byte buffer[3];

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(DAC0, OUTPUT);
  pinMode(CSTIM, INPUT);
  digitalWrite(DAC0, 1);
  DAC(4095);
  Serial.println("Teste");
  STCSTIM(currentSetpoint);
}

void loop() {
  if (flagSTIM == 1) {
    CTCSTIM();
  }
}


void CTCSTIM () {
  FBCSTIM();
  currentError = ACSTIM;
  if (currentSetpoint - currentError > 0) {
    STDAC++;
    DAC(STDAC);
  }

  if (currentSetpoint - currentError < 0) {
    STDAC--;
    DAC(STDAC);
  }
}

void STCSTIM(float current) { //SET CURRENT STIMULUS
  float CTODAC = 0; //current to DAC
  if (current > 0 && current < 21) {
    Serial.print("current: ");
    Serial.println(current);
    CTODAC = (current * 0.196) + 1.08;
    Serial.print("CTODAC: ");
    Serial.println(CTODAC);
    CTODAC = CTODAC * 819;
    Serial.print("CTODAC': ");
    Serial.println(CTODAC);
    DAC(CTODAC);
    delay(10);
    FBCSTIM();
    flagSTIM = 1;
    STDAC = CTODAC;
  }
}

void FBCSTIM() { //FEEDBACK CURRRENT STIMULUS
  DCSTIM = analogRead(CSTIM);
  Serial.print("DCSTIM: ");
  Serial.println(DCSTIM);
  ACSTIM = DCSTIM * 0.004882812;
  Serial.print("ACSTIM: ");
  Serial.println(ACSTIM);
  ACSTIM = ACSTIM / 244;
  ACSTIM = ACSTIM * 1000;
  Serial.print("ACSTIM': ");
  Serial.print(ACSTIM);
  Serial.println(" (mV)");


}

void DAC(unsigned int value) {
  buffer[0] = 0b01000000;            //Sets the buffer0 with control byte (010-Sets in Write mode)
  adc = value;          //Read Analog value from pin A0 and convert into digital (0-1023) multiply with 4 gives (0-4096)
  buffer[1] = adc >> 4;              //Puts the most significant bit values
  buffer[2] = adc << 4;              //Puts the Least significant bit values
  Wire.beginTransmission(MCP4725);         //Joins I2C bus with MCP4725 with 0x61 address

  Wire.write(buffer[0]);            //Sends the control byte to I2C
  Wire.write(buffer[1]);            //Sends the MSB to I2C
  Wire.write(buffer[2]);            //Sends the LSB to I2C

  Wire.endTransmission();           //Ends the transmission

}
