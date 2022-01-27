#define STRIP_PIN 4     // пин ленты
#define NUMLEDS 240      // кол-во светодиодов
#define COLOR_DEBTH 1   // байт на цвет

#include <microLED.h>
#include "global_variables.h"
#include "leds_mods.h"
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip;

#include <GyverPower.h>
extern GyverPower power;

#include <EEPROM.h>
#define eeprom_size EEPROM.length()
short start_index_eeprom_memory = 0;

typedef struct _config
{
short mode;
short color;
short brightness;
} START_INFO;

void setup()
{
  strip.clear();
  strip.show();
  delayMicroseconds(40);

  power.hardwareDisable(PWR_ALL);
  power.setSleepMode(POWERDOWN_SLEEP);
  power.setSleepResolution(SLEEP_FOREVER);

  start_index_eeprom_memory = find_start_eeprom();
  START_INFO si;
  (start_index_eeprom_memory+sizeof(START_INFO)<=eeprom_size)?EEPROM.get(start_index_eeprom_memory,si):EEPROM.get(0,si);
}

void loop()
{

}

short find_start_eeprom()
{
  short index(0);
  for (; index < eeprom_size; ++index)
  {
    if (EEPROM[index] != 0)
      break;
  }
  return index;
}
