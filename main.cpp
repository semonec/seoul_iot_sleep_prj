#include "mbed.h"
#include "WS2812.h"
#include "PixelArray.h"

#define NUM_OF_LEDS 12
#define LED_COLOR_R 0xff0000
#define LED_COLOR_G 0x00ff00
#define LED_COLOR_B 0x0000ff

PixelArray px(NUM_OF_LEDS);
// See the program page for information on the timing numbers
// The given numbers are for the K64F
WS2812 ws(D9, NUM_OF_LEDS, 0, 5, 5, 0);


// level should be 0.0 to 1.0
void set_px(double level) {
  int adjusted_level = (int)(255 * level);
  printf("adj level: %d", adjusted_level);
  int leveled_color_r = adjusted_level * 65536;
  int leveled_color_g = adjusted_level * 256;
  int leveled_color_b = adjusted_level * 1;
  int color = leveled_color_r + leveled_color_g + leveled_color_b;  
  for (int i = 0; i < NUM_OF_LEDS; i++) {
    printf("strip color: %x\n", color);
    px.Set(i, color);
  } 
}

void set_light() {
  for (int z=NUM_OF_LEDS; z >= 0 ; z--) {
    ws.write_offsets(px.getBuf(),z,z,z);
  }
}

void dim_on() {
  for(double i = 0.0; i <= 1.0; i+= 0.05) {
    set_px(i);
    set_light();
    wait(0.05);
  }
}

void dim_off() {
  for(double i = 1.0; i >= 0.0; i-= 0.05) {
    set_px(i);
    set_light();
    wait(0.05);
  }
}

int main()
{
  ws.useII(WS2812::OFF); // use per-pixel intensity scaling

  // Now the buffer is written, rotate it
  // by writing it out with an increasing offset
  while (1) {
    dim_on();
    dim_off();
    // set_px(0.0);
    // set_light();
    wait(1.0);
  }
}