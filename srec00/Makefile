
RISCVGNU ?= riscv32-none-elf
#RISCVGNU ?= riscv64-none-elf

MARCH = -march=rv32i
#MARCH = -march=rv64i

AOPS = --warn --fatal-warnings $(MARCH)
COPS = -Wall -O2 -ffreestanding $(MARCH) -mabi=ilp32
#COPS = -Wall -O2 -ffreestanding $(MARCH) -mabi=lp64
LOPS = -nostdlib 

all : srec.bin srec.header

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.srec
	rm -f mkheader

start.o : start.s
	$(RISCVGNU)-as $(AOPS) start.s -o start.o

srec.o : srec.c
	$(RISCVGNU)-gcc $(COPS) -c srec.c -o srec.o

uart.o : uart.c
	$(RISCVGNU)-gcc $(COPS) -c uart.c -o uart.o

srec.bin :                    memmap.ld start.o srec.o uart.o
	$(RISCVGNU)-ld $(LOPS) -T memmap.ld start.o srec.o uart.o -o srec.elf
	$(RISCVGNU)-objdump -Mnumeric -D srec.elf > srec.list
	#$(RISCVGNU)-objcopy --srec-forceS3 srec.elf -O srec srec.srec
	$(RISCVGNU)-objcopy -O binary srec.elf srec.bin

mkheader : mkheader.c
	gcc -O2 mkheader.c -o mkheader

srec.header : srec.bin mkheader
	./mkheader srec.bin srec.header
