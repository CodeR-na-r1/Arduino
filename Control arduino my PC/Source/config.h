#pragma once

/* Определённые константы */

#define NUM_LEDS 12      // Кол-во светодиодов в кулерах
#define COLOR_DEBTH 3   // Байт на цвет
#define NUM_MODES 4   // Количество режимов
#define NUM_COLORS 8   // Количество цветов

#define STRIP_PIN 7     // Пин ленты
#define PWM_PIN 9   // Пин для генерации сигнала ШИМ в кулер
#define SENSE_PIN 2   //  Пин откуда идут импульсы для подсчёта оборотов кулера
#define PWM_CONTROL_PIN A0   // Пин с потенциометром (управление скоростью вращения кулера)
#define SETUP_PIN 11   // Пин с кнопкой активации режима настроек параметров
#define MODE_BUTTON_PIN 5   // Пин с кнопкой смены режима 
#define COLOR_BUTTON_PIN 6   // Пин с кнопкой смены цвета 
#define INSIDE_LED_PIN 13   // Пин с встроенным в ардуино светодиодом
#define CHANGE_SETUP_TIME 800   // Время ожидания между опросом датчиков в режиме настройки
