#include "global_var.h"
//uint8_t* NOW_COLOR;

const mData colors[] = {mRed, mGreen, mYellow, mOrange, mLime, mMaroon, mOlive, mBlue, mNavy, mAqua, mMagenta, mTeal, mPurple, mSilver, mWhite}; 

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
  delay(random() % *NOW_DELAY);
  strip.show();
  
  return;
}

void animation_wheel()
{
  for (int j=0; j<256;j+=10)  // Сдвиг колеса hue
  {
    for (int i=0; i<NUMLEDS;++i)  // Цвета колеса hue
  {
    strip.set(i, mWheel((i*255/NUMLEDS + j)% 256));
    strip.show();
  }
  delay(*NOW_DELAY);
  }
  
  return;
}

void loading()
{
  for (int i = 0; i < NUMLEDS; ++i)  // По кругу только два диода
  {
    strip.set(i, colors[*NOW_COLOR]);
    i>1 ? strip.set(i-2, mRGB(0, 0, 0)) : strip.set(NUMLEDS+i-2, mRGB(0, 0, 0));  // Тушим цвет диода, который предшествует двум горящим
    delay(*NOW_DELAY);
    strip.show();
  }
  
  return;
}

void loading_2()
{
  mData temp = colors[rand() % NUMCOLORS];
  for (int i = 0; i < NUMLEDS; ++i)
  {
    strip.set(i, temp);
    i < (NUMLEDS / 2) ? *NOW_DELAY += 3 : *NOW_DELAY -= 3;
    delay(*NOW_DELAY);
    strip.show();
  }

  delay(*NOW_DELAY * 8);

  for (int i = 0; i < NUMLEDS; ++i)
  {
    strip.set(i, mBlack);
    delay(*NOW_DELAY);
    strip.show();
  }

  delay(*NOW_DELAY * 8);

  return;
}

void hue_colors()
{
  for (int j=0; j<=25;++j)  // 25 раз
  {
    for (int i=NUMLEDS-1; i>=0;--i)  // 25 цветов колеса hue с появлением против часовой стрелки
    {
      strip.set(i, mWheel(j*10));
      delay(15);
      strip.show();
   }

   delay(*NOW_DELAY);
  }

  return;
}

void blinking()
{
  for (int i = 0; i < NUMLEDS; ++i)
    {
      strip.set(i, colors[*NOW_COLOR]);
   }
   strip.show();
   for (int i = 255; i >= 0; --i)
    {
      strip.setBrightness(i);
      strip.show();
      delay(*NOW_DELAY / 5);
   }
   for (int i = 0; i < 256; ++i)
    {
      strip.setBrightness(i);
      strip.show();
      delay(*NOW_DELAY / 5);
   }

   return;
}

void two_led()
{
  for (int j = 0; j < NUMCOLORS; ++j)
  {
    mData color = colors[rand() % NUMCOLORS];
    for (int i = 0; i < NUMLEDS / 2; ++i)
    {
      //strip.clear();
      
      strip.set(i, color);
      strip.set(i + NUMLEDS / 2, color);
      
      delay(*NOW_DELAY);
      strip.show();
   }
  }

  return;
}

void blinking_different_colors()
{
  for (int j = 0; j < NUMCOLORS; ++j)  // NUMCOLORS раз
  {
    for (int i = 0; i < NUMLEDS; ++i)
    {
      strip.set(i, colors[j]);
      delay(*NOW_DELAY);
      strip.show();
   }
   for (int i = 255; i >= 0; --i)
    {
      strip.setBrightness(i);
      strip.show();
      delay(*NOW_DELAY / 5);
   }
   for (int i = 0; i < 256; ++i)
    {
      strip.setBrightness(i);
      strip.show();
      delay(*NOW_DELAY / 5);
   }
  }

  return;
}

void blinking_different_colors_2()
{
  for (int j = 0; j < NUMCOLORS-1; ++j)  // NUMCOLORS раз
  {
   for (int i = 255; i >= 0; --i)
    {
      strip.setBrightness(i);
      strip.show();
      delay(*NOW_DELAY / 5);
   }
   for (int i = 0; i < NUMLEDS; ++i)
    {
      strip.set(i, colors[j+1]);
   }
   strip.show();
   for (int i = 0; i < 256; ++i)
    {
      strip.setBrightness(i);
      strip.show();
      delay(*NOW_DELAY / 5);
   }
  }

  return;
}
