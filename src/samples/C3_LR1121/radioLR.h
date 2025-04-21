#ifndef ________RADIO_LR1121________
#define ________RADIO_LR1121________


#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>
#include <stdint.h>
#include <settings.h>
#include <display.h>
#include <dio_rf_switch.h>



int state = RADIOLIB_ERR_NONE; // Переменная, хранящая код состояния передачи/приёма


SPIClass SPI_MODEM;
LR1121 radio = new Module(NSS_PIN, IRQ_PIN, NRST_PIN, BUSY_PIN, SPI_MODEM); //Инициализируем экземпляр радио




//************************************** Строки для формирования вывода информации ***************************************************** */

String RSSI = F("RSSI("); //Строка для печати RSSI
String dBm = F(")dBm");   //Строка для печати RSSI

String SNR = F("SNR(");   //Строка для печати SNR
String dB = F(")dB");     //Строка для печати SNR

String FR_ERR = F("F_Err(");  //Строка для печати SNR
String HERZ = F(")Hz");         //Строка для печати SNR

String DT_RATE = F("RATE(");  //Строка для печати скорости передачи данных
String BS = F(")B/s");        //Строка для печати скорости передачи данных

String TABLE_RIGHT = F("     ***************************");
String TABLE_LEFT  = F("***************************     ");
String SPACE = F(" ");

String TRANSMIT = F("TRANSMIT: ");  //Строка сообщения для передачи
String RECEIVE = F("RECEIVE: ");  //Строка сообщения для приёма



//************************************** Строки для формирования вывода информации ***************************************************** */


/**
* @brief Структура для настройки параметров радиотрансивера (по умолчанию ставим частоту 400МГц)
* 
*/
struct LORA_CONFIGURATION
{
  float frequency = 434.0;        //Частота работы передатчика (по-умолчанию 434 MHz)
  float bandwidth = 125.0;        //Полоса пропускания (по-умолчанию 125 килогерц)
  uint8_t spreadingFactor = 9;   //Коэффициент расширения (по-умолчанию 9)
  uint8_t codingRate = 7;         //Скорость кодирования (по-умолчанию 7)
  uint8_t syncWord = 0x18;        //Слово синхронизации (по-умолчанию 0х18). ВНИМАНИЕ! Значение 0x34 зарезервировано для сетей LoRaWAN и нежелательно для использования
  int8_t outputPower = 15;        //Установить выходную мощность (по-умолчанию 10 дБм) (допустимый диапазон -3 - 17 дБм) ПРИМЕЧАНИЕ: значение 20 дБм позволяет работать на большой мощности, но передача рабочий цикл НЕ ДОЛЖЕН ПРЕВЫШАТЬ 1
  uint8_t currentLimit = 80;      //Установить предел защиты по току (по-умолчанию до 80 мА) (допустимый диапазон 45 - 240 мА) ПРИМЕЧАНИЕ: установить значение 0 для отключения защиты от перегрузки по току
  int16_t preambleLength = 8;    //Установить длину преамбулы (по-умолчанию в 8 символов) (допустимый диапазон 6 - 65535)
  uint8_t gain = 0;               //Установить регулировку усилителя (по-умолчанию 1) (допустимый диапазон 1 - 6, где 1 - максимальный рост) ПРИМЕЧАНИЕ: установить значение 0, чтобы включить автоматическую регулировку усиления оставьте в 0, если вы не знаете, что вы делаете


};



//Экземпляр структуры для настройки параметров радиотрансивера 1
LORA_CONFIGURATION config_radio;



/**
 * @brief Настройка радио передатчика в соответствии с директивами,
 * которые заданы в файле "settings.h"
 * 
 */
void setRadioMode()
{
  //Задаём параметры конфигурации радиотрансивера 1
  config_radio.frequency = RADIO_1_FREQ;
  config_radio.bandwidth = RADIO_1_BANDWIDTH;
  config_radio.spreadingFactor = RADIO_1_SPREAD_FACTOR;
  config_radio.codingRate = RADIO_1_CODING_RATE;
  config_radio.syncWord = RADIO_1_SYNC_WORD;
  config_radio.outputPower = RADIO_1_OUTPUT_POWER;
  config_radio.currentLimit = RADIO_1_CURRENT_LIMIT;
  config_radio.preambleLength = RADIO_1_PREAMBLE_LENGTH;
  config_radio.gain = RADIO_1_GAIN;
}






