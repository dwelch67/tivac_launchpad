
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU)

LLCOPS = -march=thumb -mtriple=$(ARMGNU)

LLCOPS2 = -march=thumb -mtriple=$(ARMGNU) -mcpu=cortex-m3

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug



gcc : svc01.gcc.thumb.bin svc01.gcc.thumb2.bin 

llvm : svc01.clang.thumb.norm.bin svc01.clang.thumb.opt.bin svc01.clang.thumb2.norm.bin svc01.clang.thumb2.opt.bin

llvm-test : svc01.clang.thumb2.test.bin

all : gcc llvm llvm-test

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

svc01.gcc.thumb.o : svc01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c svc01.c -o svc01.gcc.thumb.o

svc01.gcc.thumb2.o : svc01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c svc01.c -o svc01.gcc.thumb2.o

svc01.gcc.thumb.bin : memmap vectors.o svc01.gcc.thumb.o
	$(ARMGNU)-ld -o svc01.gcc.thumb.elf -T memmap vectors.o svc01.gcc.thumb.o
	$(ARMGNU)-objdump -D svc01.gcc.thumb.elf > svc01.gcc.thumb.list
	$(ARMGNU)-objcopy svc01.gcc.thumb.elf svc01.gcc.thumb.bin -O binary

svc01.gcc.thumb2.bin : memmap vectors.o svc01.gcc.thumb2.o
	$(ARMGNU)-ld -o svc01.gcc.thumb2.elf -T memmap vectors.o svc01.gcc.thumb2.o
	$(ARMGNU)-objdump -D svc01.gcc.thumb2.elf > svc01.gcc.thumb2.list
	$(ARMGNU)-objcopy svc01.gcc.thumb2.elf svc01.gcc.thumb2.bin -O binary


svc01.clang.bc : svc01.c
	clang $(LOPS) -c svc01.c -o svc01.clang.bc

svc01.clang.thumb.norm.bin : memmap vectors.o svc01.clang.bc
	llc $(LLCOPS) svc01.clang.bc -o svc01.clang.thumb.norm.s
	$(ARMGNU)-as svc01.clang.thumb.norm.s -o svc01.clang.thumb.norm.o
	$(ARMGNU)-ld -o svc01.clang.thumb.norm.elf -T memmap vectors.o svc01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D svc01.clang.thumb.norm.elf > svc01.clang.thumb.norm.list
	$(ARMGNU)-objcopy svc01.clang.thumb.norm.elf svc01.clang.thumb.norm.bin -O binary

svc01.clang.thumb.opt.bin : memmap vectors.o svc01.clang.bc
	opt $(OOPS) svc01.clang.bc -o svc01.clang.thumb.opt.bc
	llc $(LLCOPS) svc01.clang.thumb.opt.bc -o svc01.clang.thumb.opt.s
	$(ARMGNU)-as svc01.clang.thumb.opt.s -o svc01.clang.thumb.opt.o
	$(ARMGNU)-ld -o svc01.clang.thumb.opt.elf -T memmap vectors.o svc01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D svc01.clang.thumb.opt.elf > svc01.clang.thumb.opt.list
	$(ARMGNU)-objcopy svc01.clang.thumb.opt.elf svc01.clang.thumb.opt.bin -O binary

svc01.clang.thumb2.norm.bin : memmap vectors.o svc01.clang.bc
	llc $(LLCOPS2) svc01.clang.bc -o svc01.clang.thumb2.norm.s
	$(ARMGNU)-as svc01.clang.thumb2.norm.s -o svc01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o svc01.clang.thumb2.norm.elf -T memmap vectors.o svc01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D svc01.clang.thumb2.norm.elf > svc01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy svc01.clang.thumb2.norm.elf svc01.clang.thumb2.norm.bin -O binary

svc01.clang.thumb2.opt.bin : memmap vectors.o svc01.clang.bc
	opt $(OOPS) svc01.clang.bc -o svc01.clang.thumb2.opt.bc
	llc $(LLCOPS2) svc01.clang.thumb2.opt.bc -o svc01.clang.thumb2.opt.s
	$(ARMGNU)-as svc01.clang.thumb2.opt.s -o svc01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o svc01.clang.thumb2.opt.elf -T memmap vectors.o svc01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D svc01.clang.thumb2.opt.elf > svc01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy svc01.clang.thumb2.opt.elf svc01.clang.thumb2.opt.bin -O binary



svc01.clang.thumb2.test.bin : memmap vectors.o svc01.clang.bc
	opt $(OOPS) svc01.clang.bc -o svc01.clang.thumb2.test.bc
	llc $(LLCOPS2) -filetype=obj svc01.clang.thumb2.test.bc -o svc01.clang.thumb2.test.o
	$(ARMGNU)-ld -o svc01.clang.thumb2.test.elf -T memmap vectors.o svc01.clang.thumb2.test.o
	$(ARMGNU)-objdump -D svc01.clang.thumb2.test.elf > svc01.clang.thumb2.test.list
	$(ARMGNU)-objcopy svc01.clang.thumb2.test.elf svc01.clang.thumb2.test.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

