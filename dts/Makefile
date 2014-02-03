DTS= gpio_leds-00A0.dts 
DTBs=$(patsubst %.dts,../bin/%.dtbo,$(DTS))

../bin/%.dtbo: %.dts
	dtc -O dtb -o $@ -b 0 -@ $<

all: $(DTBs)

clean:
	rm -f  $(DTBs)