//Флаг окончания операции отправки/получения модема №1 чтобы указать, что пакет был отправлен или получен
volatile bool operationDone = false;

// Эта функция вызывается, когда модем №1 передает или получает полный пакет
// ВАЖНО: эта функция ДОЛЖНА БЫТЬ 'пуста' типа и НЕ должна иметь никаких аргументов!
IRAM_ATTR void flag_operationDone(void) {
// мы отправили или получили пакет, установите флаг
  operationDone = true;
}






static const uint32_t rfswitch_dio_pins[] = { 
  RADIOLIB_LR11X0_DIO5, RADIOLIB_LR11X0_DIO6,
  RADIOLIB_LR11X0_DIO7, RADIOLIB_LR11X0_DIO8, RADIOLIB_NC
};





bool ICACHE_RAM_ATTR WaitOnBusy()
{
    constexpr uint32_t wtimeoutUS = 1000U;
    uint32_t startTime = 0;
    #ifdef DEBUG_PRINT
    Serial.println("");
    Serial.println("WaitOnBusy.....................  ");
    Serial.println("");
    #endif

    while (true)
    {
        if (digitalRead(BUSY_PIN) == LOW) return true;
                
        // Use this time to call micros().
        uint32_t now = micros();
        if (startTime == 0) startTime = now;
        if ((now - startTime) > wtimeoutUS) return false;
    }

        
}




void detected_CAD()
{
  int state;
  state = radio.getChannelScanResult();
  if (state == RADIOLIB_LORA_DETECTED) {
    // LoRa packet was detected
    #ifdef DEBUG_PRINT
      Serial.println(F("[LR1110] RX_1 packet detected!"));
    #endif
  } else {
    // some other error occurred
    #ifdef DEBUG_PRINT
      Serial.print(F("[LR1110] RX_1 packet detected Failed, code "));
      Serial.println(state);
    #endif
  }
}





void detectedPreamble()
{
  int state;
  state = radio.scanChannel();
  if (state == RADIOLIB_LORA_DETECTED) {
    // LoRa preamble was detected
    #ifdef DEBUG_PRINT
      Serial.println(F("Preamble detected!"));
    #endif

  } else {
    // some other error occurred
    #ifdef DEBUG_PRINT
      Serial.print(F("Preamble detected failed, code "));
      Serial.println(state);
    #endif

  }
}






void print_to_terminal_radio_state(String &RadioName, String state) __attribute__ ((weak));
void displayPrintState(int16_t x, int16_t y, String &RadioName, String state) __attribute__ ((weak));



void printRadioBeginResult(int &STATE)
{
  String radio_name = RADIO_1_NAME;
  int x=5,y=5;

  if (STATE == RADIOLIB_ERR_NONE) {
    #ifdef DEBUG_PRINT
      print_to_terminal_radio_state(radio_name, F("INIT_GOOD"));
    #endif
      displayPrintState(x, y, radio_name, F("INIT_GOOD"));
  } else {

    String str = "ERROR " + (String)STATE;
    #ifdef DEBUG_PRINT
      print_to_terminal_radio_state(radio_name, str);
    #endif
      displayPrintState(x, y, radio_name, str);
    while (true);
  }
  
}






 



/**
 * @brief Настройка радио передатчика в соответствии с директивами,
 * которые заданы в файле "settings.h"
 */
void radioBegin()
{
  pinMode(NSS_PIN, OUTPUT);
  digitalWrite(NSS_PIN, LOW);
  
  //Инициализируем радиотрансивер 1 со значениями по-умолчанию, заданными в
  //структуре LORA_CONFIGURATION
  #ifdef DEBUG_PRINT
    Serial.println(" ");
    Serial.println(F(""));
    Serial.println(" ");
    Serial.print(RADIO_1_NAME);
    #ifdef RADIO_400M
      Serial.print(RADIO_400M_FREQ);
    #endif
    #ifdef RADIO_24G
      Serial.print(RADIO_24G_FREQ);
    #endif
    
    Serial.println(F(" INIT....."));
  #endif

  #ifdef LRS_DIO_PINS
  radio1.XTAL = true;
  radio1.setRegulatorDCDC();
  //radio1.setRegulatorLDO();
  #endif

    
  //radio.setTCXO(1.6);
  //radio.setRegulatorDCDC();
  //radio.setRegulatorLDO();
  
  int state = radio.begin();
  printRadioBeginResult(state);

  radio.setRfSwitchTable(rfswitch_dio_pins, rfswitch_table_1);

  WaitOnBusy();
  
}





