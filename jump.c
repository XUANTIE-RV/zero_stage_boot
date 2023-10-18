#include "riscv_asm.h"
#include "riscv_encoding.h"

extern unsigned long _load_start;

void next_stage(void) {
	unsigned long dtb_addr, dyn_info_addr;
	void (*next_entry)(unsigned long arg0,unsigned long arg1,unsigned long arg2);

	dtb_addr	= _load_start - (0x00100000 -0x00008000);
	dyn_info_addr	= _load_start + (0x00008000 - 0x40);
	next_entry	= (void (*))(_load_start - (0x04000000 - 0x00008000));

	/*
	 * set $a0 = hartid
	 * set $a1 = $dtb_addr
	 * set $a2 = $dyn_info_addr
	 */
	next_entry(csr_read(CSR_MHARTID), dtb_addr ,dyn_info_addr);
}
