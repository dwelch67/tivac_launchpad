
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart04.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart04.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart04.c -o uart04.o

uart04.bin : memmap vectors.o uart04.o
	$(ARMGNU)-ld -o uart04.elf -T memmap vectors.o uart04.o
	$(ARMGNU)-objdump -D uart04.elf > uart04.list
	$(ARMGNU)-objcopy uart04.elf uart04.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

