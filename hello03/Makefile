
#RISCVGNU ?= riscv32-none-elf
RISCVGNU ?= riscv64-none-elf

#MARCH = -march=rv32i
MARCH = -march=rv64i

AOPS = --warn --fatal-warnings $(MARCH)
#COPS = -Wall -O2 -ffreestanding $(MARCH) -mabi=ilp32
COPS = -Wall -O2 -ffreestanding $(MARCH) -mabi=lp64
LOPS = -nostdlib 

all : notmain.bin notmain.header

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.srec
	rm -f mkheader

start.o : start.s
	$(RISCVGNU)-as $(AOPS) start.s -o start.o

notmain.o : notmain.c
	$(RISCVGNU)-gcc $(COPS) -c notmain.c -o notmain.o

notmain.bin :               memmap.ld start.o notmain.o
	$(RISCVGNU)-ld $(LOPS) -T memmap.ld start.o notmain.o -o notmain.elf
	$(RISCVGNU)-objdump -Mnumeric -D notmain.elf > notmain.list
	$(RISCVGNU)-objcopy --srec-forceS3 notmain.elf -O srec notmain.srec
	$(RISCVGNU)-objcopy -O binary notmain.elf notmain.bin

mkheader : mkheader.c
	gcc -O2 mkheader.c -o mkheader

notmain.header : notmain.bin mkheader
	./mkheader notmain.bin notmain.header
