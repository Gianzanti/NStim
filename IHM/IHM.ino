// sudo chmod a+rw /dev/ttyUSB0

// INCLUDE LIBRARIES
//#define LCM_ENABLED

#if defined(LCM_ENABLED)
#include <SoftwareSerial.h>
#include <UnicViewAD.h>
#endif

//#define I2C_ENABLED
#if defined(I2C_ENABLED)
#include <Wire.h>
#endif

#include <EEPROM.h>

// DEFINES AND CONSTANTS

// DEBUG MODE
#define DEBUG
//#define DETAIL

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

//#define LED_8                   PIN_A0
//#define LED_7                   PIN_A1
//#define LED_6                   PIN_A2
//#define LED_5                   PIN_A3
//#define LED_4                   PIN_A4
//#define LED_3                   PIN_A5

// COMMUNICATION
#define SOFT_TX 11
#define SOFT_RX 10
#define BAUD_RATE 115200
#define I2C_SLAVE_ADDRESS 4
#define I2C_ANSWER_SIZE 2

// UNICVIEW SCREENS
#define PID_INTRO 0                                  // Endereço da tela de introdução
#define PID_BR_MAIN 4                                // Endereço da tela principal - Português
#define PID_EN_MAIN 4                                // Endereço da tela principal - Inglês
#define PID_BR_CONFIG_WAVE_POLE 4                    // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_WAVE_POLE 4                    // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_WAVE_PHASE 4                   // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_WAVE_PHASE 4                   // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_WAVE_TRAIN 4                   // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_WAVE_TRAIN 4                   // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_WAVE_TRAIN_DETAILS_PULSE 4     // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_WAVE_TRAIN_DETAILS_PULSE 4     // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL 4  // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL 4  // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_WAVE_TRAIN_DETAILS_EXIT 4      // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_WAVE_TRAIN_DETAILS_EXIT 4      // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_WAVE_TRAIN_EXIT 4              // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_WAVE_TRAIN_EXIT 4              // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_CURRENT 4                      // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_CURRENT 4                      // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_PERIOD 4                       // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_PERIOD 4                       // Endereço da 1ª tela de configuração - Inglês
#define PID_BR_CONFIG_FREQUENCY 4                    // Endereço da 1ª tela de configuração - Português
#define PID_EN_CONFIG_FREQUENCY 4                    // Endereço da 1ª tela de configuração - Inglês

#define PID_BR_CONFIG_GENERAL_USER 0x0B
#define PID_EN_CONFIG_GENERAL_USER 0x0B
#define PID_BR_CONFIG_GENERAL_USER_LANG 0x0C
#define PID_EN_CONFIG_GENERAL_USER_LANG 0x0C
#define PID_BR_CONFIG_GENERAL_USER_SOUND 0x0D
#define PID_EN_CONFIG_GENERAL_USER_SOUND 0x0D
#define PID_BR_CONFIG_GENERAL_USER_BACKLIGHT 0x0E
#define PID_EN_CONFIG_GENERAL_USER_BACKLIGHT 0x0E
#define PID_BR_CONFIG_GENERAL_USER_EXIT 0x0F
#define PID_EN_CONFIG_GENERAL_USER_EXIT 0x0F
#define PID_BR_CONFIG_GENERAL_MODE 0x10
#define PID_EN_CONFIG_GENERAL_MODE 0x10
#define PID_BR_CONFIG_GENERAL_MODE_CORTICAL 0x11
#define PID_EN_CONFIG_GENERAL_MODE_CORTICAL 0x11
#define PID_BR_CONFIG_GENERAL_MODE_LOCALIZATION 0x12
#define PID_EN_CONFIG_GENERAL_MODE_LOCALIZATION 0x12
#define PID_BR_CONFIG_GENERAL_MODE_EXIT 0x13
#define PID_EN_CONFIG_GENERAL_MODE_EXIT 0x13
#define PID_BR_CONFIG_GENERAL_EXIT 0x14
#define PID_EN_CONFIG_GENERAL_EXIT 0x14

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

// EEPROM ADDRESSES
#define FREQUENCY_ADDRESS 0x02
#define PULSE_WIDTH_ADDRESS 0x03
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

// I2C COMMUNICATION - BYTE READ REGISTERS
#define READ_STIM_ON 0x01                  // 1: stimulation is on | 0: stimulation is off
#define READ_STIM_FREQUENCY 0x02           // 0 - 100: operation frequency (Hz)
#define READ_STIM_PULSE_WIDTH 0x03         // 0 - 100: pulse width (s)
#define READ_STIM_BIPOLAR 0x04             // 0: monopolar mode | 1: bipolar mode
#define READ_STIM_TRAIN 0x05               // 0: pulse train disabled | 1: pulse train enabled
#define READ_STIM_TRAIN_COUNT 0x06         // 0 - 100: total number of pulses, if enabled
#define READ_STIM_NEGATIVE_PHASE 0x07      // 0: positive | 1: negative
#define READ_STIM_TYPE_CORTICAL 0x08       // 0: localization | 1: cortical
#define READ_STIM_CURRENT 0x09             // 0 - 100: operation current (mA)
#define READ_STIM_MEASURED_CURRENT 0x0A    // 0 - 100: operation current (mA)
#define READ_STIM_MEASURED_IMPEDANCE 0x0B  //
#define READ_STIM_MEASURED_BATTERY 0x0C    //
#define READ_STIM_ERROR 0x0D               //

