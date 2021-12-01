#include "global_var.h"
//uint8_t* NOW_COLOR;

mData colors[] = {mRed, mGreen, mYellow, mOrange, mLime, mMaroon, mOlive, mBlue, mNavy, mAqua, mMagenta, mTeal, mPurple, mSilver, mWhite}; 

void led_off()
{
  for (int i=0; i<NUMLEDS;++i)
  {
    strip.set(i, mRGB(0, 0, 0));
  }
  
  strip.show();
  
  return;
}

void static_color()
{
  for (int i=0; i<NUMLEDS;++i)
  {
    strip.set(i, colors[*NOW_COLOR]);
    delay(30);
    strip.show();
  }
  
  return;
}

void random_leds()
{
  strip.set(random() % NUMLEDS, colors[random() % NUMCOLORS]);
  delay(random() % 50);
  strip.show();
  
  return;
}

void led_first()
{
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
  return;
}
