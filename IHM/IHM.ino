//#define LCM_ENABLED
//#define I2C_ENABLED
#define DEBUG
//#define DETAIL

// INCLUDE LIBRARIES
#if defined(LCM_ENABLED)
#include <SoftwareSerial.h>
#include <UnicViewAD.h>
#endif

#if defined(I2C_ENABLED)
#include <Wire.h>
#endif

#include <EEPROM.h>

// DEFINES AND CONSTANTS

// ROTARY ENCODER
#define INPUT_ROTARY_CLK PIN_A7  // Rotary Encoder Input Clock Pin
#define INPUT_ROTARY_DAT PIN_A6  // Rotary Encoder Input Data Pin

// LEDS
#define LED_R 5  //
#define LED_G 3  //
#define LED_B 6  //

// BUTTONS
#define BTN_START 7
#define BTN_STOP 8
#define BTN_ONOFF 4
#define BTN_ENCODER 2

// COMMUNICATION
#define SOFT_TX 11
#define SOFT_RX 10
#define BAUD_RATE 115200
//#define IHM
#define I2C_IHM_ADDRESS 0xA1  // IHM I2C ADDRESS
#define I2C_ANSWER_SIZE 2

// UNICVIEW SCREENS
#define PID_INTRO 0
#define PID_BR_MAIN 10
#define PID_EN_MAIN 40
#define PID_BR_CONFIG_WAVE_POLE 25
#define PID_EN_CONFIG_WAVE_POLE 55
#define PID_BR_CONFIG_WAVE_PHASE 26
#define PID_EN_CONFIG_WAVE_PHASE 56
#define PID_BR_CONFIG_WAVE_TRAIN 27
#define PID_EN_CONFIG_WAVE_TRAIN 57
#define PID_BR_CONFIG_WAVE_TRAIN_DETAILS_PULSE 29
#define PID_EN_CONFIG_WAVE_TRAIN_DETAILS_PULSE 59
#define PID_BR_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL 30
#define PID_EN_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL 60
#define PID_BR_CONFIG_WAVE_TRAIN_DETAILS_EXIT 31
#define PID_EN_CONFIG_WAVE_TRAIN_DETAILS_EXIT 61
#define PID_BR_CONFIG_WAVE_TRAIN_EXIT 28
#define PID_EN_CONFIG_WAVE_TRAIN_EXIT 58
#define PID_BR_CONFIG_CURRENT 35
#define PID_EN_CONFIG_CURRENT 65
#define PID_BR_CONFIG_PERIOD 34
#define PID_EN_CONFIG_PERIOD 64
#define PID_BR_CONFIG_FREQUENCY 33
#define PID_EN_CONFIG_FREQUENCY 63
#define PID_BR_CONFIG_GENERAL_USER 12
#define PID_EN_CONFIG_GENERAL_USER 42
#define PID_BR_CONFIG_GENERAL_USER_LANG 16
#define PID_EN_CONFIG_GENERAL_USER_LANG 46
#define PID_BR_CONFIG_GENERAL_USER_SOUND 17
#define PID_EN_CONFIG_GENERAL_USER_SOUND 47
#define PID_BR_CONFIG_GENERAL_USER_BACKLIGHT 18
#define PID_EN_CONFIG_GENERAL_USER_BACKLIGHT 48
#define PID_BR_CONFIG_GENERAL_USER_EXIT 19
#define PID_EN_CONFIG_GENERAL_USER_EXIT 49
#define PID_BR_CONFIG_GENERAL_MODE 13
#define PID_EN_CONFIG_GENERAL_MODE 43
#define PID_BR_CONFIG_GENERAL_MODE_CORTICAL 21
#define PID_EN_CONFIG_GENERAL_MODE_CORTICAL 51
#define PID_BR_CONFIG_GENERAL_MODE_LOCALIZATION 22
#define PID_EN_CONFIG_GENERAL_MODE_LOCALIZATION 52
#define PID_BR_CONFIG_GENERAL_MODE_EXIT 23
#define PID_EN_CONFIG_GENERAL_MODE_EXIT 53
#define PID_BR_CONFIG_GENERAL_EXIT 14
#define PID_EN_CONFIG_GENERAL_EXIT 44

// LIMITS
#define STIM_CURRENT_MIN 1
#define STIM_CURRENT_MAX 20
#define STIM_PERIOD_MIN 1
#define STIM_PERIOD_MAX 999
#define STIM_FREQUENCY_MIN 1
#define STIM_FREQUENCY_MAX 999
#define STIM_TRAIN_COUNT_MIN 1
#define STIM_TRAIN_COUNT_MAX 20
#define STIM_TRAIN_INTERVAL_MIN 1
#define STIM_TRAIN_INTERVAL_MAX 999
#define STIM_SOUNDLEVEL_MIN 0
#define STIM_SOUNDLEVEL_MAX 100
#define STIM_BACKLIGHT_MIN 0
#define STIM_BACKLIGHT_MAX 100

