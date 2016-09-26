
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

gcc : uart07.thumb.bin uart07.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart07.thumb.o : uart07.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart07.c -o uart07.thumb.o

uart07.thumb2.o : uart07.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c uart07.c -o uart07.thumb2.o

uart07.thumb.bin : memmap vectors.o uart07.thumb.o
	$(ARMGNU)-ld -o uart07.thumb.elf -T memmap vectors.o uart07.thumb.o
	$(ARMGNU)-objdump -D uart07.thumb.elf > uart07.thumb.list
	$(ARMGNU)-objcopy uart07.thumb.elf uart07.thumb.bin -O binary

uart07.thumb2.bin : memmap vectors.o uart07.thumb2.o
	$(ARMGNU)-ld -o uart07.thumb2.elf -T memmap vectors.o uart07.thumb2.o
	$(ARMGNU)-objdump -D uart07.thumb2.elf > uart07.thumb2.list
	$(ARMGNU)-objcopy uart07.thumb2.elf uart07.thumb2.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

