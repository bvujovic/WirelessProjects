//* Slanje periodicnih test signala koriscenjem NexaCtrl klase.
//* https://github.com/TheCarlG/NexaCtrl

#include <NexaCtrl.h>

//* za Arduino
// const byte pinLed = LED_BUILTIN;
// const byte pinTx = 7;
// const byte pinRx = 8;
//* za ATtiny85
const byte pinLed = 2;
const byte pinTx = 0;
const byte pinRx = 3;

// Your remote id which can be it can be "sniffed" with the
// reciever example at http://playground.arduino.cc/Code/HomeEasy
const static unsigned long controller_id = 123;
unsigned int device = 0;

NexaCtrl nexaCtrl(pinTx, pinRx, pinLed);

void setup() {}

void loop()
{
    nexaCtrl.DeviceOn(controller_id, device);
    delay(1000);
    nexaCtrl.DeviceOff(controller_id, device);
    delay(4000);
}
