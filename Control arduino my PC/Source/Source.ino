// Скетч для адресной светодиодной ленты

#pragma once

#include "config.h"  // Файл с конфигом
#include "headers.h"  // Файл с заголовками
#include "some_functions.h"  // Файл с заголовками

/* Инициализации */

LiquidCrystal_I2C display(0x27, 16, 2); // Дисплей

microLED<NUM_LEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip; // Адресная лента в кулерах
void (*modes_arr[])() = {led_off, static_color, loading_2, two_led};  // Массив с функциями подсветок

bool setup_mode = false;
bool power_display = false;

uint8_t mode = 1; // Счётчик текущего режима
uint8_t color = 0; //  Счётчик текущего цвета
uint8_t mode_delay = 18; //

volatile int counter_rpm(0);  //

auto timer_rpm = millis();
unsigned long long int setup_timer = millis();
unsigned long long int change_setup_timer = millis();
unsigned long long int INSIDE_LED_timer = millis();

int pwm_input(0);  //
int pwm_output(0);  //

void setup()
{
  /* Настройки объектов */

  Serial.begin(9600);  //
  Serial.println("Setup stage");
  display.begin();

  Wire.begin();   //
  display.begin();
  display.backlight();
  display.noBlink();
  display.noCursor();
  display.noDisplay();

  strip.setBrightness(255);  //
  strip.clear();
  strip.show();

  randomSeed(analogRead(0));  //

  /* Прерывания */

  //pinMode(SENSE_PIN, INPUT);  //
  //attachInterrupt(0, rpm_counter, CHANGE);  // Прерывание sense fan, счётчик оборотов (D2)

  //pinMode(3, INPUT_PULLUP);
  //attachInterrupt(1, change_mode, FALLING);  // Свободное прерывание (D3)

  /* Настройки пинов */

  pinMode(PWM_CONTROL_PIN, INPUT);  //

  // Настройка ШИМ сигнала на пины D9 и D10 - 31.4 кГц (подача питания на вентиляторы)
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000001;  // x1 phase correct
  pinMode(PWM_PIN, OUTPUT);  //

  pinMode(SETUP_PIN, INPUT_PULLUP);  //

  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);  //

  pinMode(COLOR_BUTTON_PIN, INPUT_PULLUP);  //

  pinMode(INSIDE_LED_PIN, OUTPUT);  //
}

void loop()
{
  (*modes_arr[mode])(); //

  if (millis() - timer_rpm > 500)  //
  {
    Serial.println(counter_rpm);
    counter_rpm = 0;
    timer_rpm = millis();
  }

  if (!digitalRead(SETUP_PIN))  //
  {
    setup_timer = millis();
    while (!digitalRead(SETUP_PIN))
    {
      if (millis() - setup_timer > 500)
      {
        setup_mode ? completed() : start_setup();  //
        delay(890);
        setup_mode = !setup_mode;  //
        power_display ? display.noDisplay() : display.display();
        power_display = !power_display;
        break;
      }
    }
    (*modes_arr[mode])(); //
  }

  if (setup_mode)  //
  {
    if (millis() - change_setup_timer > CHANGE_SETUP_TIME)
    {
      change_setup_timer = millis();
      if (!digitalRead(MODE_BUTTON_PIN))
      {
        mode = (++mode >= NUM_MODES) ? 0 : mode;
      }
      if (!digitalRead(COLOR_BUTTON_PIN))
      {
        color = (++color >= NUM_COLORS) ? 0 : color;
      }
      pwm_input = analogRead(PWM_CONTROL_PIN);
      pwm_output = constrain(map(pwm_input, 0, 1023, 0, 255), 0, 255);
      analogWrite(PWM_PIN, pwm_output);

      display.clear();
      display.setCursor(0, 0);
      display.print("PWM: ");
      display.print(pwm_output);
      display.setCursor(0, 1);
      display.print("Color: ");
      display.print(color);
      display.print(";Mode: ");
      display.print(mode);

    }
    if (millis() - INSIDE_LED_timer > 400)
    {
      digitalWrite(INSIDE_LED_PIN, !digitalRead(INSIDE_LED_PIN));
      INSIDE_LED_timer = millis();
    }
  }
  else
  {
    digitalWrite(INSIDE_LED_PIN, LOW);
  }
}

void rpm_counter()
{
  counter_rpm += 1;
}
