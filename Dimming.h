#include "mbed.h"
#include "WS2812.h"
#include "PixelArray.h"

#define NUM_OF_LEDS 12
#define LED_COLOR_R 0xff0000
#define LED_COLOR_G 0x00ff00
#define LED_COLOR_B 0x0000ff



class Dimming {
public:
  Dimming(PinName pin) {
    // See the program page for information on the timing numbers
    // The given numbers are for the K64F
    ws = new WS2812(pin, NUM_OF_LEDS, 0, 5, 5, 0);
    ws->useII(WS2812::OFF); // use per-pixel intensity scaling
    
    px = new PixelArray(NUM_OF_LEDS);
  }
  ~Dimming() {
    delete ws;
    delete px;
  }

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
      px->Set(i, color);
    } 
  }

  void set_light() {
    for (int z=NUM_OF_LEDS; z >= 0 ; z--) {
      ws->write_offsets(px->getBuf(),z,z,z);
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

private:
  WS2812* ws;
  PixelArray* px;
};