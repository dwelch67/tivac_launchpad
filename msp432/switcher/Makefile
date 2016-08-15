
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : switcher.gcc.thumb.sram.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

switcher.gcc.thumb.o : switcher.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c switcher.c -o switcher.gcc.thumb.o

switcher.gcc.thumb.sram.bin : sram.ld sram.o switcher.gcc.thumb.o
	$(ARMGNU)-ld -o switcher.gcc.thumb.sram.elf -T sram.ld sram.o switcher.gcc.thumb.o
	$(ARMGNU)-objdump -D switcher.gcc.thumb.sram.elf > switcher.gcc.thumb.sram.list
	$(ARMGNU)-objcopy switcher.gcc.thumb.sram.elf switcher.gcc.thumb.sram.bin -O binary

