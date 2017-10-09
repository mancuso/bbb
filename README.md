## BeagleBone Black BBB C++ API

I started this project to fulfill a need for a high-speed digital GPIO API on BeagleBone Black. At the time, existing GPIO APIs like [BoneScript](http://beagleboard.org/Support/BoneScript) and [Adafruit-BeagleBone-IO-Python](https://github.com/adafruit/adafruit-beaglebone-io-python) were very useful libraries that let developers write GPIO applications quickly and efficiently. However, this convenience came at a cost. One less than ideal feature of those libraries is the manner in which they handle GPIO - using the ```/sys``` directory. This meant that GPIO operations were executed via the Linux file system, which greatly limits the speed of GPIO operations. The more direct and subsequently much faster method of handling GPIO operations is to read and write values from system memory, using the Linux ```mmap``` function. The catch is that this method requires a little more work to manually configure the memory mapping via GPIO pin multiplexing.


I forked a [universal framework](https://github.com/piranha32/IOoo) that featured a good example of memory mapped GPIO on BeagleBone Black, extracted the memory mapping layer and combined it with manual configuration of system memory using device tree overlays for pin multiplexing. The resulting project featured here is a good foundation for memory mapped high speed digital using GPIO on Beaglbone Black.  Although it is more work than a ```/sys``` GPIO solution and comes with a higher learning curve, the result in some cases is a [thousand-fold increase in performance](http://hackaday.com/2013/12/07/speeding-up-beaglebone-black-gpio-a-thousand-times). Applications that require high frequency GPIO operations will benefit greatly from using the BBB C++ library.

### Setup  

The following setup applies to the Angstrom Distribution. Setup on other Linux distributions would be very similar, just find the appropriate directories and compiler locations.

##### Setup Environment Variables. 
On your Linux system, two environment variables require setup and configuration to support the device tree overlay, which handles pin multiplexing. This can be accomplished by executing the following commands in the shell:

```export SLOTS=/sys/devices/bone_capemgr.8/slots```

```export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins```

Placing the above lines in the .profile file in your home directory will execute them automatically upon login.

Double check that these directories exist. You may have to make changes to match the capemgr version you are running. Use the directories you find on your system. 

##### Device Tree Overlay
The DTS file needs to be complied and placed into

    /lib/firmware

For details on device tree overlays, see the great tutorial on the [Derek Molloy's Web site](http://derekmolloy.ie/tag/beaglebone-black). 

##### Running the Setup Script
Once your environment variables are setup, device tree overlay compiled, running the setup script will enable the GPIO configuration and you're ready to execute code. 


### Useful Links


1. https://github.com/piranha32/IOoo

2. http://derekmolloy.ie/tag/beaglebone-black
