#include "riscv_asm.h"
#include "riscv_encoding.h"
#include "fw_dynamic.h"

extern unsigned long _load_start;
static struct fw_dynamic_info info;

void next_stage(void) {
	unsigned long dtb_addr, dyn_info_addr, next_addr;
	void (*next_entry)(unsigned long arg0, unsigned long arg1, unsigned long arg2);

	dtb_addr		= _load_start - (0x00100000 - 0x00008000);
	dyn_info_addr	= (unsigned long)&info;
	next_addr		= _load_start + 0x00008000;
	next_entry		= (void (*))(_load_start - (0x00400000 - 0x00008000));

	info.boot_hart	= 0;
	info.magic		= FW_DYNAMIC_INFO_MAGIC_VALUE;
	info.version	= FW_DYNAMIC_INFO_VERSION_2;
	info.next_addr	= next_addr;
	info.next_mode	= FW_DYNAMIC_INFO_NEXT_MODE_S;
	info.options	= 0;
	info.boot_hart	= 0;

	/*
	 * set $a0 = hartid
	 * set $a1 = $dtb_addr
	 * set $a2 = $dyn_info_addr
	 */
	next_entry(csr_read(CSR_MHARTID), dtb_addr, dyn_info_addr);
}
