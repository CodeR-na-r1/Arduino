// Скетч для адресной светодиодной ленты

#pragma once

#include "config.h"  // Файл с конфигом
#include "headers.h"  // Файл с заголовками

/* Инициализации */

LiquidCrystal_I2C display(0x27, 16, 2); // Дисплей

microLED<NUM_LEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip; // Адресная лента в кулерах
void (*modes_arr[])() = {led_off, static_color, loading_2, two_led};  // Массив с функциями подсветок

bool setup_mode = false; // Флаг активации режима настройки (регулировки оборотов, цвета и режима)
bool power_display = false; // Флаг активации дисплея

uint8_t mode = 1; // Счётчик текущего режима
uint8_t color = 0; //  Счётчик текущего цвета
uint8_t mode_delay = 18; // Счётчик задержки для режимов подсветки
volatile int counter_rpm(0);  // Счётчик оборотов кулера
int now_rpm(0);  // Счётчик оборотов кулера для выввода на дисплей

auto timer_rpm = millis();  // Таймер для замера оборотов кулера
unsigned long long int setup_timer = millis();  // Таймер для активации режима настройки
unsigned long long int change_setup_timer = millis();  // Таймер для удобного замера значений при режиме настройки
unsigned long long int INSIDE_LED_timer = millis();  // Таймер для мигания встроенным светодиодом

int pwm_input(0);  // Значение сигнала с аналового порта (потенциометр)
int pwm_output(0);  // Значение ШИМ сигнала, выставляемое на цифровой порт

void setup()
{
  /* Настройки объектов */

  Serial.begin(9600);  // Порт
  Serial.println("Setup stage");

  Wire.begin();   // Дисплей
  display.begin();
  display.backlight();
  display.noBlink();
  display.noCursor();
  display.noDisplay();

  strip.setBrightness(255);  // Лента
  strip.clear();
  strip.show();

  randomSeed(analogRead(0));  // Инициализация для rand()

  /* Прерывания */

  //pinMode(SENSE_PIN, INPUT);  //
  attachInterrupt(0, rpm_counter, CHANGE);  // Прерывание sense fan, счётчик оборотов (D2)

  //pinMode(3, INPUT_PULLUP);
  //attachInterrupt(1, change_mode, FALLING);  // Свободное прерывание (D3)

  /* Настройки пинов */

  pinMode(PWM_CONTROL_PIN, INPUT);  // Аналоговый порт с потенциометром

  // Настройка ШИМ сигнала на пины D9 и D10 - 31.4 кГц (подача питания (ШИМ) на вентиляторы)
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000001;  // x1 phase correct
  pinMode(PWM_PIN, OUTPUT);  // Цифровой порт с генерацией ШИМ

  pinMode(SETUP_PIN, INPUT_PULLUP);  // Кнопка активации режима настроек

  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);  // Кнопка смены режима

  pinMode(COLOR_BUTTON_PIN, INPUT_PULLUP);  // Кнопка смены цвета

  pinMode(INSIDE_LED_PIN, OUTPUT);  // Индикация встроенным светодиодом
}

void loop()
{
  (*modes_arr[mode])(); // Вызов функции подсветки

  if (millis() - timer_rpm > 500)  // Подсчет оборотов кулера
  {
    now_rpm = counter_rpm;
    counter_rpm = 0;
    timer_rpm = millis();
  }

  if (!digitalRead(SETUP_PIN))  // Для активации режима настроек
  {
    setup_timer = millis();
    while (!digitalRead(SETUP_PIN))
    {
      if (millis() - setup_timer > 500) // Для активации режима настроек зажать кнопку на полсекунды
      {
        setup_mode ? completed() : start_setup();  // Индикация подсветкой вентиляторов об активации или деактивации режима настроек
        delay(890);  // Выжидание
        setup_mode = !setup_mode;  // Переключение режима
        power_display ? display.noDisplay() : display.display();  // Активация или деактивация дисплея
        power_display = !power_display;  // Переключение режима дисплея
        break;
      }
    }
    (*modes_arr[mode])(); // Возврат подсветки
  }

  if (setup_mode)  // Если активирован режим настроек
  {
    if (millis() - change_setup_timer > CHANGE_SETUP_TIME)  // Для плавного переключения параметров
    {
      change_setup_timer = millis();
      if (!digitalRead(MODE_BUTTON_PIN))  // Проверка действия с кнопкой смены режима
      {
        mode = (++mode >= NUM_MODES) ? 0 : mode;
      }
      if (!digitalRead(COLOR_BUTTON_PIN))  // Проверка действия с кнопкой смены цвета
      {
        color = (++color >= NUM_COLORS) ? 0 : color;
      }
      pwm_input = analogRead(PWM_CONTROL_PIN);  // Считывание значения с потенциометра
      pwm_output = constrain(map(pwm_input, 0, 1023, 0, 255), 0, 255);  // Преобразование значения с потенциометра
      analogWrite(PWM_PIN, pwm_output);  // Генерация ШИМ

      display.clear();  // Очистка дисплея и выввод информации на дисплей
      display.setCursor(0, 0);
      display.print("PWM: ");
      display.print(pwm_output);
      display.print("; ");
      display.print(now_rpm);
      display.print(" rpm");
      display.setCursor(0, 1);
      display.print("Color: ");
      display.print(color);
      display.print(";Mode: ");
      display.print(mode);
    }
    if (millis() - INSIDE_LED_timer > 400)  // Индикация встроенным светодиодом
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

void rpm_counter()  // Прерывание подсчета rpm fan
{
  counter_rpm += 1;
}
