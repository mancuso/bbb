##BeagleBone Black BBB C++ API

The code was forked from [a more generic API project] (https://github.com/piranha32/IOoo) and modifed to run exclusively on BeagleBone Black, resulting in a high performance, memory mapped C++ API. [BoneScript] (http://beagleboard.org/Support/BoneScript) and [Adafruit-BeagleBone-IO-Python] (https://github.com/adafruit/adafruit-beaglebone-io-python) are very useful libraries that let developers write applications quickly and efficiently. One less than ideal feature of those libraries is the manner in which they handle GPIO - using the /sys directory. This means that GPIO operations are executed via the Linux file system, which greatly limits the speed of GPIO operations. The BBB C++ API uses the Linux mmap function to execute GPIO operations, where values are  read and written directly from system memory. The result is in some cases a [thousand-fold increase in performance](http://hackaday.com/2013/12/07/speeding-up-beaglebone-black-gpio-a-thousand-times). Applications that require high frequency GPIO operations will benefit greatly from using the BBB C++ library.

###Setup

The following setup applies to the Angstrom Distribution. Setup on other Linux distributions would be very similar. 

#####Setup Environment Variables. 
On your Linux system, two environment variables require setup and configuration to support the device tree overlay, which handles pin multiplexing. This can be accomplished by executing the following commands in the shell:

```export SLOTS=/sys/devices/bone_capemgr.8/slots```

```export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins```

Placing the above lines in the .profile file in your home directory will execute them automatically upon login.

Double check that these directories exist. You may have to make changes to the capemgr version you are running. Use the directories you find on your system. 

#####Device Tree Overlay
The DTS file needs to be complied and placed into

    /lib/firmware

For details on device tree overlays, see the great tutorial on the site listed below. 

#####Running the Setup Script
Once your environment variable are setup, device tree overlay compiled, running the setup script will enable the GPIO configuration and you're ready to execute code. 



###Development

Developing high performance analong input is the highest priority, but also the most challenging to tackle, because of ongoing changes to the Linux kernel wrt ARM and how analog input is processed. I will probably wait until the dust settles before investing time in a solid analog API for the onboard ADCs. Development plans are as follows

- Analog input using memory maped I/O
- Integrate SPI and I2C functionality
- More documentation and example code

###Useful Links


1. https://github.com/piranha32/IOoo

2. http://derekmolloy.ie/tag/beaglebone-black
