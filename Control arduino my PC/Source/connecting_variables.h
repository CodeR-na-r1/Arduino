#pragma once

extern microLED<NUM_LEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip; // лента

extern uint8_t mode; // режим
extern uint8_t color; // цвет
extern uint8_t mode_delay; // задержка в режимах
