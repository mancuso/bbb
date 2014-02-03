#ifndef BEAGLEGOO_H_
#define BEAGLEGOO_H_

#include "BeagleGPIO.h"
#include <stdint.h>

class BeaglePins;

class BeagleGPIO 
{
	protected:
		friend class BeaglePins;
        
		struct GPIOInfo
		{
				char *name;
				int gpioNum;
				int bitNum;
				int refCounter;
				int flags;
		};

		bool active;
		static struct GPIOInfo gpioInfos[];
		static uint16_t addrs[];
		int gpioFd;
		uint32_t *gpios[4];
		static const uint32_t gpioAddrs[];
		struct GPIOInfo *_findGpio(char *name);
		static const int MaxGpioNameLen = 32;
		static const int GpioMemBlockLength = 0xfff;

	BeagleGPIO();
    
	public:
		static BeagleGPIO *getInstance();
			enum gpioWriteSemantics
		{
			//pin write semantics
			gpioWrite = 1, //!< gpioWrite - Simple write to the port. Prone to race conditions, offers no multi-process safety.
						   //!< State of the port can be affected by writes to the pins on the same GPIO port.
			gpioWriteAtomic, //!< gpioWriteAtomic - Atomic write to the port. Write to the port must be guaranteed to be successful and effective.
			gpioWriteSetBeforeClear, //!< gpioWriteSetBeforeClear - In two-step implementation of writing to the pins,
									 //!< pins with value '1' are set before pins with value '0' are cleared.
									 //!< For a short period of time the state of the pins in the GPIO block will
									 //!< be equal to bitwise OR of the previous and next states.
			gpioWriteClearBeforeSet //!< gpioWriteClearBeforeSet - In two-step implementation of writing to the pins,
									//!< pins with value '0' are cleared before pins with value '1' are set.
									//!< For a short period of time the state of the pins in the GPIO block will
									//!< be equal to bitwise AND of the previous and next states.
		};
		
		enum gpioFlags
		{
			gpioFlagsNone = 0, //!< gpioFlagsNone - No flags
			gpioExclusive = 1 //!< gpioExclusive - GPIOs allocated exclusively. Allocating with this flag disables sharing with other blocks.
		};


		virtual ~BeagleGPIO();

		virtual BeaglePins *claim(char *names[], int num,
				gpioWriteSemantics semantics, gpioFlags flags = gpioFlagsNone);
                
		void release(BeaglePins **gpio);
        
		virtual BeaglePins *claim(char *names[], int num)
		{
			return claim(names, num, gpioWrite, gpioFlagsNone);
		};
		
};

#endif /* BEAGLEGOO_H_ */
