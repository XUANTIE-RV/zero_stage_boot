#include "riscv_asm.h"
#include "riscv_encoding.h"

void init_cpu(void)
{
	/*
         * Invalidate & Clear IBP BTB BHT ICache & DCache
	 */
	csr_write(CSR_MCOR, 0x70013);
}

void setup_features(void)
{
	unsigned int i, cpu_type, cpu_ver;
	unsigned long version[8];

	for (i = 0; i < 8; i++)
		version[i] = csr_read(CSR_MCPUID);

	cpu_type = (version[0] >> 18) & 0xf;
	cpu_ver  = (version[1] >> 12) & 0xffff;

	switch (cpu_type) {
	case 0x3: /* 910 */
		if (cpu_ver >= 0x1080 && cpu_ver <= 0x10bf) {
			csr_write(CSR_MCCR2, 0xe0010009);
			csr_write(CSR_MHCR, 0x11ff);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x6e30c);
		} else if (cpu_ver >= 0x1100 && cpu_ver <= 0xffff) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xe0010009);
			csr_write(CSR_MHCR, 0x11ff);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x6e30c);
		} else {
			while(1);
		}
		break;
	case 0x4: /* 906 */
		if (cpu_ver >= 0x1002 && cpu_ver <= 0xffff) {
			csr_write(CSR_MHCR, 0x17f);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x610c);
		} else {
			while(1);
		}
		break;
	default:
		while(1);
	}
}
