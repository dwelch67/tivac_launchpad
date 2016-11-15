
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : svc01.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

svc01.o : svc01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c svc01.c -o svc01.o

svc01.bin : memmap vectors.o svc01.o
	$(ARMGNU)-ld -o svc01.elf -T memmap vectors.o svc01.o
	$(ARMGNU)-objdump -D svc01.elf > svc01.list
	$(ARMGNU)-objcopy svc01.elf svc01.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

