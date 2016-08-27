
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU)

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings



gcc : sharp01.gcc.thumb.bin sharp01.gcc.thumb2.bin 


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

sharp01.gcc.thumb.o : sharp01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c sharp01.c -o sharp01.gcc.thumb.o

sharp01.gcc.thumb2.o : sharp01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -c sharp01.c -o sharp01.gcc.thumb2.o
	#$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7-m -c sharp01.c -o sharp01.gcc.thumb2.o

sharp01.gcc.thumb.bin : memmap vectors.o sharp01.gcc.thumb.o
	$(ARMGNU)-ld -o sharp01.gcc.thumb.elf -T memmap vectors.o sharp01.gcc.thumb.o
	$(ARMGNU)-objdump -D sharp01.gcc.thumb.elf > sharp01.gcc.thumb.list
	$(ARMGNU)-objcopy sharp01.gcc.thumb.elf sharp01.gcc.thumb.bin -O binary

sharp01.gcc.thumb2.bin : memmap vectors.o sharp01.gcc.thumb2.o
	$(ARMGNU)-ld -o sharp01.gcc.thumb2.elf -T memmap vectors.o sharp01.gcc.thumb2.o
	$(ARMGNU)-objdump -D sharp01.gcc.thumb2.elf > sharp01.gcc.thumb2.list
	$(ARMGNU)-objcopy sharp01.gcc.thumb2.elf sharp01.gcc.thumb2.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

