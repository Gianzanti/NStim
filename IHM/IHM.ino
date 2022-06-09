// sudo chmod a+rw /dev/ttyUSB0

// INCLUDE LIBRARIES
#include <Wire.h>
#include <SoftwareSerial.h>
//#include <UnicViewAD.h>
#include <EEPROM.h>

// DEFINES AND CONSTANTS

// DEBUG MODE
#define DEBUG                   true

// ROTARY ENCODER
#define INPUT_ROTARY_CLK        PIN_A7       // Rotary Encoder Input Clock Pin
#define INPUT_ROTARY_DAT        PIN_A6       // Rotary Encoder Input Data Pin

// LEDS
#define LED_INTERNAL           13       // Arduino Nano Internal Led LED_BUILTIN
#define LED_R                   5       // 
#define LED_G                   3       // 
#define LED_B                   6       // 

// BUTTONS
#define BTN_START               7
#define BTN_STOP                8
#define BTN_ONOFF               4
#define BTN_ENCODER             2

#define LED_8                   PIN_A0
#define LED_7                   PIN_A1
#define LED_6                   PIN_A2
#define LED_5                   PIN_A3
#define LED_4                   PIN_A4
#define LED_3                   PIN_A5

// COMMUNICATION
#define SOFT_TX                 11
#define SOFT_RX                 10
#define BAUD_RATE               115200
#define I2C_SLAVE_ADDRESS       4 
#define I2C_ANSWER_SIZE         2

// GLOBAL VARIABLES
//SoftwareSerial softSerial(SOFT_RX, SOFT_TX);
//LCM Lcm(softSerial);             // Inicializa o LCM com o nome "Lcm" e com o parâmetro da Serial de comunicação com o Display
//LcmVar ledButton(16);            // Cria um objeto com o nome de ledButton configurado com o VP 16

enum states {
  NONE,
  SETUP_DONE,
  INITIAL_STATE,
  LOAD_FROM_EPROM,
  LOADING_FROM_EPROM,
  MOUNT_SCREEN,
  MOUNTING_SCREEN,
  SET_SLAVE_STATE,
  GET_SLAVE_STATE,
  READY
};

states currentState = NONE;
states lastState = NONE;

bool STIM_ON = false;
bool STIM_BIPOLAR = false;
bool STIM_TRAIN = false;
bool STIM_NEGATIVE_PHASE = false;
bool STIM_CORTICAL = false;

unsigned int STIM_FREQUENCY = 50;
unsigned int STIM_PULSE_WIDTH = 50;
unsigned int STIM_TRAIN_COUNT = 3;
unsigned int STIM_CURRENT = 5;

bool EEPROM_READ = false;

void turnOnLed()
{
  debugPrintln("LED ON");
  digitalWrite(LED_INTERNAL, LOW);
};

void turnOffLed()
{
  debugPrintln("LED OFF");  
  digitalWrite(LED_INTERNAL, HIGH);
};

void CW() {
  debugPrintln("CW");
}


void CCW() {
  debugPrintln("CCW");
}


// FUNCTION PROTOTYPES

// USER DEFINED FUNCTIONS

// SETUP()

// LOOP()


/**
 * Botão VERDE: inicia estímulo
 * Botão VERMELHO: para estímulo
 * Botão AZUL: Pular entre as funções
 */
