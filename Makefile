CC      =   $(CROSS_COMPILE)gcc
CPP     =   $(CROSS_COMPILE)cpp
AR      =   $(CROSS_COMPILE)ar
LD      =   $(CROSS_COMPILE)ld
OBJCOPY     =   $(CROSS_COMPILE)objcopy

main.elf: start.o main.o
	$(LD) -Tlink.lds $^ -o $@
	$(OBJCOPY) -O binary $@ main.bin

main.o: main.c
	$(CC) -mcmodel=medany -g -c $^

start.o: start.S
	$(CC) -g -c $^

clean:
	-rm -rf *.o *.bin *.elf
