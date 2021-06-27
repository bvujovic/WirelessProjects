// https://www.electrodragon.com/w/Si4432

#include <SPI.h>
#include <RH_RF22.h>

RH_RF22 rf22;

void setup()
{
    Serial.begin(9600);
    if (!rf22.init())
        Serial.println(" *** init failed! ***");
}

void loop()
{
    Serial.println('.');
    delay(1000);
}