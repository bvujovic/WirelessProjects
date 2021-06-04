//* Arduino preko STX882 salje test signal koji moze da primi ATtiny85 sa pulseIn svakih 3 sec.

#include <Arduino.h>

typedef unsigned int uint;

const byte pinLed = LED_BUILTIN;
const byte pinTx = 7;

const uint itvMain = 100; // (u msec) bleja izmedju slanja signala, za test itvMain=3000
const uint itvHigh = 3000; // (u usec) trajanje HIGH signala

// Salje (HIGH/LOW) na pinTx i drzi tu vrednost itv mikrosekundi.
void send(int val, uint itv)
{
    digitalWrite(pinTx, val);
    delayMicroseconds(itv);
}
// Salje HIGH signal na pinTx i drzi ga itvHigh mikrosekundi.
void sendHigh() { send(HIGH, itvHigh); }
// Salje LOW signal na pinTx i drzi ga itv mikrosekundi.
void sendLow(uint itv) { send(LOW, itv); }
// Salje HIGH (itvHigh usec) pa LOW (itv usec) na pinTx.
void sendPulse(uint itv)
{
    sendHigh();
    sendLow(itv);
}

void setup()
{
    pinMode(pinTx, OUTPUT);
    pinMode(pinLed, OUTPUT);
}

void loop()
{
    cli(); // disable interupts
    digitalWrite(pinLed, HIGH);

    sendPulse(4500); // Latch 1 - nekad je potreban, a nekad nije!!
    sendPulse(2675); // Latch 2

    // trajanje slanja signala (u ms): 10+2+3+4+5 = 24
    for (size_t i = 2; i <= 5; i++)
    {
        //B sendPulse(2000);     // za ATtiny85 ide svaki drugi fake signal - ispade da ne treba
        sendPulse(i * 1000); // pravi signal
    }
    sendHigh(); // HIGH za kraj

    digitalWrite(pinLed, LOW);
    sei();
    delay(itvMain);
}
