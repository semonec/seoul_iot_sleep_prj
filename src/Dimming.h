#include "mbed.h"
#include "WS2812.h"
#include "PixelArray.h"

#define LED_COLOR_R 0xff0000
#define LED_COLOR_G 0x00ff00
#define LED_COLOR_B 0x0000ff



class Dimming {
public:
  Dimming(PinName pin) {
    // See the program page for information on the timing numbers
    // The given numbers are for the K64F
    m_ws = new WS2812(pin, m_led_nums, 0, 5, 5, 0);
    m_ws->useII(WS2812::OFF); // use per-pixel intensity scaling
    
    m_px = new PixelArray(m_led_nums);
    m_dimming_level = 0.05; // pre-defined dimming level value
    m_dimming_step_time = 0.05; // Each pre-defined dimming level's staying time.
    m_is_dim_on = false;
  }
  ~Dimming() {
    delete m_ws;
    delete m_px;
  }

  // level should be 0.0 to 1.0
  void set_px(double level) {
    int adjusted_level = (int)(255 * level);
    int leveled_color_r = adjusted_level * 65536;
    int leveled_color_g = adjusted_level * 256;
    int leveled_color_b = adjusted_level * 1;
    int color = leveled_color_r + leveled_color_g + leveled_color_b;  
    for (int i = 0; i < m_led_nums; i++) {
      m_px->Set(i, color);
    } 
  }

  void set_light() {
    for (int z=m_led_nums; z >= 0 ; z--) {
      m_ws->write_offsets(m_px->getBuf(),z,z,z);
    }
  }

  void on() {
    if (m_is_dim_on)
      return;
    m_is_dim_on = true;
    for(double i = 0.0; i <= 1.0; i+= m_dimming_level) {
      set_px(i);
      set_light();
      wait(m_dimming_step_time);
    }
    set_px(1);
    set_light();
  }

  void off() {
    if (!m_is_dim_on)
      return;
    m_is_dim_on = false;      
    for(double i = 1.0; i >= 0.0; i-= m_dimming_level) {
      set_px(i);
      set_light();
      wait(m_dimming_step_time);
    }
    set_px(0);
    set_light();
  }

  void set_led_numbers(int num) {
    m_led_nums = num;
  }
  void set_dimming_level(double level) {
    m_dimming_level = level;
  }
  void set_dimming_step_time(double sec) {
    m_dimming_step_time = sec;
  }

  bool is_light_on() {
    return m_is_dim_on;
  }
private:
  WS2812* m_ws;
  PixelArray* m_px;
  int m_led_nums = 12;
  double m_dimming_level;
  double m_dimming_step_time;
  bool m_is_dim_on;
};