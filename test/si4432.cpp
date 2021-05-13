// https://github.com/ADiea/si4432
// https://www.electrodragon.com/w/Si4432
// https://www.youtube.com/watch?v=_vg5eoZ70E8


#include "si4432.h"

#define TX

//* Si4432 radio(7, 6);
Si4432 radio(2);
unsigned long pTime;
//The setup function is called once at startup of the sketch
void setup()
{

    Serial.begin(115200);
    delay(300);
    radio.init();
    radio.setBaudRate(70);
    radio.setFrequency(433);
    radio.readAll();

#ifdef RX
    radio.startListening();
#endif
    pTime = millis();
    // Add your initialization code here
}

// The loop function is called in an endless loop
void loop()
{
    //Add your repeated code here

    byte dummy[70] = {0x42, 0x3, 0x11, 0x13};
#ifdef TX
    byte resLen = 0;
    byte answer[64] = {0};

    bool pkg = radio.sendPacket(8, dummy, true, 70, &resLen, answer);
#endif
#ifdef RX
    bool pkg = radio.isPacketReceived();
#endif
    if (pkg)
    {
#ifdef TX
        Serial.print("PACKET CAME - ");
        Serial.println((int)resLen, DEC);

        for (byte i = 0; i < resLen; ++i)
        {
            Serial.print(answer[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
#endif

#ifdef RX

        byte payLoad[64] = {0};
        byte len = 0;
        radio.getPacketReceived(&len, payLoad);
        Serial.print("PACKET CAME - ");
        Serial.print(len, DEC);
        Serial.print(" - ");
        Serial.println(millis() - pTime, DEC);

        pTime = millis();
        for (byte i = 0; i < len; ++i)
        {
            Serial.print((int)payLoad[i], HEX);
            Serial.print(" ");
        }
        Serial.println(" ");

        Serial.print("Sending response- ");
        while (!radio.sendPacket(50, dummy))
            ;
        Serial.println(" SENT!");

        radio.startListening(); // restart the listening.
#endif
    }
    else
    {
#ifdef RX
        //Serial.println("No packet this cycle");
#endif
    }

    delay(4000);
}