BeagleBone Black BBB C++ API
==========
A high performance, memory mapped, C++ API for GPIO on the Beaglebone Black.
The code was forked from https://github.com/piranha32/IOoo and stripped down to bare essentials for BBB. 


Setup
========
The following setup applies to the Angstrom Distribution. Setup on other Linux distributions would be very similar. 

Setup Environment Variables. 
Two environment variables need configuration to support the device tree overlay, which handles pin multiplexing. 

export SLOTS=/sys/devices/bone_capemgr.8/slots
export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins

Double check that these directories exist. You may have to make changes to the capemgr version. Use the directories you find on your system. 

Device Tree Overlay
The DTS file needs to be complied and placed into the /lib/firmware directory. For details on device tree overlays, see the great tutorial on the site listed below. 

Setup
Once your environment variable are setup, device tree overlay compiled, running the setup script will enable the GPIO configuration and you're ready to execute code. 

Development
========
- Analog input using mmap
- Fork SPI and I2C functionality


Useful Links
========

1. https://github.com/piranha32/IOoo

2. http://derekmolloy.ie/tag/beaglebone-black/
