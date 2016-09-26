
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : blinker03.thumb.bin blinker03.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker03.thumb.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c blinker03.c -o blinker03.thumb.o

blinker03.thumb2.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c blinker03.c -o blinker03.thumb2.o

blinker03.thumb.bin : memmap vectors.o blinker03.thumb.o
	$(ARMGNU)-ld -o blinker03.thumb.elf -T memmap vectors.o blinker03.thumb.o
	$(ARMGNU)-objdump -D blinker03.thumb.elf > blinker03.thumb.list
	$(ARMGNU)-objcopy blinker03.thumb.elf blinker03.thumb.bin -O binary

blinker03.thumb2.bin : memmap vectors.o blinker03.thumb2.o
	$(ARMGNU)-ld -o blinker03.thumb2.elf -T memmap vectors.o blinker03.thumb2.o
	$(ARMGNU)-objdump -D blinker03.thumb2.elf > blinker03.thumb2.list
	$(ARMGNU)-objcopy blinker03.thumb2.elf blinker03.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

