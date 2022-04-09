// Скетч для адресной светодиодной ленты

#pragma once

#include "config.h"  // Файл с конфигом
#include "headers.h"  // Файл с заголовками
#include "some_functions.h"  // Файл с заголовками

/* Инициализации */

LiquidCrystal_I2C display(0x27, 16, 2); // Дисплей

microLED<NUM_LEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip; // Адресная лента в кулерах
void (*modes_arr[])() = {led_off, static_color, loading_2, two_led};  // Массив с функциями подсветок

uint8_t mode = 3; // Счётчик текущего режима
uint8_t color = 0; //  Счётчик текущего цвета
uint8_t mode_delay = 18; // 

volatile int counter_rpm(0);  // 
int now_rpm(0);
unsigned long long int time_rpm = millis();

void setup()
{
  /* Настройки объектов */

  Serial.begin(9600);  // 
  Serial.println("Setup stage");
  
  Wire.begin();   // 
  display.begin();
  display.backlight();
  display.noBlink();
  display.noCursor();

  strip.setBrightness(255);  // 
  strip.clear();
  strip.show();

  randomSeed(analogRead(0));  // 

  pinMode(2, INPUT);
  attachInterrupt(0, rpm_counter, RISING);  // Прерывание sense fan, счётчик оборотов  // D2

  pinMode(PWM_CONTROL_PIN, INPUT);  // 
  
  // Настройка ШИМ сигнала на пины D9 и D10 - 31.4 кГц (подача питания на вентиляторы)
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000001;  // x1 phase correct
  pinMode(PWM_PIN, OUTPUT);
  
  pinMode(3, INPUT_PULLUP);
  //attachInterrupt(1, change_mode, FALLING);  // Прерывание на кнопку, смена режима подсветки по кнопке // D3
}

void loop()
{
  (*modes_arr[mode])();
  
  if (time_rpm - millis() >= 1000)
  {
    now_rpm = counter_rpm * 60;
    counter_rpm = 0;
    time_rpm = millis();
  }
  
  
  
  
  
  
}

void rpm_counter()
{
  ++counter_rpm;
}
