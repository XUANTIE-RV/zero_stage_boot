CC      =   $(CROSS_COMPILE)gcc
CPP     =   $(CROSS_COMPILE)cpp
AR      =   $(CROSS_COMPILE)ar
LD      =   $(CROSS_COMPILE)ld
OBJCOPY     =   $(CROSS_COMPILE)objcopy
OBJDUMP     =   $(CROSS_COMPILE)objdump
CFLAGS = -fPIC
TARGET = zero_stage_boot
${TARGET}.elf: start.o main.o
	$(LD) -Tlink.lds $^ -o $@
	$(OBJCOPY) -O binary $@ ${TARGET}.bin
	$(OBJDUMP) -D $@ > ${TARGET}.asm

main.o: main.c
	$(CC) -mcmodel=medany ${CFLAGS} -g -c $^

start.o: start.S
	$(CC) ${CFLAGS} -g -c $^

clean:
	-rm -rf *.o *.bin *.elf
