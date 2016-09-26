
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart02.thumb.bin uart02.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart02.thumb.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart02.c -o uart02.thumb.o

uart02.thumb2.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart02.c -o uart02.thumb2.o

uart02.thumb.bin : memmap vectors.o uart02.thumb.o
	$(ARMGNU)-ld -o uart02.thumb.elf -T memmap vectors.o uart02.thumb.o
	$(ARMGNU)-objdump -D uart02.thumb.elf > uart02.thumb.list
	$(ARMGNU)-objcopy uart02.thumb.elf uart02.thumb.bin -O binary

uart02.thumb2.bin : memmap vectors.o uart02.thumb2.o
	$(ARMGNU)-ld -o uart02.thumb2.elf -T memmap vectors.o uart02.thumb2.o
	$(ARMGNU)-objdump -D uart02.thumb2.elf > uart02.thumb2.list
	$(ARMGNU)-objcopy uart02.thumb2.elf uart02.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

