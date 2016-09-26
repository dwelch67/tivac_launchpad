
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart01.thumb.bin uart01.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart01.thumb.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart01.c -o uart01.thumb.o

uart01.thumb2.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart01.c -o uart01.thumb2.o

uart01.thumb.bin : memmap vectors.o uart01.thumb.o
	$(ARMGNU)-ld -o uart01.thumb.elf -T memmap vectors.o uart01.thumb.o
	$(ARMGNU)-objdump -D uart01.thumb.elf > uart01.thumb.list
	$(ARMGNU)-objcopy uart01.thumb.elf uart01.thumb.bin -O binary

uart01.thumb2.bin : memmap vectors.o uart01.thumb2.o
	$(ARMGNU)-ld -o uart01.thumb2.elf -T memmap vectors.o uart01.thumb2.o
	$(ARMGNU)-objdump -D uart01.thumb2.elf > uart01.thumb2.list
	$(ARMGNU)-objcopy uart01.thumb2.elf uart01.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

