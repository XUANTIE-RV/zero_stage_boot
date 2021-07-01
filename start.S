.global entry
entry:
	la sp, stacks
	call init_cpu
	call setup_features
	ebreak

.bss
stacks:
	.skip 0x1000
