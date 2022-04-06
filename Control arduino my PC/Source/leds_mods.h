#pragma once

#include "connecting_variables.h"
#include "config.h"

const mData colors[] = {mPurple, mRed, mOrange, mYellow, mAqua, mNavy, mMagenta, mSilver}; 

void led_off()
{
  for (int i=0; i<NUMLEDS;++i)
  {
    strip.set(i, mRGB(0, 0, 0));
  }
  
  strip.show();
  
  return;
}