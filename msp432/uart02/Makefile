
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : uart02.gcc.thumb.sram.bin

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

uart02.gcc.thumb.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart02.c -o uart02.gcc.thumb.o

uart02.gcc.thumb.sram.bin : sram.ld sram.o uart02.gcc.thumb.o
	$(ARMGNU)-ld -o uart02.gcc.thumb.sram.elf -T sram.ld sram.o uart02.gcc.thumb.o
	$(ARMGNU)-objdump -D uart02.gcc.thumb.sram.elf > uart02.gcc.thumb.sram.list
	$(ARMGNU)-objcopy uart02.gcc.thumb.sram.elf uart02.gcc.thumb.sram.bin -O binary

