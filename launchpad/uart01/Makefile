
ARMGNU?=arm-none-eabi
#ARMGNU=arm-none-linux-gnueabi
#ARMGNU=arm-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings


gcc : uart01.gcc.thumb.bin uart01.gcc.thumb2.bin 

all : gcc

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart01.gcc.thumb.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart01.c -o uart01.gcc.thumb.o

uart01.gcc.thumb2.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart01.c -o uart01.gcc.thumb2.o

uart01.gcc.thumb.bin : memmap vectors.o uart01.gcc.thumb.o
	$(ARMGNU)-ld -o uart01.gcc.thumb.elf -T memmap vectors.o uart01.gcc.thumb.o
	$(ARMGNU)-objdump -D uart01.gcc.thumb.elf > uart01.gcc.thumb.list
	$(ARMGNU)-objcopy uart01.gcc.thumb.elf uart01.gcc.thumb.bin -O binary

uart01.gcc.thumb2.bin : memmap vectors.o uart01.gcc.thumb2.o
	$(ARMGNU)-ld -o uart01.gcc.thumb2.elf -T memmap vectors.o uart01.gcc.thumb2.o
	$(ARMGNU)-objdump -D uart01.gcc.thumb2.elf > uart01.gcc.thumb2.list
	$(ARMGNU)-objcopy uart01.gcc.thumb2.elf uart01.gcc.thumb2.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

