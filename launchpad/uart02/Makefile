
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart02.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart02.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart02.c -o uart02.o

uart02.bin : memmap vectors.o uart02.o
	$(ARMGNU)-ld -o uart02.elf -T memmap vectors.o uart02.o
	$(ARMGNU)-objdump -D uart02.elf > uart02.list
	$(ARMGNU)-objcopy uart02.elf uart02.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

