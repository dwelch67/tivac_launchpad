
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart06.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart06.o : uart06.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart06.c -o uart06.o

uart06.bin : memmap vectors.o uart06.o
	$(ARMGNU)-ld -o uart06.elf -T memmap vectors.o uart06.o
	$(ARMGNU)-objdump -D uart06.elf > uart06.list
	$(ARMGNU)-objcopy uart06.elf uart06.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

