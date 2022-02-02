// Скетч для адресной светодиодной ленты

//! При самом первом запуске этого скетча на ардуино, то есть еще до его загрузки в плату, необходимо проинициализироовать
//! EEPROM память нолями. Для этого есть скетч с названием init

// Подключаем библиотеку для ленты
#define STRIP_PIN 4     // пин ленты
#define NUMLEDS 240      // кол-во светодиодов
#define COLOR_DEBTH 1   // байт на цвет

#include <microLED.h>
#include "global_variables.h"
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip;

#define NUM_MODES 2
#define NUM_COLORS 8
#include "leds_mods.h"
void(*modes_arr[])() = {off, static_color};

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
bool flag_dynamic; // Флаг, информирующий о том, что включен динамический режим и что ардуино спать нельзя
volatile unsigned long long last_time = 0;  // Инициализируем переменную для сэйва времени

void setup()
{
  Serial.begin(9600);

  strip.clear();  // Инициализируем ленту
  strip.setBrightness(255);
  strip.show();
  delayMicroseconds(40);

#define BRIGHTNESS_PIN A0
  pinMode(BRIGHTNESS_PIN, INPUT);  // Инициализируем аналоговый порт для считывания яркости потенциометром

#define LED_PIN 13
  pinMode(LED_PIN, OUTPUT);  // Инициализируем цифровой порт для индикации режима работы МК встроенным светодиодом

#define RESET_PIN 9
  pinMode(RESET_PIN, INPUT_PULLUP);  // Инициализируем цифровой порт для сброса режимов в дефолт

#define ON_PIN 7
#define ON_SLEEP_PIN 8
  pinMode(ON_PIN, INPUT_PULLUP);  // Инициализируем цифровые порты для выбора режима работы МК (переключение тумблером) -> в основном цикле
  pinMode(ON_SLEEP_PIN, INPUT_PULLUP);
  flag_on = false;  // Инициализируем флаг, информирующий о режиме ON (в режиме ON он в тру автоматом, а вот для других надо задать)

#define MODE_PIN 2
#define COLOR_PIN 3
  pinMode(MODE_PIN, INPUT_PULLUP);  // Инициализируем цифровые порты для прерываний (считывания режима или цвета)
  pinMode(COLOR_PIN, INPUT_PULLUP);

  last_time = millis();  // Инициализируем переменную для сэйва времени

  power.setSleepMode(POWERDOWN_SLEEP);  // Инициализируем режим энергосбережения

  start_index_eeprom_memory = find_start_eeprom();  // Ищем область в памяти EEPROM с последним сохранённым режимом
  mode = new uint8_t;  // Выделяем память указателям
  color = new uint8_t;
  load_info_eeprom(start_index_eeprom_memory);  // Далее инициализируем указатели последним сохранённым режимом

  Serial.print("Start_index_eeprom_memory === ");  // Инфа о текущем индексе в EEPROM
  Serial.println(start_index_eeprom_memory);
}

void loop()
{
  (*modes_arr[*mode])();

  if (!digitalRead(ON_PIN))
  {
    if (!flag_on)
    {
      flag_on = true;
      Serial.print("\nMODE: ON");
      delay(100);
      attachInterrupt(0, change_mode, FALLING);
      attachInterrupt(1, change_color, FALLING);
    }
    //strip.setBrightness(constrain(map(analogRead(BRIGHTNESS_PIN), 0, 1023, 0, 255), 0, 255));
    if (*mode != EEPROM[start_index_eeprom_memory] || *color != EEPROM[start_index_eeprom_memory + 1])  // Мигающее предупреждение светодиода если есть изменения, иначе просто горит (мой пукан)
    {
      delay(50);
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
    else
    {
      digitalWrite(LED_PIN, HIGH);
    }
    if (!digitalRead(RESET_PIN))
    {
      load_info_eeprom(start_index_eeprom_memory);
    }
  }
  else
  {
    if (flag_on)
    {
      flag_on = false;
      Serial.print("\nMODE: SLEEP");
      detachInterrupt(0);
      detachInterrupt(1);
    }
    if (digitalRead(ON_SLEEP_PIN))
    {
      Serial.print("_OFF");
      (*modes_arr[0])();  // Выключение ленты и заодно сброс флага, чтобы поспать со 100% вероятностью
      if (*mode != EEPROM[start_index_eeprom_memory] || *color != EEPROM[start_index_eeprom_memory + 1])
      {
        save_info_eeprom(start_index_eeprom_memory);
      }
    }
    if (!flag_dynamic)
    {
      attachInterrupt(0, none, FALLING); // Ставим прерывания чтобы просыпаться по кнопкам
      attachInterrupt(1, none, FALLING);
      power.hardwareDisable(PWR_ALL);
      power.sleep(SLEEP_FOREVER);
      power.hardwareEnable(PWR_ALL);
      detachInterrupt(0); // Затираем прерывания после сна
      detachInterrupt(1);
    }
  }
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

void load_info_eeprom(short& index)
{
  if (index + 2 <= eeprom_size)  // Инициализируем указатели
  {
    *mode = EEPROM.read(index); // Если переменная больше байта, то юзать метод get, и ниже в арифметике оператор sizeof(T)
    *color = EEPROM.read(index + 1);
    *mode = (*mode >= NUM_MODES) ? 0 : *mode;
    *color = (*color >= NUM_COLORS) ? 0 : *color;
  }
  else
  {
    *mode = 0;
    *color = 0;
  }

  return;
}

void save_info_eeprom(short& index)
{
  index += 2; // Начальный индекс, куда надо писать данные
  index = ((index + 1) >= eeprom_size) ? 0 : index; // Проверка на границы и сброс при выходе за границы
  EEPROM.update(index, *mode);
  EEPROM.update(index + 1, *color);

  return;
}

void change_mode()
{
  if (millis() - last_time > 350)
  {
    *mode = (++(*mode) >= NUM_MODES) ? 0 : *mode;
    last_time = millis();
  }

  Serial.print("mode === ");
  Serial.println(*mode);

  return;
}

void change_color()
{
  if (millis() - last_time > 200)
  {
    *color = (++(*color) >= NUM_COLORS) ? 0 : *color;
    last_time = millis();
  }

  Serial.print("color === ");
  Serial.println(*color);

  return;
}

void none()
{
  return;
}
