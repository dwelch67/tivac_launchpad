
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart04.thumb.bin uart04.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart04.thumb.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart04.c -o uart04.thumb.o

uart04.thumb2.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart04.c -o uart04.thumb2.o

uart04.thumb.bin : memmap vectors.o uart04.thumb.o
	$(ARMGNU)-ld -o uart04.thumb.elf -T memmap vectors.o uart04.thumb.o
	$(ARMGNU)-objdump -D uart04.thumb.elf > uart04.thumb.list
	$(ARMGNU)-objcopy uart04.thumb.elf uart04.thumb.bin -O binary

uart04.thumb2.bin : memmap vectors.o uart04.thumb2.o
	$(ARMGNU)-ld -o uart04.thumb2.elf -T memmap vectors.o uart04.thumb2.o
	$(ARMGNU)-objdump -D uart04.thumb2.elf > uart04.thumb2.list
	$(ARMGNU)-objcopy uart04.thumb2.elf uart04.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

