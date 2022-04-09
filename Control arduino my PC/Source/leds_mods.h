#pragma once

#include "connecting_variables.h"
#include "config.h"

const mData colors[] = {mPurple, mRed, mOrange, mYellow, mAqua, mNavy, mMagenta, mSilver}; 

void led_off()
{
  for (int i=0; i<NUM_LEDS;++i)
  {
    strip.set(i, mRGB(0, 0, 0));
  }
  
  strip.show();
  
  return;
}

void static_color()
{
  for (int i=0; i<NUM_LEDS;++i)
  {
    strip.set(i, colors[color]);
    delay(mode_delay);
    strip.show();
  }
  
  return;
}

void loading_2()
{
  mData temp = colors[rand() % NUM_COLORS];
  for (int i = 0; i < NUM_LEDS; ++i)
  {
    strip.set(i, temp);
    i < (NUM_LEDS / 2) ? mode_delay += 3 : mode_delay -= 3;
    delay(mode_delay);
    strip.show();
  }

  delay(mode_delay * 8);

  for (int i = 0; i < NUM_LEDS; ++i)
  {
    strip.set(i, mBlack);
    delay(mode_delay);
    strip.show();
  }

  delay(mode_delay * 8);

  return;
}

void two_led()
{
  for (int j = 0; j < NUM_COLORS; ++j)
  {
    mData color = colors[rand() % NUM_COLORS];
    for (int i = 0; i < NUM_LEDS / 2; ++i)
    {
      //strip.clear();
      
      strip.set(i, color);
      strip.set(i + NUM_LEDS / 2, color);
      
      delay(mode_delay);
      strip.show();
   }
  }

  return;
}
