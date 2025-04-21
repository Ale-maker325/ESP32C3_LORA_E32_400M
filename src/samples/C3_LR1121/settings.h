#pragma once
#include <Arduino.h>

#define DEBUG_PRINT

/**
 * @brief Следует раскомментировать одно из определений, а другое закомментировать.
 *  - RADIO_1 - раскомментировать если используется одно радио
 *  
 */
#define RADIO_1             //Раскомментировать если будет использован только один модем LR1121



/**
 * @brief Выбор режима работы радио при компиляции. Раскомментировать то, что необходимо
 * 
 */

#define TRANSMITTER             //раскомментировать, если модуль будет использоваться как простой передатчик
//#define RECEIVER                //раскомментировать, если модуль будет использоваться как простой приёмник


#define RADIO_400M
//#define RADIO_24G
 

//**************************************************** Параметры радио для компиляции ************************************************
#ifdef RADIO_400M
    //Задаём параметры конфигурации радиотрансивера на частоте 400M
    #define RADIO_1_FREQ 441
    #define RADIO_1_BANDWIDTH 125
    #define RADIO_1_SPREAD_FACTOR 9
    #define RADIO_1_CODING_RATE 7
    #define RADIO_1_SYNC_WORD RADIOLIB_LR11X0_LORA_SYNC_WORD_PRIVATE
    #define RADIO_1_OUTPUT_POWER 10
    #define RADIO_1_CURRENT_LIMIT 200
    #define RADIO_1_PREAMBLE_LENGTH 8
    #define RADIO_1_GAIN 0
#endif

#ifdef RADIO_24G
    //Задаём параметры конфигурации радиотрансивера на частоте 2,4G
    #define RADIO_1_FREQ 2441
    #define RADIO_1_BANDWIDTH 125
    #define RADIO_1_SPREAD_FACTOR 9
    #define RADIO_1_CODING_RATE 7
    #define RADIO_1_SYNC_WORD RADIOLIB_LR11X0_LORA_SYNC_WORD_PRIVATE
    #define RADIO_1_OUTPUT_POWER 5
    #define RADIO_1_CURRENT_LIMIT 200
    #define RADIO_1_PREAMBLE_LENGTH 8
    #define RADIO_1_GAIN 0
#endif

//**************************************************** Параметры радио для компиляции ************************************************

#ifdef TRANSMITTER
    String RADIO_1_NAME = F("TX");
    String RADIO_24G_FREQ = F("_2.4GHz");
    String RADIO_400M_FREQ = F("_400MHz");
#endif

#ifdef RECEIVER
    String RADIO_1_NAME = F("RX");
    String RADIO_24G_FREQ = F("_2.4G");
    String RADIO_400M_FREQ = F("_400M");
#endif



//*********************************************************************************************************************************** */




// /**
//  * @brief Пины I2C
//  * 
//  */
// #define I2C_SDA_PIN 0
// #define I2C_SCL_PIN 10


// // _______________________ Пин пользовательского светодиода _______________________
// #define LED_PIN 8


// // _______________________ Пины модема _______________________
// #define NSS_PIN 6
// #define IRQ_PIN 1
// #define NRST_PIN 2
// #define BUSY_PIN 3

// #define MOSI_RADIO 4
// #define MISO_RADIO 5
// #define SCK_RADIO 7











/**_______________________________________________________________________________________________________________________________________ */
/**_______________________________________________________________________________________________________________________________________ */
/**_______________________________________________________________________________________________________________________________________ */
/**На основании распиновки модуля HELTEC C3 HT-CT62 единственно что пин NSS не 8, а 1, так как на 8 пине служебный светодиод */

/**
 * @brief Пины I2C
 * 
 */
#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 0


// _______________________ Пин пользовательского светодиода _______________________
#define LED_PIN 8


// _______________________ Пины модема _______________________
#define NSS_PIN 1
#define IRQ_PIN 3
#define NRST_PIN 5
#define BUSY_PIN 4

#define MOSI_RADIO 7
#define MISO_RADIO 6
#define SCK_RADIO 10

/**_______________________________________________________________________________________________________________________________________ */
/**_______________________________________________________________________________________________________________________________________ */
/**_______________________________________________________________________________________________________________________________________ */












// {
//     "serial_rx": 20,
//     "serial_tx": 21,

//     "radio_miso": 5,
//     "radio_mosi": 4,
//     "radio_sck": 6,

//     "radio_busy": 3,
//     "radio_dio1": 1,
//     "radio_nss": 7,
//     "radio_rst": 2,

//     "power_min": 0,
//     "power_high": 3,
//     "power_max": 3,
//     "power_default": 0,
//     "power_control": 0,
//     "power_values": [12,16,19,22],
//     "power_values_dual": [-12,-9,-6,-2],

//     "led": 8,

//     "radio_dcdc": true,

//     "button": 9
// }