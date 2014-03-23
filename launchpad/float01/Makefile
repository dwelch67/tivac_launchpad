
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



gcc : float01.gcc.thumb.bin float01.gcc.thumb2.bin 

llvm : float01.clang.thumb.norm.bin float01.clang.thumb.opt.bin float01.clang.thumb2.norm.bin float01.clang.thumb2.opt.bin

llvm-test : float01.clang.thumb2.test.bin

all : gcc llvm llvm-test

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) -mthumb -mfpu=vfp vectors.s -o vectors.o

float01.gcc.thumb.o : float01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c float01.c -o float01.gcc.thumb.o

float01.gcc.thumb2.o : float01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c float01.c -o float01.gcc.thumb2.o

float01.gcc.thumb.bin : memmap vectors.o float01.gcc.thumb.o
	$(ARMGNU)-ld -o float01.gcc.thumb.elf -T memmap vectors.o float01.gcc.thumb.o
	$(ARMGNU)-objdump -D float01.gcc.thumb.elf > float01.gcc.thumb.list
	$(ARMGNU)-objcopy float01.gcc.thumb.elf float01.gcc.thumb.bin -O binary

float01.gcc.thumb2.bin : memmap vectors.o float01.gcc.thumb2.o
	$(ARMGNU)-ld -o float01.gcc.thumb2.elf -T memmap vectors.o float01.gcc.thumb2.o
	$(ARMGNU)-objdump -D float01.gcc.thumb2.elf > float01.gcc.thumb2.list
	$(ARMGNU)-objcopy float01.gcc.thumb2.elf float01.gcc.thumb2.bin -O binary


float01.clang.bc : float01.c
	clang $(LOPS) -c float01.c -o float01.clang.bc

float01.clang.thumb.norm.bin : memmap vectors.o float01.clang.bc
	llc $(LLCOPS) float01.clang.bc -o float01.clang.thumb.norm.s
	$(ARMGNU)-as float01.clang.thumb.norm.s -o float01.clang.thumb.norm.o
	$(ARMGNU)-ld -o float01.clang.thumb.norm.elf -T memmap vectors.o float01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D float01.clang.thumb.norm.elf > float01.clang.thumb.norm.list
	$(ARMGNU)-objcopy float01.clang.thumb.norm.elf float01.clang.thumb.norm.bin -O binary

float01.clang.thumb.opt.bin : memmap vectors.o float01.clang.bc
	opt $(OOPS) float01.clang.bc -o float01.clang.thumb.opt.bc
	llc $(LLCOPS) float01.clang.thumb.opt.bc -o float01.clang.thumb.opt.s
	$(ARMGNU)-as float01.clang.thumb.opt.s -o float01.clang.thumb.opt.o
	$(ARMGNU)-ld -o float01.clang.thumb.opt.elf -T memmap vectors.o float01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D float01.clang.thumb.opt.elf > float01.clang.thumb.opt.list
	$(ARMGNU)-objcopy float01.clang.thumb.opt.elf float01.clang.thumb.opt.bin -O binary

float01.clang.thumb2.norm.bin : memmap vectors.o float01.clang.bc
	llc $(LLCOPS2) float01.clang.bc -o float01.clang.thumb2.norm.s
	$(ARMGNU)-as float01.clang.thumb2.norm.s -o float01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o float01.clang.thumb2.norm.elf -T memmap vectors.o float01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D float01.clang.thumb2.norm.elf > float01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy float01.clang.thumb2.norm.elf float01.clang.thumb2.norm.bin -O binary

float01.clang.thumb2.opt.bin : memmap vectors.o float01.clang.bc
	opt $(OOPS) float01.clang.bc -o float01.clang.thumb2.opt.bc
	llc $(LLCOPS2) float01.clang.thumb2.opt.bc -o float01.clang.thumb2.opt.s
	$(ARMGNU)-as float01.clang.thumb2.opt.s -o float01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o float01.clang.thumb2.opt.elf -T memmap vectors.o float01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D float01.clang.thumb2.opt.elf > float01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy float01.clang.thumb2.opt.elf float01.clang.thumb2.opt.bin -O binary



float01.clang.thumb2.test.bin : memmap vectors.o float01.clang.bc
	opt $(OOPS) float01.clang.bc -o float01.clang.thumb2.test.bc
	llc $(LLCOPS2) -filetype=obj float01.clang.thumb2.test.bc -o float01.clang.thumb2.test.o
	$(ARMGNU)-ld -o float01.clang.thumb2.test.elf -T memmap vectors.o float01.clang.thumb2.test.o
	$(ARMGNU)-objdump -D float01.clang.thumb2.test.elf > float01.clang.thumb2.test.list
	$(ARMGNU)-objcopy float01.clang.thumb2.test.elf float01.clang.thumb2.test.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

