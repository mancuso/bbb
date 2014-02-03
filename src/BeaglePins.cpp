#include "BeaglePins.h"
#include <string.h>
#include <stdio.h>
#include "debug.h"

#define DATA_OUT_REG    0x13C
#define DATA_IN_REG     0x138
#define GPIO_OE_REG     0x134
#define DATA_CLEAR_REG  0x190
#define DATA_SET_REG    0x194

BeaglePins::BeaglePins(int num, BeagleGPIO::gpioWriteSemantics semantics,
		BeagleGPIO *parent):BeaglePins()
{
	this->parent = parent;
	this->num = num;
	pins = new BeagleGPIO::GPIOInfo *[num];
	localNames = new char *[num];
	ports = new uint32_t[num];
	masks = new uint32_t[num];
	writeSemantics = semantics;
	current = 0;

	for(int i=0;i<num;i++)
	{
		ports[i]=0;
		masks[i]=0;
		pins[i]=NULL;
	}
	debug(2,"BeaglePins::BeaglePins(): done\n");
}

BeaglePins::~BeaglePins()
{
	active = false;
	for (int i = 0; i < current; i++)
	{
		//decrease reference count on pin being released
		pins[i]->refCounter--;
		//sanity check
		if (pins[i]->refCounter < 0)
			debug(0,
					"BeaglePins::~BeaglePins(): reference count on pin '%s' is lower than zero\n",
					pins[i]->name);
		//clear gpioExclusive pin if present
		if (pins[i]->flags & BeagleGPIO::gpioExclusive)
			pins[i]->flags &= ~BeagleGPIO::gpioExclusive;
		//if has been allocated, delete local storage for local pin name
		if (localNames[i] != pins[i]->name)
			delete[] localNames[i];
	}

	//free memory allocated for arrays
	delete[] ports;
	delete[] masks;
	delete[] localNames;
	delete[] pins;
}

int BeaglePins::addPin(BeagleGPIO::GPIOInfo *pin)
{
	if (current >= num)
	{
		debug(2,"BeaglePins::addPin(): current >= num\n");
		return -1;
	}

	pins[current] = pin;
	ports[current] = pin->gpioNum;
	masks[current] = 1 << (pin->bitNum);
	localNames[current] = pin->name;
	pin->refCounter++;
	debug(2,
			"BeaglePins::addPin(): current=%i, pin->gpioNum=%i, pin->bitNum=%i, masks=%08x, localNames=\"%s\"\n",
			current, pin->gpioNum, pin->bitNum, masks[current],
			localNames[current]);
	current++;
	active=true;
	return 0;
}

void BeaglePins::namePin(int i, char* name)
{
	if (i >= current)
		return;
	//if localNames[i]==pins[i]->name then local name is just a reference to the system pin name.
	//If so, local storage has to be allocated.
	debug(2,"Naming pin %i as \"%s\" (formerly \"%s\")\n",i,name,localNames[i]);
	if (localNames[i] == pins[i]->name)
		localNames[i] = new char[BeagleGPIO::MaxGpioNameLen + 1];
	for(int j=0;j<BeagleGPIO::MaxGpioNameLen + 1;j++)
		localNames[i][j]=0;
	//memset(localNames[i], 0, BeagleGPIO::MaxGpioNameLen + 1);
	strncpy(localNames[i],name,BeagleGPIO::MaxGpioNameLen);
}

void BeaglePins::namePins(char *names[])
{
	for (int i = 0; i < current; i++)
		namePin(i, names[i]);
}

int BeaglePins::findPinIndex(char* name)
{
	for (int i = 0; i < current; i++)
		if (strncmp(localNames[i], name, BeagleGPIO::MaxGpioNameLen) == 0)
			return i;
	debug(0,"BeaglePins::findPinIndex(): Index for pin %s not found\n", name);
	return -1;
}

void BeaglePins::enableOutput(bool enable)
{
	for (int i = 0; i < current; i++)
		enableOutput(i, enable);
}

void BeaglePins::enableOutput(int i, bool enable)
{
	debug(2,"BeaglePins::enableOutput(): i=%i, enable=%i\n", i, enable);
	if (i < 0 || i >= current)
	{
		debug(1,"BeaglePins::enableOutput(): Index %i out of range\n", i);
		return;
	}
	debug(2,
			"BeaglePins::enableOutput(): enabling pin %i (%s): port=%i, mask=%08x, OE_REG=%08x\n",
			i, localNames[i], ports[i], masks[i],
			parent->gpios[ports[i]][GPIO_OE_REG / 4]);
	if (enable)
		parent->gpios[ports[i]][GPIO_OE_REG / 4] &= ~masks[i];
	else
		parent->gpios[ports[i]][GPIO_OE_REG / 4] |= masks[i];
	debug(2,"BeaglePins::enableOutput(): port=%i, mask=%08x, OE_REG=%08x\n",
			ports[i], masks[i], parent->gpios[ports[i]][GPIO_OE_REG / 4]);
}

