#ifndef BEAGLEGOOP_H_
#define BEAGLEGOOP_H_

#include "BeagleGPIO.h"
#include <stdint.h>

class BeaglePins
{

	protected:
		friend class GPIO;
		bool active;

		BeaglePins(){active = false;};
        
	private:
		friend class BeagleGPIO;
		friend class TestGPIOLeds;

		BeagleGPIO *parent;
		char **localNames;
		BeagleGPIO::GPIOInfo **pins;
		uint32_t *ports;
		uint32_t *masks;
		int num;
		int current;
		BeagleGPIO::gpioWriteSemantics writeSemantics;
        
		int addPin(BeagleGPIO::GPIOInfo *pin);
        
		BeaglePins(int num, BeagleGPIO::gpioWriteSemantics semantics,
				BeagleGPIO *parent);
                
		virtual ~BeaglePins();
        
	public:

		virtual void namePin(int i, char *name);
		virtual void namePins(char *names[]);
		virtual int findPinIndex(char *name);
		virtual void enableOutput(bool enable);
		virtual void enableOutput(int i, bool enable);
		virtual void enableOutput(int *outs, int num);
		virtual void enableOutput(char **outNames, int num);
		virtual void write(uint32_t v);
		virtual void set(uint32_t v);
		virtual void setBit(int bit);
		virtual void clear(uint32_t v);
		virtual void clearBit(int bit);
		virtual uint32_t read();
        
		bool isValid(){ return active;};
};

#endif /* BEAGLEGOOP_H_ */
