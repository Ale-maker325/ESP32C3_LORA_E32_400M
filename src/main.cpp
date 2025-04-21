#include <Arduino.h>
#include <settings.h>
#include <display.h>



uint64_t count = 0;           //Счётчик для сохранения количества отправленных/полученных пакетов


void setup() {
    #ifdef DEBUG_PRINT
        Serial.begin(115200);
        Serial.println(" ");
        Serial.println(F("*************************  Serial ready 115200 *******************************"));
        Serial.println(" ");
    #endif

    // SPI_MODEM.begin(SCK_RADIO, MISO_RADIO, MOSI_RADIO, NSS_PIN);
    SPI_MODEM.begin(SCK_RADIO, MISO_RADIO, MOSI_RADIO, NSS_PIN);
    display_wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, 100000);
    pinMode(LED_PIN, OUTPUT);

    #ifdef DEBUG_PRINT
        Serial.println(" ");
        Serial.print("Chip Model = ");
        Serial.println(ESP.getChipModel());
        Serial.print("Cpu Freq = ");
        Serial.println(ESP.getCpuFreqMHz());
        Serial.print("ChipRevision = ");
        Serial.println(ESP.getChipRevision());
        Serial.print("SDK Version = ");
        Serial.println(ESP.getSdkVersion());
        Serial.println(" ");
    #endif

    #ifdef DEBUG_PRINT
        Serial.print(TABLE_LEFT);
        Serial.print(F("DISPLAY INIT"));
        Serial.println(TABLE_RIGHT);
        Serial.println(SPACE);
    #endif

    //инициализируем дисплей
    displayInit();

    radioBegin();
    
    setRadioMode();

    //Устанавливаем функцию, которая будет вызываться при отправке пакета данных
    radio.setPacketSentAction(flag_operationDone);

    //Включаем светодиод на плате
    digitalWrite(LED_PIN, LOW);

    #ifdef DEBUG_PRINT
        Serial.println(SPACE);
    #endif

    //Устанавливаем наши значения, определённые ранее в структуре config_radio1
    radio_setSettings(radio, config_radio);

    #ifdef TRANSMITTER   //Если определена работа модуля как передатчика
        

        //Начинаем передачу пакетов
        Serial.println(F("Sending first packet ... "));

        String str = F("START!");
        
        transmit_and_print_data(str);
        

    #endif

}
















void loop() {
    delay(500);
    digitalWrite(LED_PIN, HIGH);
    
    #ifdef TRANSMITTER   //Если определен как передатчик проверяем, была ли предыдущая передача успешной
        
        // if(operationDone_1) {
            #ifdef DEBUG_PRINT
                Serial.println("..................................................");
            #endif
            //Сбрасываем сработавший флаг прерывания
            operationDone = false;

            //готовим строку для отправки
            String str = "#" + String(count++);
            transmit_and_print_data(str);
            digitalWrite(LED_PIN, LOW);            
            
        // }

    #endif

   
}

