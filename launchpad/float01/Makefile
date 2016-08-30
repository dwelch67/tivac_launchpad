
ARMGNU?=arm-none-eabi
#ARMGNU=arm-linux-gnueabi
#ARMGNU=arm-none-linux-gnueabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

gcc : float01.gcc.thumb.bin float01.gcc.thumb2.bin 

all : gcc

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) -mthumb -mfpu=vfp vectors.s -o vectors.o

float01.gcc.thumb.o : float01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c float01.c -o float01.gcc.thumb.o

float01.gcc.thumb2.o : float01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7e-m -c float01.c -o float01.gcc.thumb2.o

float01.gcc.thumb.bin : memmap vectors.o float01.gcc.thumb.o
	$(ARMGNU)-ld -o float01.gcc.thumb.elf -T memmap vectors.o float01.gcc.thumb.o
	$(ARMGNU)-objdump -D float01.gcc.thumb.elf > float01.gcc.thumb.list
	$(ARMGNU)-objcopy float01.gcc.thumb.elf float01.gcc.thumb.bin -O binary

float01.gcc.thumb2.bin : memmap vectors.o float01.gcc.thumb2.o
	$(ARMGNU)-ld -o float01.gcc.thumb2.elf -T memmap vectors.o float01.gcc.thumb2.o
	$(ARMGNU)-objdump -D float01.gcc.thumb2.elf > float01.gcc.thumb2.list
	$(ARMGNU)-objcopy float01.gcc.thumb2.elf float01.gcc.thumb2.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

