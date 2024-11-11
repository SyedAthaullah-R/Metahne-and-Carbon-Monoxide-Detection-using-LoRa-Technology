#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LORA_SS 5
#define LORA_RST 14
#define LORA_DIO0 26
#define LED_PIN 2        // LED connected to GPIO 2
#define BUZZER_PIN 4     // Buzzer connected to GPIO 4
#define METHANE_THRESHOLD 600  // Threshold for methane gas level
#define CO_THRESHOLD 1400       // Threshold for carbon monoxide level

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to your LCD's I2C address

void setup() {
  Serial.begin(115200);
  
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LoRa Receiver");

  Serial.println("LoRa Receiver Ready");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    int methaneLevel = 0;
    int carbonMonoxideLevel = 0;
    
    if (sscanf(receivedData.c_str(), "Methane:%d,CO:%d", &methaneLevel, &carbonMonoxideLevel) == 2) {
      Serial.print("Packets Received. Methane Level: ");
      Serial.println(methaneLevel);
      Serial.print("Carbon Monoxide Level: ");
      Serial.println(carbonMonoxideLevel);
      
      int rssi = LoRa.packetRssi();
      Serial.print("RSSI: ");
      Serial.println(rssi);

      // Display levels on the LCD and control the LED/Buzzer
      if (methaneLevel > METHANE_THRESHOLD || carbonMonoxideLevel > CO_THRESHOLD) {
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);

        lcd.clear();
        lcd.setCursor(0, 0);
        if (methaneLevel > METHANE_THRESHOLD) {
          lcd.print("Methane Level: ");
           lcd.setCursor(0, 1);
          lcd.print(methaneLevel);
        }
        lcd.setCursor(0, 1);
        if (carbonMonoxideLevel > CO_THRESHOLD) {
          lcd.print("CO Level: ");
          lcd.print(carbonMonoxideLevel);
        }
      } else {
        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Safe Levels");
      }
    } else {
      Serial.println("Error: Data format not recognized");
    }
  }
}
