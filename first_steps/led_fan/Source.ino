#define STRIP_PIN 5     // пин ленты
#define NUMLEDS 12      // кол-во светодиодов
#define COLOR_DEBTH 1   // кол-во байт на цвет

#define NUMCOLORS 15   // кол-во цветов в массиве
#define NUMMODES 4   // кол-во режимов подсветки в массиве (функции подсветок)

#include <microLED.h>
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip;
#include "leds_modes.h"
#include "global_var.h"

void (*modes_arr[])() = {led_off, static_color, random_leds, led_first};  // Массив с функциями подсветок

long last_time = millis();  // Переменная таймера

static uint8_t now_mode;
static uint8_t now_color;
//volatile bool flag_interrupt;
//volatile uint16_t count_interrupt;
uint8_t* NOW_COLOR;
void setup()
{
  now_mode = 0;  // Переменная-счетчик текущего режима подсветки
  now_color = 0;  // Переменная-счетчик текущего цвета подсветки
  //flag_interrupt = false;
  //count_interrupt = 0;
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
  //Serial.println(count_interrupt);
  
  (*modes_arr[now_mode])();
  //count_interrupt=0;
  delay(1);
  
  // if (millis() - last_time >= 5000) // Сработывает раз в ~5 секунд
  // {
  //   last_time = millis();
  //   Serial.println("5 seconds прошло");
  // }
}

void change_mode()
{
  if (millis() - last_time >= 400)
  {
    now_mode = ++now_mode >= NUMMODES ? 0 : now_mode;  //  Увелечение текущего режима подсветки или сброс при последней подсветке
    last_time = millis();
  }
  
  /*
  if (!flag_interrupt && !count_interrupt)
  {
    flag_interrupt = true;
    //last_time = millis();
  }
  if (count_interrupt <= 20 && flag_interrupt)
  {
    ++count_interrupt;
  }
  if (count_interrupt >= 20 && flag_interrupt)
  {
    flag_interrupt = false;
    now_mode = ++now_mode >= NUMMODES ? 0 : now_mode;  //  Увелечение текущего режима подсветки или сброс при последней подсветке
  }
  Serial.println(count_interrupt);
  */
}

void change_color()
{
  if (millis() - last_time >= 400)
  {
    now_color = ++now_color >= NUMCOLORS ? 0 : now_color;  //  Увелечение текущего цвета подсветки или сброс при последнем цвете
    last_time = millis();
  }
}
