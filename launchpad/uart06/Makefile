
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart06.thumb.bin uart06.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart06.thumb.o : uart06.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart06.c -o uart06.thumb.o

uart06.thumb2.o : uart06.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart06.c -o uart06.thumb2.o

uart06.thumb.bin : memmap vectors.o uart06.thumb.o
	$(ARMGNU)-ld -o uart06.thumb.elf -T memmap vectors.o uart06.thumb.o
	$(ARMGNU)-objdump -D uart06.thumb.elf > uart06.thumb.list
	$(ARMGNU)-objcopy uart06.thumb.elf uart06.thumb.bin -O binary

uart06.thumb2.bin : memmap vectors.o uart06.thumb2.o
	$(ARMGNU)-ld -o uart06.thumb2.elf -T memmap vectors.o uart06.thumb2.o
	$(ARMGNU)-objdump -D uart06.thumb2.elf > uart06.thumb2.list
	$(ARMGNU)-objcopy uart06.thumb2.elf uart06.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

