#include "mbed.h"
#include "Dimming.h"
// For K64F, wifi driver port
#ifdef TARGET_K64F
#include "wifi_k64f.h"
#endif
// temp & humidity sensor
#include "DHT.h"
#include "simplem2mclient.h"


enum EventList {
  ButtonOff,
  ButtonOn
};

Dimming dim(D9);
InterruptIn button(D2);
EventQueue queue;
Thread eventThread;
DHT sensor(D4, DHT11);
Serial pc(USBTX, USBRX);

// DECLARE RESOURCES HERE
static M2MResource* message_res; // Resource to hold a string message

void dispatcher(int event_type) {
  pc.printf("Event Dispatcher: eventType: %d\n", event_type);

  switch(event_type) {
    case ButtonOn:
      dim.on();  
    break;
    case ButtonOff:
      dim.off();
    break;
  }
}
int main()
{
#ifdef TARGET_K64F
  pc.printf("connect wifi\n");
  connect_wifi();
#endif
  eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

  button.fall(queue.event(dispatcher, ButtonOff));
  button.rise(queue.event(dispatcher, ButtonOn));

  // MBED cloud initializing
      // SimpleClient is used for registering and unregistering resources to a server.
  SimpleM2MClient mbedClient;
  mbedClient.start_client();
  
  if (!mbedClient.init()) {
      printf("Initialization failed, exiting application!\n");
      return 1;
  }

  // All the event from sensor should be sent to dispatcher function with params;
  // example
  // queue.event(dispatcher, TestSensorReaded, value1);

  int error = 0;
  float h = 0.0f, c = 0.0f, f = 0.0f, k = 0.0f, dp = 0.0f, dpf = 0.0f;
  while (1) {
    wait(2.0f);
    error = sensor.readData();
    if (0 == error) {
        c   = sensor.ReadTemperature(CELCIUS);
        f   = sensor.ReadTemperature(FARENHEIT);
        k   = sensor.ReadTemperature(KELVIN);
        h   = sensor.ReadHumidity();
        dp  = sensor.CalcdewPoint(c, h);
        dpf = sensor.CalcdewPointFast(c, h);
        pc.printf("Temperature in Kelvin: %4.2f, Celcius: %4.2f, Farenheit %4.2f\n", k, c, f);
        pc.printf("Humidity is %4.2f, Dewpoint: %4.2f, Dewpoint fast: %4.2f\n", h, dp, dpf);
    } else {
        pc.printf("Error: %d\n", error);
    }
  }
}