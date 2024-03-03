CMD=/mnt/c/WINDOWS/System32/cmd.exe

CC=gcc
LD=ld

SRC=$(shell pwd)
BIN=./bin

FLAGS=-Wall -Wextra -ffreestanding -m32 -fno-pie -mgeneral-regs-only -mno-red-zone
LFLAGS=--oformat binary -m elf_i386

CSRC := $(shell find ./ -name "*.c")
CTAR := $(patsubst %.c,%.o,$(CSRC))

## Assembly source files that must be compiled to ELF
ASMSRC := $(SRC)/source/cpu/int/isr.asm $(SRC)/source/krnl/syscall/syscall_isr.asm
## Assembly target files
ASMTAR := $(patsubst %.asm,%.o,$(ASMSRC))

## Files which must be linked first in the kernel
LDPRIORITY := $(BIN)/source/krnl/main.o

## LD files to os entry
## LDOSENTRY := $(BIN)/osentry.o $(BIN)/s/boot/osentry_ata.o

all: prebuild build

prebuild:
	clear
	rm -rf $(BIN)
	mkdir $(BIN)

build: boot $(ASMTAR) $(CTAR)
	$(LD) -o $(BIN)/kernel.bin $(LDPRIORITY) $(filter-out $(LDPRIORITY) $(LDOSENTRY),$(shell find ./ -name "*.o" | xargs)) $(LFLAGS) -T build/kernel.ld
#dd if=/dev/zero bs=1 seek=8192 count=1 of=$(BIN)/osentry.bin
	dd if=/dev/zero bs=1 seek=16383 count=1 of=$(BIN)/kernel.bin
#cat $(BIN)/mbr.bin $(BIN)/registry.bin $(BIN)/osentry.bin > $(BIN)/boot.bin
	nasm source/progs/prog.asm -f bin -o $(BIN)/prog.bin
	cat $(BIN)/mbr.bin $(BIN)/load.bin $(BIN)/startft.bin $(BIN)/kernel.bin $(BIN)/prog.bin > os.bin

boot:
	nasm source/boot/mbr/mbr.asm -f bin -o $(BIN)/mbr.bin
	nasm source/boot/load/load.asm -f bin -o $(BIN)/load.bin
	nasm source/fs/seedfs/startft.asm -f bin -o $(BIN)/startft.bin

%.o: %.c
	mkdir -p $(BIN)/$(shell dirname $<)
# $(CC) $(FLAGS) -c $< -o $(BIN)/$(subst .c,.o,$<) $(addprefix -I ,$(shell dirname $(shell echo $(CSRC) | tr ' ' '\n' | sort -u | xargs)))
	$(CC) $(FLAGS) -c $< -o $(BIN)/$(subst .c,.o,$<) -I$(SRC)/source -I$(SRC)/source/libc

%.o : %.asm
	mkdir -p $(BIN)/$(shell dirname $<)
	nasm $< -f elf -o $(BIN)/$(subst .asm,.o,$<) $(addprefix -i ,$(shell dirname $(shell echo $(CSRC) | tr ' ' '\n' | sort -u | xargs)))

bochs: prebuild build
	$(CMD) /c bochs -f ./.bochsrc

qemu: prebuild build
	$(CMD) /c qemu-system-x86_64 \
	-monitor stdio \
	-m 8M \
	-drive format=raw,file=$(BINDIR)os.bin,if=ide,index=0,media=disk \
	-rtc base=localtime,clock=host,driftfix=slew
	