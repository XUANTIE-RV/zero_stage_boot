#include "riscv_asm.h"
#include "riscv_encoding.h"

.global _load_start

#define BOOT_STATUS_RELOCATE_DONE	1

entry:
	/*
	 * Clear L1 cache & BTB & BHT ...
	 */
	li      t0, 0x70013
	csrw    CSR_MCOR, t0

	/*
	 * Enable cache coherency
	 */
	li	t0, 1
	csrw	CSR_MSMPR, t0

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
	REG_L  t2, 0(t0)
	lla	t1, entry
	REG_S	t1, 0(t0)

	/*
	 * Relocate GOT table
	 * Only for one globe variable:
	 * _load_start
	 */
	sub	t2, t1, t2
	lla	t0, _GLOBAL_OFFSET_TABLE_
	REG_L	t1, 8(t0)
	add	t1, t2, t1
	REG_S	t1, 8(t0)

	/*
	 * Set relocate done flag, let other harts boot.
	 */
	fence	rw, rw
	li	t0, BOOT_STATUS_RELOCATE_DONE
	lla	t1, _boot_status
	REG_S	t0, 0(t1)

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
	REG_L	t1, 0(t1)

	/* Reduce the bus traffic so that boot hart may proceed faster */
	nop
	nop
	nop
	bne	t0, t1, _wait_relocate_copy_done
	j	_relocate_copy_done

	.align 8
	.data
_relocate_lottery:
	RISCV_PTR 0
_boot_status:
	RISCV_PTR 0
_load_start:
	RISCV_PTR _fw_start

	.bss
	.skip 0x4000
stacks:
