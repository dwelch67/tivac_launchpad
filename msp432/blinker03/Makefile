
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : blinker03.sram.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

#---------------------------------

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

blinker03.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker03.c -o blinker03.o

blinker03.sram.bin : sram.ld sram.o blinker03.o
	$(ARMGNU)-ld -o blinker03.sram.elf -T sram.ld sram.o blinker03.o
	$(ARMGNU)-objdump -D blinker03.sram.elf > blinker03.sram.list
	$(ARMGNU)-objcopy blinker03.sram.elf blinker03.sram.bin -O binary