/**
* @brief Функция установки настроек передатчика
* 
* @param radio - экземпляр класса передатчика
* @param config - экземпляр структуры для настройки модуля
*/
void radio_setSettings(LR1121 radio, LORA_CONFIGURATION config_radio)
{
  #ifdef DEBUG_PRINT
    Serial.print(TABLE_LEFT);
    Serial.print(F("SET SETTINGTH OF RADIO "));
    Serial.println(TABLE_RIGHT);
  #endif
  
  // Устанавливаем необходимую нам частоту работы трансивера
  if (radio.setFrequency(config_radio.frequency) == RADIOLIB_ERR_INVALID_FREQUENCY) {
    #ifdef DEBUG_PRINT
      Serial.println(F("Selected frequency is invalid for this module!"));
    #endif
    while (true);
  }
  #ifdef DEBUG_PRINT
    Serial.print(F("Set frequency = "));
    Serial.println(config_radio.frequency);
  #endif


  // установить полосу пропускания до 250 кГц
  if (radio.setBandwidth(config_radio.bandwidth) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
    #ifdef DEBUG_PRINT
      Serial.println(F("Selected bandwidth is invalid for this module!"));
    #endif
    while (true);
  }
  #ifdef DEBUG_PRINT
    Serial.print(F("Set bandWidth = "));
    Serial.println(config_radio.bandwidth);
  #endif

  // коэффициент расширения 
  if (radio.setSpreadingFactor(config_radio.spreadingFactor) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
    #ifdef DEBUG_PRINT
    Serial.println(F("Selected spreading factor is invalid for this module!"));
    #endif
    while (true);
  }
  #ifdef DEBUG_PRINT
  Serial.print(F("Set spreadingFactor = "));
  Serial.println(config_radio.spreadingFactor);
  #endif

  // установить скорость кодирования
  if (radio.setCodingRate(config_radio.codingRate) == RADIOLIB_ERR_INVALID_CODING_RATE) {
    #ifdef DEBUG_PRINT
      Serial.println(F("Selected coding rate is invalid for this module!"));
    #endif
    while (true);
  }
  #ifdef DEBUG_PRINT
    Serial.print(F("Set codingRate = "));
    Serial.println(config_radio.codingRate);
  #endif

  // Устанавливаем слово синхронизации
  if (radio.setSyncWord(config_radio.syncWord) != RADIOLIB_ERR_NONE) {
    #ifdef DEBUG_PRINT
      Serial.println(F("Unable to set sync word!"));
    #endif
    while (true);
  }
  #ifdef DEBUG_PRINT
    Serial.print(F("Set syncWord = "));
    Serial.println(config_radio.syncWord);
  #endif

  // Устанавливаем выходную мощность трансивера
  if (radio.setOutputPower(config_radio.outputPower) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    #ifdef DEBUG_PRINT
      Serial.println(F("Selected output power is invalid for this module!"));
    #endif
    while (true);
  }
  #ifdef DEBUG_PRINT
    Serial.print(F("Set setOutputPower = "));
    Serial.println(config_radio.outputPower); 
  #endif

  // установить длину преамбулы (допустимый диапазон 6 - 65535)
  if (radio.setPreambleLength(config_radio.preambleLength) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
    #ifdef DEBUG_PRINT
      Serial.println(F("Selected preamble length is invalid for this module!"));
    #endif
    while (true);
  }
  #ifdef DEBUG_PRINT
    Serial.print(F("Set preambleLength = "));
    Serial.println(config_radio.preambleLength);

    

    Serial.println(F("All settings successfully changed!"));

    Serial.print(TABLE_LEFT);
    Serial.print(F("END SETTINGTH OF RADIO "));
    Serial.println(TABLE_RIGHT);
    Serial.println(SPACE);
  #endif
}







#endif;