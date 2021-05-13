// https://learn.sparkfun.com/tutorials/reducing-arduino-power-consumption/all
// potrosnja 3.7mA

#include "LowPower.h"

const byte ledPin = LED_BUILTIN;

void setup()
{
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    digitalWrite(ledPin, HIGH);
    delay(4000);
    digitalWrite(ledPin, LOW);
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    //* Mozda bi budjenje na interapt moglo da prodje sa SLEEP_FOREVER podesavanjem
}
