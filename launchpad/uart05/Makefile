
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart05.thumb.bin uart05.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart05.thumb.o : uart05.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart05.c -o uart05.thumb.o

uart05.thumb2.o : uart05.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart05.c -o uart05.thumb2.o

uart05.thumb.bin : memmap vectors.o uart05.thumb.o
	$(ARMGNU)-ld -o uart05.thumb.elf -T memmap vectors.o uart05.thumb.o
	$(ARMGNU)-objdump -D uart05.thumb.elf > uart05.thumb.list
	$(ARMGNU)-objcopy uart05.thumb.elf uart05.thumb.bin -O binary

uart05.thumb2.bin : memmap vectors.o uart05.thumb2.o
	$(ARMGNU)-ld -o uart05.thumb2.elf -T memmap vectors.o uart05.thumb2.o
	$(ARMGNU)-objdump -D uart05.thumb2.elf > uart05.thumb2.list
	$(ARMGNU)-objcopy uart05.thumb2.elf uart05.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

