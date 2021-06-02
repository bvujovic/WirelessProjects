# NexaCtrlTest
Sending and receiving radio signal via STX882/SRX882 devices and NexaCtrl library.
Sender: sleeping ATtiny85 that wakes up on PIR signal.

## NexaSend
- send 'device on' signal, 1sec delay
- send 'device off' signal, 4sec delay
- repeat...

## NexaCtrlATtinyPIR
- PIR wakes up ATtiny85
- tiny sends 'device on' signal
- goes to sleep

![Sender and Receiver test devices](https://github.com/bvujovic/WirelessProjects/blob/main/projects/NexaCtrlTest/.github/test_send_recv.jpg)