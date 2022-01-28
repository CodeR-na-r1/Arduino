// Скетч для адресной светодиодной ленты

#define NUM_MODES 1
#define NUM_COLORS 1

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

// Указатели для хранения и чтения инфы о режиме ленты в текущем и других юнитах
uint8_t* mode;
uint8_t* color;
bool flag_on; // Флаг, информирующий о режиме ON (включена настройка режимов, цветов, яркости)
volatile unsigned long long last_time = 0;  // Инициализируем переменную для сэйва времени

void setup()
{
  Serial.begin(9600);

  strip.clear();  // Инициализируем ленту
  strip.show();
  delayMicroseconds(40);

#define BRIGHTNESS_PIN A0
  pinMode(BRIGHTNESS_PIN, INPUT);  // Инициализируем аналоговый порт для считывания яркости потенциометром

#define LED_PIN 13
  pinMode(LED_PIN, INPUT);  // Инициализируем цифровой порт для индикации режима работы МК встроенным светодиодом

#define ON_PIN 7
#define ON_SLEEP_PIN 8
  pinMode(ON_PIN, INPUT);  // Инициализируем цифровые порты для выбора режима работы МК (переключение тумблером) -> в основном цикле
  pinMode(ON_SLEEP_PIN, INPUT);
  flag_on = false;  // Инициализируем флаг, информирующий о режиме ON (в режиме ON он в тру автоматом, а вот для других надо задать)

#define MODE_PIN 2
#define COLOR_PIN 3
  pinMode(MODE_PIN, INPUT_PULLUP);  // Инициализируем цифровые порты для прерываний (считывания режима или цвета)
  pinMode(COLOR_PIN, INPUT_PULLUP);

  last_time = millis();  // Инициализируем переменную для сэйва времени

  power.setSleepMode(POWERDOWN_SLEEP);  // Инициализируем режим энергосбережения

  start_index_eeprom_memory = find_start_eeprom();  // Далее инициализируем структуру с последним режимом
  mode = new uint8_t;  // Инициализируем указатели
  color = new uint8_t;
  if (start_index_eeprom_memory + 2 <= eeprom_size)
  {
    *mode = EEPROM.read(start_index_eeprom_memory); // Если переменная больше байта, то юзать метод get, и ниже в арифметике оператор sizeof(T)
    *color = EEPROM.read(start_index_eeprom_memory + 1);
  }
  else
  {
    *mode = 0;
    *color = 0;
  }

  Serial.print("Start_index_eeprom_memory === ");  // Инфа о текущем индексе в EEPROM
  Serial.println(start_index_eeprom_memory);
}

void loop()
{
  if (digitalRead(ON_PIN))
  {
    if (!flag_on)
    {
      flag_on = true;
      attachInterrupt(0, change_mode, FALLING);
      attachInterrupt(1, change_color, FALLING);
    }

  }
  else if (digitalRead(ON_SLEEP_PIN))
  {
    power.hardwareDisable(PWR_ALL);
    power.sleep(SLEEP_FOREVER);
    power.hardwareEnable(PWR_ALL);
  }
  else
  {
    if (flag_on)
    {
      flag_on = false;
      detachInterrupt(0);
      detachInterrupt(1);
    }
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

void change_mode()
{
  if (millis() - last_time > 200)
  {
    *mode = (++(*mode) >= NUM_MODES) ? 0 : *mode;
    last_time = millis();
  }

  return;
}

void change_color()
{
  if (millis() - last_time > 200)
  {
    *color = (++(*color) >= NUM_COLORS) ? 0 : *color;
    last_time = millis();
  }

  return;
}
