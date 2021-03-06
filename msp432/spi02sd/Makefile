
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : spi02.gcc.thumb.sram.bin crc7

all : gcc clang

clang : spi02.clang.thumb.norm.flash.bin spi02.clang.thumb.opt.flash.bin spi02.clang.thumb.norm.sram.bin spi02.clang.thumb.opt.sram.bin 

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f crc7

crc7 : crc7.c
	gcc -O2 crc7.c -o crc7
	./crc7

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

spi02.gcc.thumb.o : spi02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c spi02.c -o spi02.gcc.thumb.o

spi02.gcc.thumb.flash.bin : flash.ld flash.o spi02.gcc.thumb.o
	$(ARMGNU)-ld -o spi02.gcc.thumb.flash.elf -T flash.ld flash.o spi02.gcc.thumb.o
	$(ARMGNU)-objdump -D spi02.gcc.thumb.flash.elf > spi02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy spi02.gcc.thumb.flash.elf spi02.gcc.thumb.flash.bin -O binary

spi02.gcc.thumb.sram.bin : sram.ld sram.o spi02.gcc.thumb.o
	$(ARMGNU)-ld -o spi02.gcc.thumb.sram.elf -T sram.ld sram.o spi02.gcc.thumb.o
	$(ARMGNU)-objdump -D spi02.gcc.thumb.sram.elf > spi02.gcc.thumb.sram.list
	$(ARMGNU)-objcopy spi02.gcc.thumb.sram.elf spi02.gcc.thumb.sram.bin -O binary

#---------------------------------

spi02.clang.bc : spi02.c
	clang $(LOPS) -c spi02.c -o spi02.clang.bc

spi02.clang.thumb.norm.flash.bin : flash.ld flash.o spi02.clang.bc
	#llc $(LLCOPS) spi02.clang.bc -o spi02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) spi02.clang.thumb.norm.s -o spi02.clang.thumb.norm.o
	llc $(LLCOPS) spi02.clang.bc -filetype=obj -o spi02.clang.thumb.norm.o
	$(ARMGNU)-ld -o spi02.clang.thumb.norm.flash.elf -T flash.ld flash.o spi02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D spi02.clang.thumb.norm.flash.elf > spi02.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy spi02.clang.thumb.norm.flash.elf spi02.clang.thumb.norm.flash.bin -O binary

spi02.clang.thumb.opt.flash.bin : flash.ld flash.o spi02.clang.bc
	opt $(OOPS) spi02.clang.bc -o spi02.clang.thumb.opt.bc
	#llc $(LLCOPS) spi02.clang.thumb.opt.bc -o spi02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) spi02.clang.thumb.opt.s -o spi02.clang.thumb.opt.o
	llc $(LLCOPS) spi02.clang.thumb.opt.bc -filetype=obj -o spi02.clang.thumb.opt.o
	$(ARMGNU)-ld -o spi02.clang.thumb.opt.flash.elf -T flash.ld flash.o spi02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D spi02.clang.thumb.opt.flash.elf > spi02.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy spi02.clang.thumb.opt.flash.elf spi02.clang.thumb.opt.flash.bin -O binary


spi02.clang.thumb.norm.sram.bin : sram.ld sram.o spi02.clang.bc
	#llc $(LLCOPS) spi02.clang.bc -o spi02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) spi02.clang.thumb.norm.s -o spi02.clang.thumb.norm.o
	llc $(LLCOPS) spi02.clang.bc -filetype=obj -o spi02.clang.thumb.norm.o
	$(ARMGNU)-ld -o spi02.clang.thumb.norm.sram.elf -T sram.ld sram.o spi02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D spi02.clang.thumb.norm.sram.elf > spi02.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy spi02.clang.thumb.norm.sram.elf spi02.clang.thumb.norm.sram.bin -O binary

spi02.clang.thumb.opt.sram.bin : sram.ld sram.o spi02.clang.bc
	opt $(OOPS) spi02.clang.bc -o spi02.clang.thumb.opt.bc
	#llc $(LLCOPS) spi02.clang.thumb.opt.bc -o spi02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) spi02.clang.thumb.opt.s -o spi02.clang.thumb.opt.o
	llc $(LLCOPS) spi02.clang.thumb.opt.bc -filetype=obj -o spi02.clang.thumb.opt.o
	$(ARMGNU)-ld -o spi02.clang.thumb.opt.sram.elf -T sram.ld sram.o spi02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D spi02.clang.thumb.opt.sram.elf > spi02.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy spi02.clang.thumb.opt.sram.elf spi02.clang.thumb.opt.sram.bin -O binary




