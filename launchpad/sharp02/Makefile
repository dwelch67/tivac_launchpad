
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : sharp02.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

sharp02.o : sharp02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c sharp02.c -o sharp02.o

sharp02.bin : memmap vectors.o sharp02.o
	$(ARMGNU)-ld -o sharp02.elf -T memmap vectors.o sharp02.o
	$(ARMGNU)-objdump -D sharp02.elf > sharp02.list
	$(ARMGNU)-objcopy sharp02.elf sharp02.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

