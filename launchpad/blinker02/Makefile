
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : blinker02.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c blinker02.c -o blinker02.o

blinker02.bin : memmap vectors.o blinker02.o
	$(ARMGNU)-ld -o blinker02.elf -T memmap vectors.o blinker02.o
	$(ARMGNU)-objdump -D blinker02.elf > blinker02.list
	$(ARMGNU)-objcopy blinker02.elf blinker02.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

