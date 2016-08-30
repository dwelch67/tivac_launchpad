
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings


gcc : uart04.gcc.thumb.bin uart04.gcc.thumb2.bin 

all : gcc

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart04.gcc.thumb.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart04.c -o uart04.gcc.thumb.o

uart04.gcc.thumb2.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart04.c -o uart04.gcc.thumb2.o

uart04.gcc.thumb.bin : memmap vectors.o uart04.gcc.thumb.o
	$(ARMGNU)-ld -o uart04.gcc.thumb.elf -T memmap vectors.o uart04.gcc.thumb.o
	$(ARMGNU)-objdump -D uart04.gcc.thumb.elf > uart04.gcc.thumb.list
	$(ARMGNU)-objcopy uart04.gcc.thumb.elf uart04.gcc.thumb.bin -O binary

uart04.gcc.thumb2.bin : memmap vectors.o uart04.gcc.thumb2.o
	$(ARMGNU)-ld -o uart04.gcc.thumb2.elf -T memmap vectors.o uart04.gcc.thumb2.o
	$(ARMGNU)-objdump -D uart04.gcc.thumb2.elf > uart04.gcc.thumb2.list
	$(ARMGNU)-objcopy uart04.gcc.thumb2.elf uart04.gcc.thumb2.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

