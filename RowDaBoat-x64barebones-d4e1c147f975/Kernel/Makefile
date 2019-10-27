include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard src/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=


all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	#-o kernel.elf
%.o: %.c
ifeq ($(MEM_MANAGER), FIRST_FIT)
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@ -DMEM_MANAGER=0 -lm
	echo "FIRST FIT MANAGER"
else ifeq ($(MEM_MANAGER), BUDDY)
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@ -DMEM_MANAGER=1 -lm
	echo "BUDDY MANAGER"
else
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@ -DMEM_MANAGER=1 -lm
	echo "DEFAULT MANAGER"
endif

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o src/*.o *.elf *.bin

.PHONY: all clean




#$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
#	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
