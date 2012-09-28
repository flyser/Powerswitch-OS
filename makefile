ASM       = nasm
CC        = gcc
CFLAGS    = -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -march=i586 -m32 -Wwrite-strings
LINK      = ld
LINKFLAGS = -A i386 -melf_i386
EMU       = qemu-system-i386

OBJFILES = loader.o kernel.o io.o
 
all: kernel.img
 
.s.o:
	$(ASM) -f elf -o $@ $<
 
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

kernel.bin: $(OBJFILES)
	$(LD) $(LINKFLAGS) -T linker.ld -o $@ $^
 
kernel.img: kernel.bin
	dd if=/dev/zero of=pad bs=1 count=750
	cat stage1 stage2 pad $< > $@
 
clean:
	$(RM) $(OBJFILES) kernel.bin kernel.img
 
install:
	$(RM) $(OBJFILES) kernel.bin
	
start:
	$(EMU) -kernel kernel.bin
	#$(EMU) -fda kernel.img