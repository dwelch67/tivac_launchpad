
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart01.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart01.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart01.c -o uart01.o

uart01.bin : memmap vectors.o uart01.o
	$(ARMGNU)-ld -o uart01.elf -T memmap vectors.o uart01.o
	$(ARMGNU)-objdump -D uart01.elf > uart01.list
	$(ARMGNU)-objcopy uart01.elf uart01.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

