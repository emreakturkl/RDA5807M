# ----------------------------------------------------------------------

PORT     = /dev/ttyUSB0
BAUDRATE = 57600
CPU      = atmega328p
CLOCK    = 16000000L
PLATFORM = arduino

# ----------------------------------------------------------------------

CC         = avr-gcc
CC+        = avr-g++ 

CFLAGS     = -Wall -Os -std=gnu11   -DF_CPU=$(CLOCK) -mmcu=$(CPU) -ffunction-sections -fdata-sections -flto
CFLAGS+    = -Wall -Os -std=gnu++11 -DF_CPU=$(CLOCK) -mmcu=$(CPU) -ffunction-sections -fdata-sections -flto
 
COMPILE     = ${CC} ${CFLAGS}
COMPILE+    = ${CC+} ${CFLAGS+}
PROGRAMMER  = -c ${PLATFORM} -P ${PORT} -b ${BAUDRATE}
AVRDUDE     = avrdude $(PROGRAMMER) -p $(CPU)

# ----------------------------------------------------------------------

SOURCE_DIR = .
PATH_      = $(PWD)
OBJECT_DIR = obj/
BIN        = bin/

ARDUINO_DIR                  = $(HOME)/apps/arduino-1.8.12/
ARDUINO_CORE_DIR             = $(ARDUINO_DIR)hardware/arduino/avr/cores/arduino/
LIBRARY_DIR                  = $(ARDUINO_DIR)hardware/arduino/avr/libraries/
INCLUDE_LIB                  = -I$(ARDUINO_CORE_DIR)
INCLUDE_PIN_STANDART         = -I$(ARDUINO_DIR)hardware/arduino/avr/variants/standard/
INCLUDE_PIN_EIGHTANALOGINPUT = -I$(ARDUINO_DIR)hardware/arduino/avr/variants/eightanaloginputs/

WIRE                         = $(LIBRARY_DIR)Wire/src/
SPI                          = $(LIBRARY_DIR)SPI/src/
SOFTWARE_SERIAL              = $(LIBRARY_DIR)SoftwareSerial/src/
EEPROM                       = $(LIBRARY_DIR)EEPROM/src/

# ----------------------------------------------------------------------

TARGET = main
DRIVER = RDA5807M

EXTERNAL_OBJS = $(OBJECT_DIR)Wire.o \
								$(OBJECT_DIR)twi.o \
								$(OBJECT_DIR)SPI.o \
								$(OBJECT_DIR)SoftwareSerial.o \
								$(OBJECT_DIR)EEPROM.o \

ARDUINO_OBJS = ${patsubst $(ARDUINO_CORE_DIR)%.c, ${OBJECT_DIR}%.c.o, $(wildcard $(ARDUINO_CORE_DIR)*.c)} \
			         ${patsubst $(ARDUINO_CORE_DIR)%.S, ${OBJECT_DIR}%.S.o, $(wildcard $(ARDUINO_CORE_DIR)*.S)} \
			         ${patsubst $(ARDUINO_CORE_DIR)%.cpp, ${OBJECT_DIR}%.cpp.o, $(filter-out $(ARDUINO_CORE_DIR)main.cpp, $(wildcard $(ARDUINO_CORE_DIR)*.cpp))} \

#$(info $(x))


$(BIN)${TARGET}.hex: $(BIN)${TARGET}.elf
	rm -f $(BIN)${TARGET}.hex
	avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 -O ihex $^ $(BIN)${TARGET}.eep
	avr-objcopy -R .eeprom -O ihex $^ $@
	avr-size -A $^

$(BIN)${TARGET}.elf: $(OBJECT_DIR)${TARGET}.o $(OBJECT_DIR)${DRIVER}.o libcore.a 
	if ! [ -d "$(PATH_)/$(BIN)" ]; then mkdir $(BIN); fi;
	${COMPILE} -Wl,--gc-sections -lm -o $@ $^

$(OBJECT_DIR)${TARGET}.o: ${TARGET}.cpp
	if ! [ -d "$(PATH_)/$(OBJECT_DIR)" ]; then mkdir $(OBJECT_DIR); fi;
	${COMPILE+} ${INCLUDE_LIB} ${INCLUDE_PIN_STANDART} -I$(WIRE) -I$(SPI) -I$(SOFTWARE_SERIAL) -I$(EEPROM) -c $^ -o $@

$(OBJECT_DIR)${DRIVER}.o: ${DRIVER}.cpp
	${COMPILE+} ${INCLUDE_LIB} ${INCLUDE_PIN_STANDART} -I$(WIRE) -c $^ -o $@

libcore.a: ${ARDUINO_OBJS} ${EXTERNAL_OBJS}
	avr-gcc-ar rcs $@ $^ 

# EXTERNAL LIBRARY

$(OBJECT_DIR)Wire.o: $(WIRE)Wire.cpp 
	${COMPILE+} ${INCLUDE_LIB} ${INCLUDE_PIN_EIGHTANALOGINPUT} -c $^ -o $@

$(OBJECT_DIR)twi.o: $(WIRE)utility/twi.c 
	${COMPILE} ${INCLUDE_LIB} ${INCLUDE_PIN_EIGHTANALOGINPUT} -c $^ -o $@

$(OBJECT_DIR)SPI.o: $(SPI)SPI.cpp
	${COMPILE+} ${INCLUDE_LIB} ${INCLUDE_PIN_EIGHTANALOGINPUT} -c $^ -o $@

$(OBJECT_DIR)SoftwareSerial.o: $(SOFTWARE_SERIAL)SoftwareSerial.cpp 
	${COMPILE+} ${INCLUDE_LIB} ${INCLUDE_PIN_EIGHTANALOGINPUT} -I$(SOFTWARE_SERIAL) -c $^ -o $@

$(OBJECT_DIR)EEPROM.o: $(EEPROM)EEPROM.h
	${COMPILE+} ${INCLUDE_LIB} ${INCLUDE_PIN_EIGHTANALOGINPUT} -c $^ -o $@

# ARDUINO CORES
$(OBJECT_DIR)%.c.o: ${ARDUINO_CORE_DIR}%.c
	${COMPILE} ${INCLUDE_LIB} ${INCLUDE_PIN_STANDART} -c $< -o $@

$(OBJECT_DIR)%.cpp.o: ${ARDUINO_CORE_DIR}%.cpp
	${COMPILE+} ${INCLUDE_LIB} ${INCLUDE_PIN_STANDART} -c $< -o $@

$(OBJECT_DIR)%.S.o: ${ARDUINO_CORE_DIR}%.S
	${COMPILE} ${INCLUDE_LIB} ${INCLUDE_PIN_STANDART} -c -x assembler-with-cpp $< -o $@

load:
	make
	${AVRDUDE} -U flash:w:$(BIN)${TARGET}.hex:i

clean:
	rm -f libcore.a
	rm -f $(BIN)${TARGET}.elf $(BIN)${TARGET}.eep
	rm -f $(OBJECT_DIR)*.o $(OBJECT_DIR)*.d 