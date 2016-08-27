
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU)

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings



gcc : sharp02.gcc.thumb.bin sharp02.gcc.thumb2.bin 


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

sharp02.gcc.thumb.o : sharp02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c sharp02.c -o sharp02.gcc.thumb.o

sharp02.gcc.thumb2.o : sharp02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -c sharp02.c -o sharp02.gcc.thumb2.o
	#$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7-m -c sharp02.c -o sharp02.gcc.thumb2.o

sharp02.gcc.thumb.bin : memmap vectors.o sharp02.gcc.thumb.o
	$(ARMGNU)-ld -o sharp02.gcc.thumb.elf -T memmap vectors.o sharp02.gcc.thumb.o
	$(ARMGNU)-objdump -D sharp02.gcc.thumb.elf > sharp02.gcc.thumb.list
	$(ARMGNU)-objcopy sharp02.gcc.thumb.elf sharp02.gcc.thumb.bin -O binary

sharp02.gcc.thumb2.bin : memmap vectors.o sharp02.gcc.thumb2.o
	$(ARMGNU)-ld -o sharp02.gcc.thumb2.elf -T memmap vectors.o sharp02.gcc.thumb2.o
	$(ARMGNU)-objdump -D sharp02.gcc.thumb2.elf > sharp02.gcc.thumb2.list
	$(ARMGNU)-objcopy sharp02.gcc.thumb2.elf sharp02.gcc.thumb2.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

