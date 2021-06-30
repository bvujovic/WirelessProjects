/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: https://github.com/nRF24/RF24
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println(" *** Recv Started ***");
}

int i = 0;

void loop()
{
  if (radio.available())
  {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    if (*text != 0)
    {
      //T Serial.print(strlen(text));
      Serial.print(i++);
      Serial.print('\t');
      Serial.println(text);
    }
  }
}