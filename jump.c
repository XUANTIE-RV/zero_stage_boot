#include "riscv_asm.h"
#include "riscv_encoding.h"
/*
 * set $opensbi_addr = 0x60000000
 * set $vmlinux_addr = $opensbi_addr + 0x00400000
 * set $rootfs_addr  = $opensbi_addr + 0x04000000
 * set $dtb_addr     = $rootfs_addr  - 0x00100000
 * set $zsb_addr     = $rootfs_addr  - 0x00008000
 * set $dyninfo_addr = $rootfs_addr  - 0x40
 */
extern unsigned long _load_start[];
void next_stage(void){
	unsigned long dtb_addr, dyn_info_addr;
	void (*next_stage_entry)(unsigned long arg0,unsigned long arg1,unsigned long arg2);

	unsigned long entry = _load_start[0];
	dtb_addr = entry - (0x00100000 -0x00008000);
	dyn_info_addr = entry + (0x00008000 - 0x40);
	next_stage_entry = (void (*)())(entry - (0x04000000 - 0x00008000));
	/*
	 * set $a0 = 0
	 * set $a1 = $dtb_addr
	 * set $a2 = $dyn_info_addr
	 */
	next_stage_entry(csr_read(CSR_MHARTID), dtb_addr ,dyn_info_addr);
}
