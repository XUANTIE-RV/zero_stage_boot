.global entry
.global _load_start

#define BOOT_STATUS_RELOCATE_DONE	1

entry:
        /*
         * Clear L1 cache & BTB & BHT ...
         */
        li      t0, 0x70013
        csrw    0x7c2, t0

	/*
	 * Only one core could do relocate
	 */
	lla     t0, _relocate_lottery
	li      t1, 1
	amoadd.w t0, t1, (t0)
	bnez t0, _wait_relocate_copy_done
	lla	t0, _load_start
	lla	t1, entry
	sd	t1, 0(t0)
	/* relocate the global table content */
	lla	t0, _link_start
	ld  t0, 0(t0)
	sub	t2, t1, t0
	lla	t0, _runtime_offset
	sd	t2, 0(t0)
	lla	t0, _link_data_end
	ld  t1, 16(t0)
	add t1, t2, t1
	sd t1, 16(t0)
	li t0, BOOT_STATUS_RELOCATE_DONE
	lla	t1, _boot_status
	sd t0, 0(t1)
_relocate_copy_done:
	csrr t0, mhartid
	li t1, 0x400
	mul t1, t1, t0
	lla sp, stacks
	add sp, sp, t1
	call init_cpu
	call setup_features
	call next_stage
loop:
    j loop

_wait_relocate_copy_done:
	li	t0, BOOT_STATUS_RELOCATE_DONE
	lla	t1, _boot_status
	ld	t1, 0(t1)
	/* Reduce the bus traffic so that boot hart may proceed faster */
	nop
	nop
	nop
	bne	t0, t1, _wait_relocate_copy_done
	j _relocate_copy_done
#	ebreak
.align 8
.data
_runtime_offset:
	.dword 0
_relocate_lottery:
	.dword 0
_boot_status:
	.dword 0
_load_start:
	.dword _fw_start
_link_start:
	.dword _fw_start
_link_end:
	.dword _fw_end
_link_data_end:
	.dword _data_end

.bss
.skip 0x1000
stacks:

