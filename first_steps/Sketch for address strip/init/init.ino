#include <EEPROM.h>
#define eeprom_size EEPROM.length()

#define LED_PIN 13

bool flag_end = false;
short count(0);

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  for (int i(0); i < eeprom_size; ++i)
  {
    Serial.println(EEPROM[i]);
  }
}

void loop()
{
  while (flag_end) {}
  Serial.println("Start EEPROM initialization!");
  for (int i(0); i < eeprom_size; ++i)
  {
    if (EEPROM[i].update(0))
      ++count;
  }
  Serial.print("\n---------------\nEEPROM size === ");
  Serial.println(eeprom_size);
  Serial.print("\nErase Cells === ");
  Serial.println(count);
  Serial.println("\nEnd EEPROM initialization!");
  digitalWrite(LED_PIN, HIGH);
  flag_end = true;
}
