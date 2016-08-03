
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : stack.gcc.thumb.sram.bin

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

stack.gcc.thumb.o : stack.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c stack.c -o stack.gcc.thumb.o

stack.gcc.thumb.sram.bin : sram.ld sram.o stack.gcc.thumb.o
	$(ARMGNU)-ld -o stack.gcc.thumb.sram.elf -T sram.ld sram.o stack.gcc.thumb.o
	$(ARMGNU)-objdump -D stack.gcc.thumb.sram.elf > stack.gcc.thumb.sram.list
	$(ARMGNU)-objcopy stack.gcc.thumb.sram.elf stack.gcc.thumb.sram.bin -O binary

