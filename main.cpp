#include "mbed.h"
#include "Dimming.h"

Dimming dim(D9);
DigitalIn button(D0);

int main()
{
  while (1) {
    // dim.dim_on();
    // dim.dim_off();
    int button_status = button.read();
    if (button_status == 1) {
      printf("+++button pressed, dim status: %d\n", dim.is_light_on());
      
      if (!dim.is_light_on()) {
        printf("**** dim on***");
        dim.dim_on();
      }
    } else if (button_status == 0) {
      printf("---button released, dim status: %d\n", dim.is_light_on());        
      
      if (dim.is_light_on()) {
        printf("**** dim off***");
        dim.dim_off();
      }
    }
    wait(1.0);
  }
}