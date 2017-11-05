#include "mbed.h"
#include "Dimming.h"

int main()
{
  Dimming dim(D9);
  while (1) {

    dim.dim_on();
    dim.dim_off();
    wait(1.0);
  }
}