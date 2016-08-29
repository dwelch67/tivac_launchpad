
ARMGNU?=arm-none-eabi
#ARMGNU=arm-none-linux-gnueabi
#ARMGNU=arm-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

gcc : blinker02.gcc.thumb.bin blinker02.gcc.thumb2.bin 

all : gcc

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker02.gcc.thumb.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.gcc.thumb.o

blinker02.gcc.thumb2.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c blinker02.c -o blinker02.gcc.thumb2.o

blinker02.gcc.thumb.bin : memmap vectors.o blinker02.gcc.thumb.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb.elf -T memmap vectors.o blinker02.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb.elf > blinker02.gcc.thumb.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb.elf blinker02.gcc.thumb.bin -O binary

blinker02.gcc.thumb2.bin : memmap vectors.o blinker02.gcc.thumb2.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb2.elf -T memmap vectors.o blinker02.gcc.thumb2.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb2.elf > blinker02.gcc.thumb2.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb2.elf blinker02.gcc.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

