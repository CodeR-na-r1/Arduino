// Скетч для адресной светодиодной ленты

// Подключаем библиотеку для ленты
#define STRIP_PIN 4     // пин ленты
#define NUMLEDS 240      // кол-во светодиодов
#define COLOR_DEBTH 1   // байт на цвет

#include <microLED.h>
#include "global_variables.h"
#include "leds_mods.h"
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip;

// Подключаем библиотеку для возможности энергосбережения
#include <GyverPower.h>
extern GyverPower power;

// Подключаем библиотеку для использования EEPROM памяти
#include <EEPROM.h>
#define eeprom_size EEPROM.length()
short start_index_eeprom_memory = 0;

// Структура с данными, в них инфа о последнем режиме
typedef struct _config
{
  short mode;
  short color;
} START_INFO;

void setup()
{
  Serial.begin(9600);

  strip.clear();  // Инициализируем ленту
  strip.show();
  delayMicroseconds(40);

  pinMode(A0, INPUT);  // Инициализируем аналоговый порт для считывания яркости потенциометром

  pinMode(13, INPUT);  // Инициализируем цифровой порт для индикации режима работы МК встроенным светодиодом

  pinMode(7, INPUT);  // Инициализируем цифровые порты для выбора режима работы МК (переключение тумблером) -> в основном цикле
  pinMode(8, INPUT);

  pinMode(2, INPUT_PULLUP);  // Инициализируем цифровые порты для прерываний (считывания режима или цвета)
  pinMode(3, INPUT_PULLUP);

  volatile unsigned long long last_time = 0;  // Инициализируем переменную для сэйва времени

  power.setSleepMode(POWERDOWN_SLEEP);  // Инициализируем режим энергосбережения

  start_index_eeprom_memory = find_start_eeprom();  // Далее инициализируем структуру с последним режимом
  START_INFO si;
  (start_index_eeprom_memory + sizeof(START_INFO) <= eeprom_size) ? EEPROM.get(start_index_eeprom_memory, si) : EEPROM.get(0, si);
  Serial.print("Start_index_eeprom_memory === ");
  Serial.println(start_index_eeprom_memory);
}

void loop()
{
  if ()
  {
    attachInterrupt(0, fucn, FALLING);
    attachInterrupt(1, fucn, FALLING);
  }
  else if ()
  {
    power.hardwareDisable(PWR_ALL);
    power.sleep(SLEEP_FOREVER);
    power.hardwareEnable(PWR_ALL);
  }
  else
  {
    power.hardwareDisable(PWR_ALL);
    power.sleep(SLEEP_FOREVER);
    power.hardwareEnable(PWR_ALL);
  }
  /*
    analogRead(0)) //+ время (чо-то бешенные значения)
    {
      value = analogRead(0);
      Serial.print('\n');
      Serial.print(value);
    }
    }
  */
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

/*
  void fucn()
  {
  if (millis() - last_time >= 200)
  {
    last_time = millis();
    ++value;
  }
  }*/
