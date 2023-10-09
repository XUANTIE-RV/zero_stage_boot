.global _load_start

#define BOOT_STATUS_RELOCATE_DONE	1

entry:
	/*
	 * Clear L1 cache & BTB & BHT ...
	 */
	li      t0, 0x70013
	csrw    0x7c2, t0

	/*
	 * Enable cache coherency
	 */
	li	t0, 1
	csrw	msmpr,t0

	/*
	 * Only one core could do relocate
	 */
	lla     t0, _relocate_lottery
	li      t1, 1
	amoadd.w t0, t1, (t0)
	bnez t0, _wait_relocate_copy_done

	/*
	 * Calculate current load_start
	 */
	lla	t0, _load_start
	ld 	t2, 0(t0)
	lla	t1, entry
	sd	t1, 0(t0)

	/*
	 * Relocate GOT table
	 * Only for one globe variable:
	 * _load_start
	 */
	sub	t2, t1, t2
	lla	t0, _GLOBAL_OFFSET_TABLE_
	ld	t1, 8(t0)
	add	t1, t2, t1
	sd	t1, 8(t0)

	/*
	 * Set relocate done flag, let other harts boot.
	 */
	li	t0, BOOT_STATUS_RELOCATE_DONE
	lla	t1, _boot_status
	sd	t0, 0(t1)
	fence	rw, rw

_relocate_copy_done:
	/*
	 *Prepare percpu stack
	 */
	csrr	t0, mhartid
	li	t1, 0x400
	mul	t1, t1, t0
	lla	sp, stacks
	add	sp, sp, t1

	/*
	 * Call C routine
	 */
	call	setup_features
	call	next_stage

	/*
	 * Never get here, dead loop
	 */
	j .

_wait_relocate_copy_done:
	li	t0, BOOT_STATUS_RELOCATE_DONE
	lla	t1, _boot_status
	ld	t1, 0(t1)

	/* Reduce the bus traffic so that boot hart may proceed faster */
	nop
	nop
	nop
	bne	t0, t1, _wait_relocate_copy_done
	j	_relocate_copy_done

	.align 8
	.data
_relocate_lottery:
	.dword 0
_boot_status:
	.dword 0
_load_start:
	.dword _fw_start

	.bss
	.skip 0x4000
stacks:
