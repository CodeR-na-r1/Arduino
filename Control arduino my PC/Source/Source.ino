// Скетч для адресной светодиодной ленты

#pragma once

#include "config.h"  // Файл с конфигом
#include "headers.h"  // Файл с заголовками

/* Инициализации */

LiquidCrystal_I2C display(0x27, 16, 2); // Дисплей

microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip; // Адресная лента в кулерах
void (*modes_arr[])() = {led_off};  // Массив с функциями подсветок

uint8_t mode; // Счётчик текущего режима
uint8_t color; //  Счётчик текущего цвета

void setup()
{
  /* Настройки объектов */

  Serial.begin(9600);
  Serial.println("Setup stage");
  
  Wire.begin(); 
  display.begin();
  display.backlight();
  display.noBlink();
  display.noCursor();

  strip.setBrightness(255);
  strip.clear();
  strip.show();

  pinMode(3, INPUT_PULLUP);
  //attachInterrupt(1, change_mode, FALLING);  // Прерывание на кнопку, смена режима подсветки по кнопке // D3
  pinMode(2, INPUT_PULLUP);
  //attachInterrupt(0, change_color, FALLING);  // Прерывание на кнопку, смена цвета подсветки по кнопке  // D2
  
  randomSeed(analogRead(0));
  pinMode(3,OUTPUT);
  analogWrite(3, 127);
}

void loop()
{
  
}
