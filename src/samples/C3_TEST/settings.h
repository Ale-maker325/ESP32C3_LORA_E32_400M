#pragma once
/*   Этот файл предназначен для проверки наличия некоторых флагов по умолчанию */
/*   Только код PreCompiler должен быть установлен в этом файле                */
#include <Arduino.h>

/**
 * @brief Выбор режима работы радио при компиляции. Раскомментировать то, что необходимо
 * 
 */

 #define DEBUG_PRINT
 #define TRANSMITTER             //раскомментировать, если модуль будет использоваться как простой передатчик
 //#define RECEIVER                //раскомментировать, если модуль будет использоваться как простой приёмник



//**************************************************** Параметры радио для компиляции ************************************************

//Задаём параметры конфигурации радиотрансивера 
#define RADIO_FREQ 441
#define RADIO_BANDWIDTH 125
#define RADIO_SPREAD_FACTOR 9
#define RADIO_CODING_RATE 7
#define RADIO_SYNC_WORD 0x12
#define RADIO_OUTPUT_POWER 15
#define RADIO_CURRENT_LIMIT 100
#define RADIO_PREAMBLE_LENGTH 8
#define RADIO_GAIN 0

//**************************************************** Параметры радио для компиляции ************************************************


#ifdef TRANSMITTER 
String RADIO_NAME = F("TX");
#endif

#ifdef RECEIVER
String RADIO_NAME = F("RX");
#endif




/**
 * @brief Пины I2C
 * 
 */
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 10


// _______________________ Пин пользовательского светодиода _______________________
#define LED_PIN 8


// _______________________ Пины модема _______________________
#define NSS_PIN 7
#define IRQ_PIN 1
#define NRST_PIN 2
#define BUSY_PIN 3

#define MOSI_RADIO 6
#define MISO_RADIO 5
#define SCK_RADIO 4



