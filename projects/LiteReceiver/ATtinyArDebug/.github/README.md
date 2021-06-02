# ATtinyArDebug
ATtiny85 gives pulse data to Arduino.
ATtiny85 is slave on I2C bus, Arduino is master.

## Sketches
i2c_master - Arduino: gets data from tiny and prints them to the serial monitor.
i2c_slave_simple_test - ATtiny85: simple test. Slave returns some fixed string to master.
PulseInTest/attiny_pulsein - ATtiny85: slave returns last 5 pulses to master on request.

![SRX882, ATtiny85 & Arduino Uno](https://github.com/bvujovic/WirelessProjects/blob/main/projects/LiteReceiver/ATtinyArDebug/ATtinyArDebug.jpg)
