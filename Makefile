CROSS=/usr/bin/arm-angstrom-linux-gnueabi-

all:
	make -C src CROSS=$(CROSS) 
	make -C examples CROSS=$(CROSS) 
	make -C dts CROSS=$(CROSS)

clean:
	make -C src clean
	make -C examples clean
	make -C dts clean
