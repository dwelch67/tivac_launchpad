
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : flash02.gcc.thumb.sram.bin blinker.gcc.thumb.flash.bin

all : gcc clang

clang : flash02.clang.thumb.norm.flash.bin flash02.clang.thumb.opt.flash.bin flash02.clang.thumb.norm.sram.bin flash02.clang.thumb.opt.sram.bin 

clean:
	rm -f *.bin
	rm -f *.bin.c
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f bintoc
	

#---------------------------------

bintoc : bintoc.c
	gcc -O2 bintoc.c -o bintoc

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

flash02.gcc.thumb.o : flash02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c flash02.c -o flash02.gcc.thumb.o

flash02.gcc.thumb.flash.bin : flash.ld flash.o flash02.gcc.thumb.o
	$(ARMGNU)-ld -o flash02.gcc.thumb.flash.elf -T flash.ld flash.o flash02.gcc.thumb.o
	$(ARMGNU)-objdump -D flash02.gcc.thumb.flash.elf > flash02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy flash02.gcc.thumb.flash.elf flash02.gcc.thumb.flash.bin -O binary

flash02.gcc.thumb.sram.bin : sram.ld sram.o flash02.gcc.thumb.o blinker.bin.o
	$(ARMGNU)-ld -o flash02.gcc.thumb.sram.elf -T sram.ld sram.o flash02.gcc.thumb.o blinker.bin.o
	$(ARMGNU)-objdump -D flash02.gcc.thumb.sram.elf > flash02.gcc.thumb.sram.list
	$(ARMGNU)-objcopy flash02.gcc.thumb.sram.elf flash02.gcc.thumb.sram.bin -O binary

blinker.gcc.thumb.o : blinker.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker.c -o blinker.gcc.thumb.o

blinker.gcc.thumb.flash.bin : flash.ld flash.o blinker.gcc.thumb.o
	$(ARMGNU)-ld -o blinker.gcc.thumb.flash.elf -T flash.ld flash.o blinker.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker.gcc.thumb.flash.elf > blinker.gcc.thumb.flash.list
	$(ARMGNU)-objcopy blinker.gcc.thumb.flash.elf blinker.gcc.thumb.flash.bin -O binary

blinker.gcc.thumb.sram.bin : sram.ld sram.o blinker.gcc.thumb.o
	$(ARMGNU)-ld -o blinker.gcc.thumb.sram.elf -T sram.ld sram.o blinker.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker.gcc.thumb.sram.elf > blinker.gcc.thumb.sram.list
	$(ARMGNU)-objcopy blinker.gcc.thumb.sram.elf blinker.gcc.thumb.sram.bin -O binary

blinker.bin.o : bintoc blinker.gcc.thumb.flash.bin
	./bintoc blinker.gcc.thumb.flash.bin blinker.bin.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker.bin.c -o blinker.bin.o

#---------------------------------

flash02.clang.bc : flash02.c
	clang $(LOPS) -c flash02.c -o flash02.clang.bc

flash02.clang.thumb.norm.flash.bin : flash.ld flash.o flash02.clang.bc
	#llc $(LLCOPS) flash02.clang.bc -o flash02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) flash02.clang.thumb.norm.s -o flash02.clang.thumb.norm.o
	llc $(LLCOPS) flash02.clang.bc -filetype=obj -o flash02.clang.thumb.norm.o
	$(ARMGNU)-ld -o flash02.clang.thumb.norm.flash.elf -T flash.ld flash.o flash02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D flash02.clang.thumb.norm.flash.elf > flash02.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy flash02.clang.thumb.norm.flash.elf flash02.clang.thumb.norm.flash.bin -O binary

flash02.clang.thumb.opt.flash.bin : flash.ld flash.o flash02.clang.bc
	opt $(OOPS) flash02.clang.bc -o flash02.clang.thumb.opt.bc
	#llc $(LLCOPS) flash02.clang.thumb.opt.bc -o flash02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) flash02.clang.thumb.opt.s -o flash02.clang.thumb.opt.o
	llc $(LLCOPS) flash02.clang.thumb.opt.bc -filetype=obj -o flash02.clang.thumb.opt.o
	$(ARMGNU)-ld -o flash02.clang.thumb.opt.flash.elf -T flash.ld flash.o flash02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D flash02.clang.thumb.opt.flash.elf > flash02.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy flash02.clang.thumb.opt.flash.elf flash02.clang.thumb.opt.flash.bin -O binary


flash02.clang.thumb.norm.sram.bin : sram.ld sram.o flash02.clang.bc
	#llc $(LLCOPS) flash02.clang.bc -o flash02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) flash02.clang.thumb.norm.s -o flash02.clang.thumb.norm.o
	llc $(LLCOPS) flash02.clang.bc -filetype=obj -o flash02.clang.thumb.norm.o
	$(ARMGNU)-ld -o flash02.clang.thumb.norm.sram.elf -T sram.ld sram.o flash02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D flash02.clang.thumb.norm.sram.elf > flash02.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy flash02.clang.thumb.norm.sram.elf flash02.clang.thumb.norm.sram.bin -O binary

flash02.clang.thumb.opt.sram.bin : sram.ld sram.o flash02.clang.bc
	opt $(OOPS) flash02.clang.bc -o flash02.clang.thumb.opt.bc
	#llc $(LLCOPS) flash02.clang.thumb.opt.bc -o flash02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) flash02.clang.thumb.opt.s -o flash02.clang.thumb.opt.o
	llc $(LLCOPS) flash02.clang.thumb.opt.bc -filetype=obj -o flash02.clang.thumb.opt.o
	$(ARMGNU)-ld -o flash02.clang.thumb.opt.sram.elf -T sram.ld sram.o flash02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D flash02.clang.thumb.opt.sram.elf > flash02.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy flash02.clang.thumb.opt.sram.elf flash02.clang.thumb.opt.sram.bin -O binary




