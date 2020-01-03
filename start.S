.global entry
entry:
	la sp, stacks
	call setup_pmp
	call set_cpu_regs
	call setup_dtb_addr
	jr zero
	ebreak

.bss
stacks:
	.skip 0x1000
