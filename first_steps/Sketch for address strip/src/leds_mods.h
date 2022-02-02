#include "global_variables.h"

const mData colors[NUM_COLORS] = {mPurple, mRed, mOrange, mYellow, mAqua, mNavy, mMagenta, mSilver};

void off()
{
  flag_dynamic = false;

  for (int i(0); i < NUMLEDS; ++i)
  {
    strip.set(i, mBlack);
    strip.show();
    delayMicroseconds(40);
  }

  return;
}

void static_color()
{
  flag_dynamic = false;

  for (int i(0); i < NUMLEDS; ++i)
  {
    strip.set(i , colors[*color]);
    strip.show();
    delayMicroseconds(40);
  }

  return;
}
