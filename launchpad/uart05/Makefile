
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : uart05.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart05.o : uart05.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -c uart05.c -o uart05.o

uart05.bin : memmap vectors.o uart05.o
	$(ARMGNU)-ld -o uart05.elf -T memmap vectors.o uart05.o
	$(ARMGNU)-objdump -D uart05.elf > uart05.list
	$(ARMGNU)-objcopy uart05.elf uart05.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