// I2C COMMUNICATION - BYTE SET REGISTERS
#define SET_STIM_ON 0xA1           // 1: stimulation is on | 0: stimulation is off
#define SET_STIM_FREQUENCY 0xA2    // 0 - 100: operation frequency (Hz)
#define SET_STIM_PULSE_WIDTH 0xA3  // 0 - 100: pulse width (ms)
#define SET_STIM_BIPOLAR 0xA4      // 0: monopolar mode | 1: bipolar mode
#define SET_STIM_TRAIN 0xA5        // 0: pulse train disabled | 1: pulse train enabled
#define SET_STIM_TRAIN_COUNT 0xA6  // 0 - 100: total number of pulses, if enabled
#define SET_STIM_TRAIN_INTERVAL 0xA7
#define SET_STIM_NEGATIVE_PHASE 0xA8  // 0: positive | 1: negative
#define SET_STIM_TYPE_CORTICAL 0xA9   // 0: localization | 1: cortical
#define SET_STIM_CURRENT 0xAA         // 0 - 100: operation current (mA)

// I2C COMMUNICATION - BYTE COMMUNICATIONS
#define ERROR_NO_REGISTER 0xFF  // The register doesn't exist

// STATES
#define NONE 0x01
#define SETUP_DONE 0x02
#define INITIAL_STATE 0x03
#define LOAD_FROM_EPROM 0x04
#define LOADING_FROM_EPROM 0x05
#define MOUNT_SCREEN 0x06
#define MOUNTING_SCREEN 0x07
#define SET_SLAVE_STATE 0x08
#define GET_SLAVE_STATE 0x09
#define READY 0x0A
#define STIMULATING 0x0B
#define WITH_ERROR 0x0C
#define CONFIG_WAVE_POLE 0x0D
#define CONFIG_WAVE_PHASE 0x0E
#define CONFIG_WAVE_TRAIN 0x0F
#define CONFIG_WAVE_TRAIN_PULSES 0x10
#define CONFIG_WAVE_TRAIN_INTERVAL 0x11
#define CONFIG_WAVE_TRAIN_EXIT 0x12
#define CONFIG_WAVE_EXIT 0x13
#define CONFIG_CURRENT 0x14
#define CONFIG_PERIOD 0x15
#define CONFIG_FREQUENCY 0x16
#define CONFIG_GENERAL_USER 0x17
#define CONFIG_GENERAL_USER_LANG 0x18
#define CONFIG_GENERAL_USER_SOUND 0x19
#define CONFIG_GENERAL_USER_BACKLIGHT 0x1A
#define CONFIG_GENERAL_USER_EXIT 0x1B
#define CONFIG_GENERAL_MODE 0x1C
#define CONFIG_GENERAL_MODE_CORTICAL 0x1D
#define CONFIG_GENERAL_MODE_LOCALIZATION 0x1E
#define CONFIG_GENERAL_MODE_EXIT 0x1F
#define CONFIG_GENERAL_EXIT 0x20

// SCREENS
#define SCREEN_READY 0x00
#define SCREEN_CONFIG_WAVE_POLE 0x01
#define SCREEN_CONFIG_WAVE_PHASE 0x02
#define SCREEN_CONFIG_WAVE_TRAIN 0x03
#define SCREEN_CONFIG_WAVE_TRAIN_PULSES 0x04
#define SCREEN_CONFIG_WAVE_TRAIN_INTERVAL 0x05
#define SCREEN_CONFIG_WAVE_TRAIN_EXIT 0x06
#define SCREEN_CONFIG_WAVE_EXIT 0x07
#define SCREEN_CONFIG_CURRENT 0x08
#define SCREEN_CONFIG_PERIOD 0x09
#define SCREEN_CONFIG_FREQUENCY 0x0A
#define SCREEN_CONFIG_GENERAL_USER 0x0B
#define SCREEN_CONFIG_GENERAL_MODE 0x0C
#define SCREEN_CONFIG_GENERAL_USER_LANG 0x0D
#define SCREEN_CONFIG_GENERAL_USER_SOUND 0x0E
#define SCREEN_CONFIG_GENERAL_USER_BACKLIGHT 0x0F
#define SCREEN_CONFIG_GENERAL_USER_EXIT 0x11
#define SCREEN_CONFIG_GENERAL_MODE_CORTICAL 0x12
#define SCREEN_CONFIG_GENERAL_MODE_LOCALIZATION 0x13
#define SCREEN_CONFIG_GENERAL_MODE_EXIT 0x14
#define SCREEN_CONFIG_GENERAL_EXIT 0x15

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

bool STIM_ON = false;
bool STIM_BIPOLAR = false;
bool STIM_TRAIN = false;
bool STIM_NEGATIVE_PHASE = false;
bool STIM_CORTICAL = false;
bool STIM_LANGUAGE = false;  // 0 - PORTUGUESE | 1 - ENGLISH
bool STIM_CONFIG = false;

unsigned int STIM_FREQUENCY = 50;
unsigned int STIM_PULSE_WIDTH = 50;
byte STIM_TRAIN_COUNT = 3;
unsigned int STIM_TRAIN_INTERVAL = 50;
unsigned int STIM_CURRENT = 5;
byte STIM_SOUNDLEVEL = 50;  // 0 - 100
byte STIM_BACKLIGHT = 50;   // 0 - 100

unsigned int STIM_MEASURED_CURRENT = 0;
unsigned int STIM_MEASURED_IMPEDANCE = 0;
unsigned int STIM_MEASURED_BATTERY = 0;
byte STIM_ERROR = 0;

byte STIM_CURRENT_SCREEN = 0;

bool EEPROM_READ = false;

/**
 * Botão VERDE: inicia estímulo
 * Botão VERMELHO: para estímulo
 * Botão AZUL: Pular entre as funções
 */
