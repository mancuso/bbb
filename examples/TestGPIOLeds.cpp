#include "TestGPIOLeds.h"
#include <unistd.h>

TestGPIOLeds::TestGPIOLeds()
{
	gp = BeagleGPIO::getInstance();
	const char *ledPins[] =
		{ "P8_7", "P8_8", "P8_9", "P8_10", "P8_11", "P8_12", "P8_13", "P8_14" };
	blockLed = gp->claim((char**) ledPins, 8);

	int ledPinIdxs[] =
		{ 0, 1, 2, 3, 4, 5, 6, 7 };
	blockLed->enableOutput(ledPinIdxs, 8);
}

TestGPIOLeds::~TestGPIOLeds()
{
	delete blockLed;
}

void TestGPIOLeds::loop(int iterations)
{
	uint8_t v = 0;
	int i = 0;
	for (;;)
	{
		blockLed->write(v);
		usleep(100000);
		v = v << 1;
		v |= (((~v) >> 7)) & 0x01;
		i++;
		if (iterations > 0 && (i > iterations))
			break;
	}
}
