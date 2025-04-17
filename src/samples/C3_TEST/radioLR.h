#ifndef ________RADIO_RADIO________
#define ________RADIO_RADIO________


#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>
#include <stdint.h>
#include <settings.h>



int state = RADIOLIB_ERR_NONE; // Переменная, хранящая код состояния передачи/приёма

SPIClass SPI_MODEM;
SX1278 radio = new Module(NSS_PIN, IRQ_PIN, NRST_PIN, BUSY_PIN, SPI_MODEM); //Инициализируем экземпляр радио
// SX1278 radio = new Module(NSS_PIN, IRQ_PIN, NRST_PIN, BUSY_PIN); //Инициализируем экземпляр радио


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
* @brief Структура для настройки параметров радиотрансивера
* 
*/
struct LORA_CONFIGURATION
{
  float frequency = 434.0;        //Частота работы передатчика (по-умолчанию 434 MHz)
  float bandwidth = 125.0;        //Полоса пропускания (по-умолчанию 125 килогерц)
  uint8_t spreadingFactor = 9;   //Коэффициент расширения (по-умолчанию 9)
  uint8_t codingRate = 7;         //Скорость кодирования (по-умолчанию 7)
  uint8_t syncWord = 0x12;        //Слово синхронизации (по-умолчанию 0х18). ВНИМАНИЕ! Значение 0x34 зарезервировано для сетей LoRaWAN и нежелательно для использования
  int8_t outputPower = 10;        //Установить выходную мощность (по-умолчанию 10 дБм) (допустимый диапазон -3 - 17 дБм) ПРИМЕЧАНИЕ: значение 20 дБм позволяет работать на большой мощности, но передача рабочий цикл НЕ ДОЛЖЕН ПРЕВЫШАТЬ 1
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
  config_radio.frequency = RADIO_FREQ;
  config_radio.bandwidth = RADIO_BANDWIDTH;
  config_radio.spreadingFactor = RADIO_SPREAD_FACTOR;
  config_radio.codingRate = RADIO_CODING_RATE;
  config_radio.syncWord = RADIO_SYNC_WORD;
  config_radio.outputPower = RADIO_OUTPUT_POWER;
  config_radio.currentLimit = RADIO_CURRENT_LIMIT;
  config_radio.preambleLength = RADIO_PREAMBLE_LENGTH;
  config_radio.gain = RADIO_GAIN;

}






//Флаг окончания операции отправки/получения модема №1 чтобы указать, что пакет был отправлен или получен
volatile bool operationDone = false;

// Эта функция вызывается, когда модем №1 передает или получает полный пакет
// ВАЖНО: эта функция ДОЛЖНА БЫТЬ 'пуста' типа и НЕ должна иметь никаких аргументов!
IRAM_ATTR void flag_operationDone(void) {
// мы отправили или получили пакет, установите флаг
  operationDone = true;
  
}







// typedef uint8_t Radio_Number;
// enum
// {
//     Radio_NONE = 0b00000000,     // Bit mask for no radio
//     Radio      = 0b00000001,     // Bit mask for radio 
    
// };
 



//------------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------------




// bool ICACHE_RAM_ATTR WaitOnBusy(Radio_Number radioNumber)
// {
//     constexpr uint32_t wtimeoutUS = 1000U;
//     uint32_t startTime = 0;
//     #ifdef DEBUG_PRINT
//       Serial.println("");
//       Serial.println("WaitOnBusy.....................");
//       Serial.println("");
//     #endif

//     while (true)
//     {
//         if (radioNumber == Radio)
//         {
//             if (digitalRead(BUSY_PIN) == LOW) return true;
//         }
        
//         // Use this time to call micros().
//         uint32_t now = micros();
//         if (startTime == 0) startTime = now;
//         if ((now - startTime) > wtimeoutUS) return false;
//     }

        
// }














void print_to_terminal_radio_state(String &RadioName, String state) __attribute__ ((weak));
void displayPrintState(int16_t x, int16_t y, String &RadioName, String state) __attribute__ ((weak));



void printRadioBeginResult(int &STATE)
{
  String radio_name = RADIO_NAME;
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
  delay(500);
  
}






// void ICACHE_RAM_ATTR selectRadio(Radio_Number radio_number)
// {
//   switch (radio_number)
//   {
//   //Если выбираем радио 1 
//   case Radio:
//     digitalWrite(NSS_PIN, LOW);
//     //Если при этом есть и радио 2, то с него снимаем выделение
//     break;
    
//   default:
//     break;
//   }
// }






  



/**
 * @brief Настройка радио передатчика в соответствии с директивами,
 * которые заданы в файле "settings.h"
 */
void radioBegin()
{
  #ifdef DEBUG_PRINT
    Serial.println(" ");
    Serial.println(F(""));
    Serial.println(" ");
    Serial.print(RADIO_NAME);
    Serial.println(F(" INIT....."));
  #endif

  //Задаём параметры конфигурации радиотрансивера 1
  config_radio.frequency = 441;
  config_radio.bandwidth = 125;
  config_radio.spreadingFactor = 9;
  config_radio.codingRate = 7;
  config_radio.syncWord = 0x12;
  config_radio.outputPower = 10;
  config_radio.currentLimit = 100;
  config_radio.preambleLength = 8;
  config_radio.gain = 0;

  int state = radio.begin();
  
  printRadioBeginResult(state);

  
}













/**
* @brief Функция установки настроек передатчика
* 
* @param radio - экземпляр класса передатчика
* @param config - экземпляр структуры для настройки модуля
*/
void radio_setSettings(SX1278 radio, LORA_CONFIGURATION config_radio)
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