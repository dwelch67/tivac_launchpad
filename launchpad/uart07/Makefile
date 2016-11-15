
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

gcc : uart07.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart07.o : uart07.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart07.c -o uart07.o

uart07.bin : memmap vectors.o uart07.o
	$(ARMGNU)-ld -o uart07.elf -T memmap vectors.o uart07.o
	$(ARMGNU)-objdump -D uart07.elf > uart07.list
	$(ARMGNU)-objcopy uart07.elf uart07.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

