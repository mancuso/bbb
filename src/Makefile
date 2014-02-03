DEBUG_LEVEL=-0
CPPFLAGS=-I../include/  -D_HW_PLATFORM_BEAGLEBONE -DDEBUG_LEVEL=$(DEBUG_LEVEL)
CXXFLAGS=-O0 -g -pedantic -Wall -std=gnu++11
CFLAGS=-pedantic -Wall 
CXX=${CROSS}g++ 
CC=${CROSS}gcc
LD=${CROSS}g++ 
LDFLAGS=-Llib
AR=${CROSS}ar
ARFLAGS=rvs
vpath %.h ../include 
vpath %.cpp src 
vpath %.c src 

LIB_SRC= BeagleGPIO.cpp BeaglePins.cpp
LIB_OBJS=$(patsubst %.cpp,%.o,$(LIB_SRC)) 

../lib/libgpio.a:$(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	



%.o : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f ../bin/*.o
	rm -f ../lib/libgpio.a
    

