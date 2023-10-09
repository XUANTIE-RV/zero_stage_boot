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

	/*
	 * Warning: CSR_MCCR2 contains an L2 cache latency setting,
	 * you need to confirm it by your own soc design.
	 */
	switch (cpu_type) {
	case 0x3:
		if (cpu_ver >= 0x1180 && cpu_ver <= 0x123f) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xe249000b);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x1ee30c);
			csr_write(CSR_MHINT2, 0x180);
			csr_write(CSR_MHCR, 0x11ff);
		} else if (cpu_ver >= 0x2000 && cpu_ver <= 0xffff) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xe249000b);
			csr_write(CSR_MXSTATUS, 0x438000);
			csr_write(CSR_MHINT, 0x31ea32c);
			csr_write(CSR_MHINT2, 0x180);
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
	case 0x5:
		if (cpu_ver >= 0x0000 && cpu_ver <= 0x0007) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xe0420008);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x2c50c);
			csr_write(CSR_MHCR, 0x11ff);
		} else if (cpu_ver >= 0x0040 && cpu_ver <= 0xffff) {
			csr_write(CSR_MSMPR, 0x1);
			csr_write(CSR_MCCR2, 0xa042000a);
			csr_write(CSR_MXSTATUS, 0x638000);
			csr_write(CSR_MHINT, 0x21aa10c);
			csr_write(CSR_MHCR, 0x10011ff);
		} else {
			while(1);
		}
		break;
	default:
		while(1);
	}
}