void BeaglePins::enableOutput(int* outs, int num)
{
	debug(2,"BeaglePins::enableOutput(arr): enabling %i pins\n", num);
	if (outs == NULL || num <= 0 || num>current )
	{
		debug(0,"BeaglePins::enableOutput(): fail (current=%i, num=%i)\n",
				current, num);
		return;
	}
	bool *oe=new bool[current];
	for (int i = 0; i < current; i++)
		oe[i] = false;
	for (int i = 0; i < num; i++)
	{
		if (outs[i] < 0 || outs[i] > current)
			continue;
		oe[outs[i]] = true;
	}
	for (int i = 0; i < current; i++)
		enableOutput(i, oe[i]);
	delete[] oe;
}

void BeaglePins::enableOutput(char** outNames, int num)
{
	debug(2,"BeaglePins::enableOutput(names): enabling %i named pins\n", num);
	if (outNames == NULL || num <= 0 || num >= current)
	{
		debug(0,"BeaglePins::enableOutput(): fail\n");
		return;
	}
	bool *oe=new bool[current];
	for (int i = 0; i < current; i++)
		oe[i] = false;
	for (int i = 0; i < num; i++)
	{
		if (outNames[i] == NULL)
			continue;
		int out = findPinIndex(outNames[i]);
		oe[out] = true;
	}
	for (int i = 0; i < current; i++)
		enableOutput(i, oe[i]);
	delete[] oe;
}

void BeaglePins::write(uint32_t v)
{
	debug(3,"BeaglePins::write(): writing %i\n",v);
	switch (writeSemantics)
	{
		case BeagleGPIO::gpioWrite:
		{ //Braces are necessary to limit the visibility scope of the variables.
			uint32_t portBitmasks[4] =
				{ 0, 0, 0, 0 };
			uint32_t valueBitmasks[4] =
				{ 0, 0, 0, 0 };

			uint32_t m = 1;
			for (int i = 0; i < current; i++)
			{
				portBitmasks[ports[i]] |= masks[i];
				if (v & m)
					valueBitmasks[ports[i]] |= masks[i];
				m = m << 1;
			}
			for (int i = 0; i < 4; i++)
			{
				if (portBitmasks[i] == 0)
					continue;
				uint32_t tmp = parent->gpios[i][DATA_OUT_REG / 4];
				tmp &= ~portBitmasks[i];
				tmp |= valueBitmasks[i];
				parent->gpios[i][DATA_OUT_REG / 4] = tmp;
			}
			break;
		}
		case BeagleGPIO::gpioWriteSetBeforeClear:
		case BeagleGPIO::gpioWriteClearBeforeSet:
		{
			uint32_t setBits[4] =
				{ 0, 0, 0, 0 };
			uint32_t clearBits[4] =
				{ 0, 0, 0, 0 };
			uint32_t m = 1;
			for (int i = 0; i < current; i++)
			{
				if (v & m)
					setBits[ports[i]] |= masks[i];
				else
					clearBits[ports[i]] |= masks[i];
				m = m << 1;
			}
			if (writeSemantics == BeagleGPIO::gpioWriteSetBeforeClear)
			{
				for (int i = 0; i < 4; i++)
				{
					parent->gpios[i][DATA_SET_REG / 4] = setBits[i];
					parent->gpios[i][DATA_CLEAR_REG / 4] = clearBits[i];
				}
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					parent->gpios[i][DATA_CLEAR_REG / 4] = clearBits[i];
					parent->gpios[i][DATA_SET_REG / 4] = setBits[i];
				}
			}
			break;
		}

		case BeagleGPIO::gpioWriteAtomic: //atomic write is not supported on Beagle.
		default:
			debug(0,"BeaglePins::write(): Incorrect semantics\n");
			//do nothing
			break;
	}
}

void BeaglePins::setBit(int bit)
{
	if (bit < 0 || bit >= current)
		return;
	parent->gpios[ports[bit]][DATA_SET_REG/4]=masks[bit];
}

void BeaglePins::set(uint32_t v)
{
	uint32_t setBits[4] =
		{ 0, 0, 0, 0 };
	uint32_t m = 1;
	for (int i = 0; i < current; i++)
	{
		if (v & m)
			setBits[ports[i]] |= masks[i];
		m = m << 1;
	}
	for (int i = 0; i < 4; i++)
		parent->gpios[i][DATA_SET_REG / 4] = setBits[i];

}

void BeaglePins::clearBit(int bit)
{
	if (bit < 0 || bit >= current)
		return;
	parent->gpios[ports[bit]][DATA_CLEAR_REG/4]=masks[bit];
}

void BeaglePins::clear(uint32_t v)
{
	uint32_t clearBits[4] =
		{ 0, 0, 0, 0 };
	uint32_t m = 1;
	for (int i = 0; i < current; i++)
	{
		if (!(v & m))
			clearBits[ports[i]] |= masks[i];
		m = m << 1;
	}
	for (int i = 0; i < 4; i++)
		parent->gpios[i][DATA_CLEAR_REG / 4] = clearBits[i];
}

uint32_t BeaglePins::read()
{
	uint32_t tmp[4];
	for (int i = 0; i < 4; i++)
		tmp[i] = parent->gpios[i][DATA_IN_REG / 4];

	uint32_t r = 0;
	uint32_t m = 1;
	for (int i = 0; i < current; i++)
	{
		if (tmp[ports[i]] & masks[i])
			r |= m;
		m = m << 1;
	}
	return r;
}
