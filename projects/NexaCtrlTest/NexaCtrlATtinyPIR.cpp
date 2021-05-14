//* Signal sa PIR senzora probudi ATtiny i ovaj posalje signal preko stx882.
//* Originalni kôd: https://github.com/perja12/nexa_remote_control/blob/master/nexa_remote_control.ino
//* HomeEasy kôd (receiver) : https://playground.arduino.cc/Code/HomeEasy/

#include <NexaCtrl.h>
#include <avr/power.h>
#include <avr/sleep.h>

const byte pinTx = 0;
const byte pinRx = 2; // Not used.

#define CMD_NONE 0
#define CMD_1 1

// 26 bit controller id assigned to each Nexa device.
const static unsigned long CONTROLLER_ID = 123;

// 4 bit device id.
const static unsigned int DEVICE_1 = 0;

NexaCtrl nexaCtrl(pinTx, pinRx);

volatile int cmd = CMD_NONE;

ISR(PCINT0_vect)
{
    cli();
    if (!bit_is_set(PINB, PB1))
        cmd = CMD_1;
}

void setup()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    ADCSRA &= ~bit(ADEN); // Turn off ADC to save power.
    pinMode(PB1, INPUT);
    PCMSK |= _BV(PCINT1); // Configure pin change interrupt.
    GIFR |= bit(PCIF);    // clear any outstanding interrupts
    GIMSK |= bit(PCIE);   // enable pin change interrupts
}

void loop()
{
    pinMode(pinTx, INPUT); // Set TX_PIN to INPUT in order to save power.
    sei();                 // Enable interrupts again, go to sleep and wait for intterrupt.
    go_to_sleep();

    // Wakes up here.
    pinMode(pinTx, OUTPUT);
    if (digitalRead(PB1))
        nexaCtrl.DeviceOn(CONTROLLER_ID, DEVICE_1);
    cmd = CMD_NONE;
}

void go_to_sleep()
{
    power_all_disable(); // power off ADC, Timer 0 and 1, serial interface
    sleep_enable();
    sleep_cpu();
    sleep_disable();
    power_all_enable(); // power everything back on
}
