# RDA5807M Driver for Arduino

* build -> ```make```
* build and load-> ```make load```
* clean -> ```make clean```

#### Important Variables !

* ```PORT     = /dev/ttyUSB0```
* ```BAUDRATE = 57600```
* ```CPU      = atmega328p```
* ```CLOCK    = 16000000L```
* ```PLATFORM = arduino```
* ```ARDUINO_DIR = $(HOME)/apps/arduino-1.8.12/```

```ARDUINO_DIR``` is absolute path.  Change ```ARDUINO_DIR``` variable to your path.

### Some Functions
* ```int8_t init_rda5807();```
* ```void channel(float freq);```
* ```void volume(uint16_t level);```
* ```void dmute(uint8_t flag);```
* ```void mono(uint8_t flag);```
* ```void bass(uint8_t flag);```