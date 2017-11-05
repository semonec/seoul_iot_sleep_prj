#include "mbed.h"
#include "Dimming.h"

enum EventList {
  ButtonOff,
  ButtonOn
};

Dimming dim(D9);
InterruptIn button(D0);
EventQueue queue;
Thread eventThread;
Serial pc(USBTX, USBRX);

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
  eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

  button.fall(queue.event(dispatcher, ButtonOff));
  button.rise(queue.event(dispatcher, ButtonOn));

  // All the event from sensor should be sent to dispatcher function with params;
  // example
  // queue.event(dispatcher, TestSensorReaded, value1);
  while (1) {}
}