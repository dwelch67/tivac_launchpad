
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : blinker03.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker03.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c blinker03.c -o blinker03.o

blinker03.bin : memmap vectors.o blinker03.o
	$(ARMGNU)-ld -o blinker03.elf -T memmap vectors.o blinker03.o
	$(ARMGNU)-objdump -D blinker03.elf > blinker03.list
	$(ARMGNU)-objcopy blinker03.elf blinker03.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

