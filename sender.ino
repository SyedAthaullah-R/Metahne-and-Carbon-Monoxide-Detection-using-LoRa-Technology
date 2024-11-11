#include <SPI.h>
#include <LoRa.h>

#define LORA_SS 5
#define LORA_RST 14
#define LORA_DIO0 26

void setup() {
  Serial.begin(115200);
  
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa Sender Ready");
}

void loop() {
  int methaneLevel = analogRead(34);  // MQ-4 sensor connected to pin 34
  int carbonMonoxideLevel = analogRead(35);  // MQ-7 sensor connected to pin 35

  Serial.print("Sending Methane Level: ");
  Serial.println(methaneLevel);
  Serial.print("Sending CO Level: ");
  Serial.println(carbonMonoxideLevel);

  LoRa.beginPacket();
  LoRa.print("Methane:");
  LoRa.print(methaneLevel);
  LoRa.print(",CO:");
  LoRa.print(carbonMonoxideLevel);
  LoRa.endPacket();

  delay(1000);  // Send every second
}
