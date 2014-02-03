#!/bin/bash

slots=/sys/devices/bone_capemgr.*/slots

dtbs="gpio_leds"
for a in $dtbs; do
  echo $a > $slots
done

echo ''
echo 'Device tree overlays applied:'
echo ''
for a in $dtbs; do
	echo $a 
done


#workaround for a bug in bone-pinmux-helper to enable GPIO modules.
#based on the code from Luigi Rinaldi: https://groups.google.com/forum/#!msg/beagleboard/OYFp4EXawiI/Mq6s3sg14HoJ

echo ''
echo 'Apply patch for bone-mixmux-helper bug'
echo ''
EXPORT=/sys/class/gpio/export
echo 5 > /sys/class/gpio/export
echo 65 > /sys/class/gpio/export
echo 105 > /sys/class/gpio/export

exit 0