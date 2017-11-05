#include "mbed.h"
#include "Dimming.h"

Dimming dim(D9);
InterruptIn button(D0);

void event_button_pressed() {
  printf("Turn on with dimming");
  dim.on();
}

void event_button_released() {
  printf("Turn off with dimming");
  dim.off();
}

int main()
{
  button.fall(&event_button_released);
  button.rise(&event_button_pressed);
  while (1) {
    // dim.dim_on();
    // dim.dim_off();
    // int button_status = button.read();
    // if (button_status == 1) {
    //   printf("+++button pressed, dim status: %d\n", dim.is_light_on());
      
    //   if (!dim.is_light_on()) {
    //     printf("**** dim on***");
    //     dim.dim_on();
    //   }
    // } else if (button_status == 0) {
    //   printf("---button released, dim status: %d\n", dim.is_light_on());        
      
    //   if (dim.is_light_on()) {
    //     printf("**** dim off***");
    //     dim.dim_off();
    //   }
    // }
    // wait(1.0);
  }
}