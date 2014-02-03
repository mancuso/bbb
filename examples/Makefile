CPPFLAGS=-I../include/  -D_HW_PLATFORM_BEAGLEBONE #-DDEBUG=0
CXXFLAGS=-O0 -g -pedantic -Wall -std=gnu++11
CFLAGS=-pedantic -Wall 
CXX=${CROSS}g++ 
CC=${CROSS}gcc
LD=${CROSS}g++ 
LDFLAGS=-L../lib
MAKEDEPEND = $(CXX) -M $(CPPFLAGS) -o $(df).d $<
PRULIBS=-lprussdrv -lpthread
LDLIBS=-lgpio

vpath %.h include 

default: ../bin/gpio_leds 

%.o : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


../bin/gpio_leds: gpio_leds.o TestGPIOLeds.o
	${LD} $(LDFLAGS) $^ -o $@ $(LDLIBS)



clean:
	rm -f ../bin/gpio_leds *.o

