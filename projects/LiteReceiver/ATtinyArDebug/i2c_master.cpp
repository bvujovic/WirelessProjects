//* Arduino dobija podatke od ATtiny-a putem I2C-a i prikazuje ih na ekranu.

#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  Wire.requestFrom(9, 6);

  while (Wire.available())
  {
    int x = Wire.read();
    Serial.print(x);
    Serial.print('\t');
  }
  Serial.println();

  delay(3300);
}
