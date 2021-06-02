//* Slanje periodicnih test signala koriscenjem NexaCtrl klase.
//* https://github.com/TheCarlG/NexaCtrl

#include <NexaCtrl.h>

//* za Arduino
const byte pinLed = LED_BUILTIN;
const byte pinTx = 7;
const byte pinRx = 8;
//* za ATtiny85
// const byte pinLed = 2;
// const byte pinTx = 0;
// const byte pinRx = 3;

// Your remote id which can be it can be "sniffed" with the
// reciever example at http://playground.arduino.cc/Code/HomeEasy
const static unsigned long controller_id = 123;
unsigned int device = 0;

NexaCtrl nexaCtrl(pinTx, pinRx, pinLed);

const int itvMain = 3000;
const int kPulseLow0 = 3000;

void setup()
{
    cli(); // disable interupts
}

void loop()
{
    // Latch
    digitalWrite(pinTx, HIGH);
    delayMicroseconds(kPulseLow0);
    digitalWrite(pinTx, LOW);
    delayMicroseconds(2675);

    // 4+5+4*(3+2+3+4.5)=59
    for (size_t i = 2; i <= 5; i++)
    {
        //* za ATtiny85 ide svaki drugi fake signal
        digitalWrite(pinTx, HIGH);
        delayMicroseconds(kPulseLow0);
        digitalWrite(pinTx, LOW);
        delayMicroseconds(2000);

        // pravi signal
        digitalWrite(pinTx, HIGH);
        delayMicroseconds(kPulseLow0);
        digitalWrite(pinTx, LOW);
        delayMicroseconds(i * 1000);
    }

    // moj trip za kraj
    digitalWrite(pinLed, HIGH);
    digitalWrite(pinTx, HIGH);
    delayMicroseconds(kPulseLow0);
    digitalWrite(pinLed, LOW);

    sei(); // enable interupts

    delay(itvMain);

    // nexaCtrl.DeviceOn(controller_id, device);
    // delay(500);
    // nexaCtrl.DeviceOff(controller_id, device);
    // delay(itvMain);
}
