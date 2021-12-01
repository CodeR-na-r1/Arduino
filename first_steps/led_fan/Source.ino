#define STRIP_PIN 5     // пин ленты
#define NUMLEDS 12      // кол-во светодиодов
#define COLOR_DEBTH 1   // кол-во байт на цвет

#define NUMCOLORS 15   // кол-во цветов в массиве
#define NUMMODES 6   // кол-во режимов подсветки в массиве (функции подсветок)

#include <microLED.h>
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip;
#include "leds_modes.h"
#include "global_var.h"

void (*modes_arr[])() = {led_off, static_color, random_leds, animation_wheel, loading, led_first};  // Массив с функциями подсветок

long last_time = millis();  // Переменная таймера

static uint8_t now_mode;
static uint8_t now_color;
uint8_t* NOW_COLOR;

void setup()
{
  now_mode = 0;  // Переменная-счетчик текущего режима подсветки
  now_color = 0;  // Переменная-счетчик текущего цвета подсветки
  NOW_COLOR = &now_color;

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, change_mode, FALLING);  // Прерывание на кнопку, смена режима подсветки по кнопке // D3
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, change_color, FALLING);  // Прерывание на кнопку, смена цвета подсветки по кнопке  // D2
  
  strip.setBrightness(255);  // Яркость
  strip.clear();  // Инициализация черным
  strip.show(); // вывод изменений на ленту
  delay(1);
  
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  Serial.println("new_loop!");
  Serial.println(now_mode);
  Serial.println(now_color);
  
  (*modes_arr[now_mode])();
  //delay(1000);

  if (Serial.available() > 0) // Смена режима и цвета через порт (два числа через пробел)
  {
    if ((Serial.read() - '0') == 1) // Если первое == 1 ,то смена режима
    {
      now_mode = Serial.parseInt(); // Второе число == номер режима
    }
    else // Иначе смена цвета
    {
      now_color = Serial.parseInt(); // Второе число == номер подсветки
    }
  }
}

void change_mode()
{
  if (millis() - last_time >= 200)
  {
    now_mode = ++now_mode >= NUMMODES ? 0 : now_mode;  //  Увелечение текущего режима подсветки или сброс при последней подсветке
    last_time = millis();
  }
}

void change_color()
{
  if (millis() - last_time >= 200)
  {
    now_color = ++now_color >= NUMCOLORS ? 0 : now_color;  //  Увелечение текущего цвета подсветки или сброс при последнем цвете
    last_time = millis();
  }
}
