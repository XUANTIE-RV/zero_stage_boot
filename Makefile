zero_stage_boot.elf: start.o test.o
	~/nixon/bin/riscv64-unknown-linux-gnu-ld -Ttest.lds $^ -o $@
	~/nixon/bin/riscv64-unknown-linux-gnu-objcopy -O binary $@ zero_stage_boot.bin

test.o: test.c
	~/nixon/bin/riscv64-unknown-linux-gnu-gcc -mcmodel=medany -g -O0 -c $^

start.o: start.S
	~/nixon/bin/riscv64-unknown-linux-gnu-gcc -g -O0 -c $^

clean:
	-rm -rf test *.o *.bin *.elf 123
