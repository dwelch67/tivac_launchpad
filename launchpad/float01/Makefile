
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

gcc : float01.bin 

all : gcc

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) -mthumb -mfpu=vfp vectors.s -o vectors.o

float01.o : float01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c float01.c -o float01.o

float01.bin : memmap vectors.o float01.o
	$(ARMGNU)-ld -o float01.elf -T memmap vectors.o float01.o
	$(ARMGNU)-objdump -D float01.elf > float01.list
	$(ARMGNU)-objcopy float01.elf float01.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

