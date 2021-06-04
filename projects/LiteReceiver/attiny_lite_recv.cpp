//* ATtiny85: spavanje, cekanje na dati signal, blink ako se dobije signal.
//* Za slanje signala koristim PulseInTest/arduino_pulseout.cpp
//* Za 5V aparat radi pouzdano na 8MHz i budan trosi 10mA, a ne radi pouzdano na 1MHz kad trosi 5mA.
//* https://gist.github.com/dwhacks/8055287
/*
 * Watchdog Sleep Example 
 * Demonstrate the Watchdog and Sleep Functions
 * LED on digital pin 0
 * 
 * KHM 2008 / Lab3/  Martin Nawrath nawrath@khm.de
 * Kunsthochschule fuer Medien Koeln
 * Academy of Media Arts Cologne
 *
 * Modified on 5 Feb 2011 by InsideGadgets (www.insidegadgets.com)
 * to suit the ATtiny85 and removed the cbi( MCUCR,SE ) section 
 * in setup() to match the Atmel datasheet recommendations
 */

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

typedef unsigned long ulong;

const byte pinIn = 1;  // pin za DATA na SRX882
const byte pinCS = 2;  // pin za CS na SRX882: HIGH->work, LOW->sleep
const byte pinLed = 4; // pin za test LED diodu
ulong pul;             // duzina tekuceg pulsa u usec
ulong lastPul;         // duzina prethodnog pulsa u usec
// 0->16ms, 1->32ms, 2->64ms, 3->128ms, 4->250ms, 5->500ms
// 6->1sec, 7->2sec, 8->4sec, 9->8sec
const int factWatchdog = 6;
const int cntWatchdog = 5;        // ukupan broj kratkih budjenja tiny-a pre nego sto uzme da ceka signal
volatile int f_wdt = cntWatchdog; // brojac 0..cntWatchdog kratkih budjenja tiny-a
const ulong itvSignalWait = 200;  // vreme (u ms) koje ce tiny provesti u budnom stanju, tj. cekati singal
const byte cntPulses = 5;         // broj pulseva u signalu
#include <CircularBuffer.h>
CircularBuffer<ulong, cntPulses> lastPulses;           // poslednjih cntPulses pulseva
ulong targetPulses[] = {2675, 2000, 3000, 4000, 5000}; // sekvenca pulseva koju cekam

void setup()
{
    pinMode(pinLed, OUTPUT);
    pinMode(pinCS, OUTPUT);
    pinMode(pinIn, INPUT);
    setup_watchdog(factWatchdog);
}

//todo
// + svaki blink da traje po 1sec
// + tiny da spava odredjeno vreme
// + da se ceka signal odredjeno vreme
// - SRX da se uspava kad i tiny

void loop()
{
    if (f_wdt == cntWatchdog)
    {              // wait for timed out watchdog / flag is set when a watchdog timeout occurs
        f_wdt = 0; // reset flag
        digitalWrite(pinCS, true);
        ulong ms = millis();
        // digitalWrite(pinLed, HIGH); // let led blink
        while (millis() < ms + itvSignalWait)
        {
            //B delay(100);
            pul = pulseIn(pinIn, LOW);
            if (pul != lastPul)
            {
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
                    digitalWrite(pinCS, false);
                    // 1 blink kao simulacija nekog posla koji aparat treba da obavi na dati signal
                    digitalWrite(pinLed, true);
                    delay(250);
                    digitalWrite(pinLed, false);
                    break;
                }
            }
            lastPul = pul;
        }
        digitalWrite(pinCS, false);
        // digitalWrite(pinLed, LOW);
    }
    // set all used port to input to save power
    pinMode(pinLed, INPUT);
    system_sleep();
    // set all ports into state before sleep
    pinMode(pinLed, OUTPUT);
    pinMode(pinCS, OUTPUT);
}

// set system into the sleep state
// system wakes up when wtchdog is timed out
void system_sleep()
{
    cbi(ADCSRA, ADEN); // switch Analog to Digitalconverter OFF

    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
    sleep_enable();

    sleep_mode(); // System sleeps here

    sleep_disable();   // System continues execution here when watchdog timed out
    sbi(ADCSRA, ADEN); // switch Analog to Digitalconverter ON
}

void setup_watchdog(int ii)
{
    byte bb;
    int ww;
    if (ii > 9)
        ii = 9;
    bb = ii & 7;
    if (ii > 7)
        bb |= (1 << 5);
    bb |= (1 << WDCE);
    ww = bb;

    MCUSR &= ~(1 << WDRF);
    // start timed sequence
    WDTCR |= (1 << WDCE) | (1 << WDE);
    // set new watchdog timeout value
    WDTCR = bb;
    WDTCR |= _BV(WDIE);
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect)
{
    f_wdt++; // set global flag
}
