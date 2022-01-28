#include "global_variables.h"

const mData colors[] = {mPurple};

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