// STEPS
#define STIM_CURRENT_STEP 1
#define STIM_PERIOD_STEP 1
#define STIM_FREQUENCY_STEP 1
#define STIM_TRAIN_COUNT_STEP 1
#define STIM_TRAIN_INTERVAL_STEP 1
#define STIM_SOUNDLEVEL_STEP 10
#define STIM_BACKLIGHT_STEP 10

// EEPROM ADDRESSES
#define FREQUENCY_ADDRESS 0x02
#define PERIOD_ADDRESS 0x03
#define BIPOLAR_ADDRESS 0x04
#define TRAIN_ADDRESS 0x05
#define TRAIN_COUNT_ADDRESS 0x06
#define NEGATIVE_PHASE_ADDRESS 0x07
#define TYPE_CORTICAL_ADDRESS 0x08
#define CURRENT_ADDRESS 0x09
#define LANGUAGE_ADDRESS 0x0A
#define SOUNDLEVEL_ADDRESS 0x0B
#define BACKLIGHT_ADDRESS 0x0C
#define TRAIN_INTERVAL_ADDRESS 0x0D

// STATES
#define NONE 0x00
#define LOADING_FROM_EPROM 0x01
#define MOUNT_SCREEN 0x02
#define READY 0x04
#define STIMULATING 0x05
#define WITH_ERROR 0x06

#define CONFIG_CURRENT 0x07
#define CONFIG_PERIOD 0x08
#define CONFIG_FREQUENCY 0x09

#define CONFIG_WAVE_POLE 0x0A
#define CONFIG_WAVE_PHASE 0x0B
#define CONFIG_WAVE_TRAIN 0x0C
#define CONFIG_WAVE_TRAIN_PULSES 0x0D
#define CONFIG_WAVE_TRAIN_INTERVAL 0x0E
#define CONFIG_WAVE_TRAIN_EXIT 0x0F
#define CONFIG_WAVE_EXIT 0x10

#define CONFIG_GENERAL_USER 0x11
#define CONFIG_GENERAL_USER_LANG 0x12
#define CONFIG_GENERAL_USER_SOUND 0x13
#define CONFIG_GENERAL_USER_BACKLIGHT 0x14
#define CONFIG_GENERAL_USER_EXIT 0x15
#define CONFIG_GENERAL_MODE 0x16
#define CONFIG_GENERAL_MODE_CORTICAL 0x17
#define CONFIG_GENERAL_MODE_LOCALIZATION 0x18
#define CONFIG_GENERAL_MODE_EXIT 0x19
#define CONFIG_GENERAL_EXIT 0x1A

// GLOBAL VARIABLES
#if defined(LCM_ENABLED)
SoftwareSerial softSerial(SOFT_RX, SOFT_TX);
LCM Lcm(softSerial);  // Inicializa o LCM com o nome "Lcm" e com o parâmetro da Serial de comunicação com o Display
LcmVar VP_ErrorControl(1);
LcmVar VP_BatteryLevel(3);
LcmVar VP_VolumeLevel(5);
LcmVar VP_StimulusMode(7);
LcmVar VP_StimulusState(11);
LcmVar VP_SetCurrent(13);
LcmVar VP_SetPeriod(15);
LcmVar VP_SetFrequency(17);
LcmVar VP_MeasuredCharge(19);
LcmVar VP_MeasuredImpedance(21);
LcmVar VP_Language(23);
LcmVar VP_Backlight(25);
LcmVar VP_TrainPulses(27);
LcmVar VP_TrainInterval(29);
LcmVar VP_MeasuredCurrent(31);
LcmVar VP_WaveBipolar(33);
LcmVar VP_WavePhase(35);
LcmVar VP_TrainEnabled(37);
#endif

byte currentState = NONE;
byte lastState = NONE;
byte nextState = NONE;

bool STIM_ON = false;
bool STIM_BIPOLAR = false;
bool STIM_TRAIN = false;
bool STIM_NEGATIVE_PHASE = false;
bool STIM_CORTICAL = false;
bool STIM_LANG_ENGLISH = false;

byte STIM_TRAIN_COUNT = 3;
byte STIM_SOUNDLEVEL = 50;
byte STIM_BACKLIGHT = 50;
byte STIM_ERROR = 0;

unsigned int STIM_FREQUENCY = 50;
unsigned int STIM_PERIOD = 50;
unsigned int STIM_TRAIN_INTERVAL = 50;
unsigned int STIM_CURRENT = 5;
unsigned int STIM_MEASURED_CURRENT = 0;
unsigned int STIM_MEASURED_IMPEDANCE = 0;
unsigned int STIM_MEASURED_BATTERY = 0;
unsigned int STIM_MEASURED_CHARGE = 0;

byte currentScreen = PID_INTRO;
