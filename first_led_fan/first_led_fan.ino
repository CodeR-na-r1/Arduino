#define STRIP_PIN 2     // пин ленты
#define NUMLEDS 12      // кол-во светодиодов
#define COLOR_DEBTH 1   // кол-во байт на цвет

#include <microLED.h>

microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip;

long last_time = millis();

void setup() {
  strip.setBrightness(255);
  strip.show(); // вывод изменений на ленту
  delay(1);  
    
  Serial.begin(9600);
}

void loop() {
  Serial.println("new_loop!");
  strip.clear();
  
  for (int i=0; i<NUMLEDS;++i)  // Зеленый
  {
    strip.set(i, mRGB(0, 255, 0));
    delay(30);
    strip.show();
  }

  delay(1000);

  for (int i=0; i<NUMLEDS;++i)  // Цвета колеса hue
  {
    strip.set(i, mWheel(i*255/NUMLEDS));
    delay(30);
    strip.show();
  }

  delay(1000);
  
  for (int j=0; j<=25;++j)  // 25 раз
  {
    for (int i=NUMLEDS-1; i>=0;--i)  // 25 цветов колеса hue с появлением против часовой стрелки
    {
      strip.set(i, mWheel(j*10));
      delay(15);
      strip.show();
   }
  }

  delay(1000);

  for (int i=0; i<NUMLEDS;++i)  // Фиолетовый
  {
    strip.set(i, mRGB(148, 0, 211));
    delay(30);
    strip.show();
  }

  delay(1000);

   for (int i=NUMLEDS-1; i>=0;--i)  // черыный с появлением против часовой стрелки
  {
    strip.set(i, mRGB(0, 0, 0));
    delay(30);
    strip.show();
  }
  
  for (int j=0; j<NUMLEDS;++j)  // NUMLEDS раз
  {
    for (int i=0; i<NUMLEDS;++i)  // Красный по кругу только два диода
    {
    strip.set(i, mRGB(255, 0, 0));
    i>1 ? strip.set(i-2, mRGB(0, 0, 0)) : strip.set(NUMLEDS+i-2, mRGB(0, 0, 0));  // Тушим цвет диода, который предшествует двум горящим
    delay(30);
    strip.show();
    }
  }

  // if (millis() - last_time >= 5000) // Сработывает раз в ~5 секунд
  // {
  //   last_time = millis();
  //   Serial.println("5 seconds прошло");
  // }
}
