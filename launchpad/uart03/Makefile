
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart03.thumb.bin uart03.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart03.thumb.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart03.c -o uart03.thumb.o

uart03.thumb2.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart03.c -o uart03.thumb2.o

uart03.thumb.bin : memmap vectors.o uart03.thumb.o
	$(ARMGNU)-ld -o uart03.thumb.elf -T memmap vectors.o uart03.thumb.o
	$(ARMGNU)-objdump -D uart03.thumb.elf > uart03.thumb.list
	$(ARMGNU)-objcopy uart03.thumb.elf uart03.thumb.bin -O binary

uart03.thumb2.bin : memmap vectors.o uart03.thumb2.o
	$(ARMGNU)-ld -o uart03.thumb2.elf -T memmap vectors.o uart03.thumb2.o
	$(ARMGNU)-objdump -D uart03.thumb2.elf > uart03.thumb2.list
	$(ARMGNU)-objcopy uart03.thumb2.elf uart03.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

