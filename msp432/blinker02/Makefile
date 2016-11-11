
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : blinker02.sram.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

#---------------------------------

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.o

blinker02.sram.bin : sram.ld sram.o blinker02.o
	$(ARMGNU)-ld -o blinker02.sram.elf -T sram.ld sram.o blinker02.o
	$(ARMGNU)-objdump -D blinker02.sram.elf > blinker02.sram.list
	$(ARMGNU)-objcopy blinker02.sram.elf blinker02.sram.bin -O binary

