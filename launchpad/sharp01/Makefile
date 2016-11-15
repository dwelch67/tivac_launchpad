
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : sharp01.bin


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

sharp01.o : sharp01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c sharp01.c -o sharp01.o

sharp01.bin : memmap vectors.o sharp01.o
	$(ARMGNU)-ld -o sharp01.elf -T memmap vectors.o sharp01.o
	$(ARMGNU)-objdump -D sharp01.elf > sharp01.list
	$(ARMGNU)-objcopy sharp01.elf sharp01.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

