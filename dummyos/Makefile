TOOLCHAIN = /opt/ndless/ndless-sdk/toolchain/install/bin
ARM_GCC = $(TOOLCHAIN)/arm-none-eabi-gcc
ARM_LD = $(TOOLCHAIN)/arm-none-eabi-ld
ARM_OBJCOPY = $(TOOLCHAIN)/arm-none-eabi-objcopy

CFLAGS = -O2 -mthumb-interwork -mthumb -marm -march=armv5te -mtune=arm926ej-s
LDFLAGS = 

all: imgvieweros_demo.compressed.img.tns imgvieweros_demo.raw.img.tns nsbar.exe

nsbar.exe : tools/nsbar.cc
	gcc -o $@ $<

%.o: %.S
	$(ARM_GCC) $(CFLAGS) -c $^ -o $@

%.o: %.c
	$(ARM_GCC) $(CFLAGS) -c $^ -o $@

dummyos.elf: main.o n2DLib.o libc.o control.o
	$(ARM_LD) $(LDFLAGS)  $^ -o $@ -Tldscript

imgvieweros_demo.raw.img.tns: dummyos.elf
	$(ARM_OBJCOPY) -O binary $^ $@
	
imgvieweros_demo.compressed.img.tns: nsbar.exe dummyos.raw.img.tns
	./$^ $@

clean:
	rm -f dummyos.raw.img.tns dummyos.compressed.img.tns dummyos.elf *.o
	rm -f nsbar.o nsbar.exe

.PHONY: clean
