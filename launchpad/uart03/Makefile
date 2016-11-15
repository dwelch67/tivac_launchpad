
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart03.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart03.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart03.c -o uart03.o

uart03.bin : memmap vectors.o uart03.o
	$(ARMGNU)-ld -o uart03.elf -T memmap vectors.o uart03.o
	$(ARMGNU)-objdump -D uart03.elf > uart03.list
	$(ARMGNU)-objcopy uart03.elf uart03.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

