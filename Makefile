CC      =   $(CROSS_COMPILE)gcc
CPP     =   $(CROSS_COMPILE)cpp
AR      =   $(CROSS_COMPILE)ar
LD      =   $(CROSS_COMPILE)ld
OBJCOPY =   $(CROSS_COMPILE)objcopy
OBJDUMP =   $(CROSS_COMPILE)objdump
XLEN	:=	$(shell $(CC) -dM -E - < /dev/null | grep '__riscv_xlen' | awk '{print $$3}')
CFLAGS = -march=rv$(XLEN)gc -fPIC -fno-stack-protector -O0
TARGET = zero_stage_boot
${TARGET}.elf: start.o feature.o jump.o
	$(LD) $(CROSS_LDFLAGS) -Tlink.lds $^ -o $@
	$(OBJCOPY) -O binary $@ ${TARGET}.bin
	$(OBJDUMP) -S $@ > ${TARGET}.asm

%.o: %.c
	$(CC) -mcmodel=medany ${CFLAGS} -g -c $^

start.o: start.S
	$(CC) ${CFLAGS} -g -c $^

clean:
	-rm -rf *.o *.bin *.elf *.asm
