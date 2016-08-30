
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings


gcc : uart05.gcc.thumb.bin uart05.gcc.thumb2.bin 

all : gcc

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart05.gcc.thumb.o : uart05.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart05.c -o uart05.gcc.thumb.o

uart05.gcc.thumb2.o : uart05.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart05.c -o uart05.gcc.thumb2.o

uart05.gcc.thumb.bin : memmap vectors.o uart05.gcc.thumb.o
	$(ARMGNU)-ld -o uart05.gcc.thumb.elf -T memmap vectors.o uart05.gcc.thumb.o
	$(ARMGNU)-objdump -D uart05.gcc.thumb.elf > uart05.gcc.thumb.list
	$(ARMGNU)-objcopy uart05.gcc.thumb.elf uart05.gcc.thumb.bin -O binary

uart05.gcc.thumb2.bin : memmap vectors.o uart05.gcc.thumb2.o
	$(ARMGNU)-ld -o uart05.gcc.thumb2.elf -T memmap vectors.o uart05.gcc.thumb2.o
	$(ARMGNU)-objdump -D uart05.gcc.thumb2.elf > uart05.gcc.thumb2.list
	$(ARMGNU)-objcopy uart05.gcc.thumb2.elf uart05.gcc.thumb2.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

