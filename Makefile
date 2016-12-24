.PHONY: all install test clean

all: out/x86/sendevents out/arm/sendevents

out/x86:
	mkdir -p out/x86

out/arm:
	mkdir -p out/arm

out/x86/sendevents: src/sendevents.c out/x86
	gcc -Wall -static -O3 $< -o $@

out/arm/sendevents: src/sendevents.c out/arm
	arm-linux-gnueabi-gcc-5 -Wall -static -O3 $< -o $@

install:
	adb push out/$(shell \
		adb shell cat /system/build.prop | \
		grep ro.product.cpu.abi | \
		awk -F, '{gsub("\x3d", ",", $$0); print $$2}')/sendevents /data/local/tmp

test:
	@out/x86/sendevents -v
	@out/x86/sendevents test/dummy test/event.log

clean:
	rm -rf out test/dummy

