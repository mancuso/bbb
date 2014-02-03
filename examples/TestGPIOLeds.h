#ifndef TESTGPIOLEDS_H_
#define TESTGPIOLEDS_H_

#include "../include/BeagleGPIO.h"
#include "../include/BeaglePins.h"

class TestGPIOLeds
{
	private:
		BeagleGPIO *gp;
		BeaglePins *blockLed;
	public:
		TestGPIOLeds();
		virtual ~TestGPIOLeds();
		virtual void loop()
		{
			loop(-1);
		};
		virtual void loop(int iterations);
};

#endif /* TESTGPIOLEDS_H_ */
