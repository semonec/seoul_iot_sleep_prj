#include "mbed.h"
#include "Dimming.h"

int main()
{
  DIMMING dim(D9);
  // Now the buffer is written, rotate it
  // by writing it out with an increasing offset
  while (1) {
    // dim_on();
    // dim_off();
    // set_px(0.0);
    // set_light();
    dim.dim_on();
    dim.dim_off();
    wait(1.0);
  }
}