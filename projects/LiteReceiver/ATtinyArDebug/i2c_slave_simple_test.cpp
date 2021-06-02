//* Test kôd za ATtiny koji vraca fiksni string preko I2C veze.
//* Prave podatke o duzini pulseva vraca PulseInTest/attiny_pulsein.cpp

#include <Arduino.h>
#include <Wire.h>

void requestEvent() { Wire.write("Hello."); }

void setup()
{
    Wire.begin(9);
    Wire.onRequest(requestEvent);
}

void loop()
{
    delay(100);
}