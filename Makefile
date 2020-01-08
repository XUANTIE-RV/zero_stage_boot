CC      =   $(CROSS_COMPILE)gcc
CPP     =   $(CROSS_COMPILE)cpp
AR      =   $(CROSS_COMPILE)ar
LD      =   $(CROSS_COMPILE)ld
OBJCOPY     =   $(CROSS_COMPILE)objcopy

zero_stage_boot.elf: start.o test.o
	$(LD) -Ttest.lds $^ -o $@
	$(OBJCOPY) -O binary $@ zero_stage_boot.bin

test.o: test.c
	$(CC) -mcmodel=medany -g -O0 -c $^

start.o: start.S
	$(CC) -g -O0 -c $^

clean:
	-rm -rf test *.o *.bin *.elf 123
