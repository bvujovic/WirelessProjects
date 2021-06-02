//* ATtiny85 prihvata impulse sa SRX882 i ceka targetPulses sekvencu.
//* Dobijene vrednosti impulsa sa loguju preko EEPROMing lib (pod komentarima)
//* ili se salju na Arduino preko I2C.

//#include <EEPROMing.h>
//EEPROMing ee;
#include <Arduino.h>
#include <Wire.h>

typedef unsigned long ulong;

const byte pinIn = 1;
const byte pinLed = 4;
ulong pul, lastPul;

const byte cntPulses = 5;
#include <CircularBuffer.h>
CircularBuffer<ulong, cntPulses> lastPulses;           // poslednjih cntPulses pulseva
ulong targetPulses[] = {2675, 2000, 3000, 4000, 5000}; // sekvenca pulseva koju cekam

void requestEvent()
{
    for (int i = 0; i < cntPulses; i++)
        Wire.write(lastPulses[i] / 100);
}

void setup()
{
    //      ee.MemToDisplay(200);
    //      while(1)
    //        delay(100);
    Wire.begin(8);                // join i2c bus with address #8
    Wire.onRequest(requestEvent); // register event

    pinMode(pinIn, INPUT);
    pinMode(pinLed, OUTPUT);
    //  ee.SetWritePos(200);
}

void loop()
{
    pul = pulseIn(pinIn, LOW);
    if (pul != lastPul)
    {
        //* ideja da se svaki puls belezi u EEPROM verovatno nije dobra
        //* tako se neki puls moze propustiti zbog cekanja na upis u trajnu memoriju
        // ee.Write(pul / 100);
        int cntHits = 0;
        lastPulses.push(pul);
        ulong lastTarget = targetPulses[cntPulses - 1];
        if (pul > lastTarget - 100 && pul < lastTarget + 200)
            for (int i = 0; i < cntPulses; i++)
            {
                int diff = lastPulses[i] - targetPulses[i];
                if (diff > -100 && diff < 200)
                    cntHits++;
                else
                    break;
            }
        if (cntHits == cntPulses)
        {
            digitalWrite(pinLed, true);
            delay(250);
            digitalWrite(pinLed, false);
        }
    }
    lastPul = pul;
}
