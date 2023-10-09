#include "riscv_asm.h"
#include "riscv_encoding.h"

void setup_features(void)
{
	unsigned int i, cpu_type, cpu_ver;
	unsigned long version[8];
	unsigned long tmp;
	for (i = 0; i < 8; i++)
		version[i] = csr_read(CSR_MCPUID);

	cpu_type = (version[0] >> 18) & 0xf;
	cpu_ver  = (version[1] >> 12) & 0xffff;

	/*
	 * Warning: CSR_MCCR2 contains an L2 cache latency setting,
	 * you need to confirm it by your own soc design.
	 */
	switch (cpu_type) {
	case 0x3: //910
		if (cpu_ver >= 0x1080 && cpu_ver <= 0x10bf) {
			csr_write(CSR_MCCR2, 0xe0010009);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x6e30c);
			csr_write(CSR_MHCR, 0x11ff);
		} else if (cpu_ver >= 0x2000 && cpu_ver <= 0xffff) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xe249000b);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x31ea32c);
			csr_write(CSR_MHINT2, 0x180);
			csr_write(CSR_MHCR, 0x11ff);
			tmp = csr_read(CSR_MENVCFG);
			tmp |= 0x4000000000000000 ;
			csr_write(CSR_MENVCFG, tmp);
		}
		 else if (cpu_ver >= 0x1200 && cpu_ver < 0x2000) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xe249000b);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x1ee30c);
			csr_write(CSR_MHINT2, 0x180);
			csr_write(CSR_MHCR, 0x11ff);
		} else if (cpu_ver >= 0x1100 && cpu_ver < 0x1200) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xe0010009);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x16e30c);
			csr_write(CSR_MHCR, 0x11ff);
		} else {
			while(1);
		}
		break;
	case 0x4:
		if (cpu_ver >= 0x1002 && cpu_ver <= 0xffff) {
			csr_write(CSR_MHCR, 0x17f);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x650c);
		} else {
			while(1);
		}
		break;
	case 0x5: //908
		if (cpu_ver >= 0x0000 && cpu_ver <= 0x0007) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xe0420008);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x2c50c);
			csr_write(CSR_MHCR, 0x11ff);
		} else if (cpu_ver >= 0x00c0 && cpu_ver <= 0xffff) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xa042000a);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x21aa10c);
			csr_write(CSR_MHCR, 0x10011ff);
		} else if (cpu_ver >= 0x0040 && cpu_ver < 0x00c0) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xa042000a);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x21ac10c);
			csr_write(CSR_MHCR, 0x10011ff);
		} else {
			while(1);
		}
		break;
	default:
		while(1);
	}
}
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
