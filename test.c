#include "riscv_asm.h"
#include "riscv_encoding.h"

void setup_pmp(void)
{
	/*
	# 0x00000000 ~ 0xf0000000 DDR, TOR rwx
	set $pmpaddr0 = 0xf0000000 >> 2
	
	# 0xf0000000 ~ 0xf8000000 plic, NAPOT rw
	set $pmpaddr1 = 0xf0000000 >> 2 | ((0x8000000 - 1) >> 3)
	
	# 0xfff73000 ~ 0xfff74000 serial, NAPOT rw
	set $pmpaddr2 = 0xfff73000 >> 2 | ((0x1000 - 1) >> 3)
	
	# 0xfffc0000 ~ 0xfffc2000 gmac, NAPOT rw
	set $pmpaddr3 = 0xfffc0000 >> 2 | ((0x2000 - 1) >> 3)
	
	# 0x00000000 ~ 0x10000000 NAPOT no access
	set $pmpaddr4 = 0x00000000 >> 2 | ((0x10000000 - 1) >> 3)
	
	# 0x10000000 ~ 0x10000000000 TOR no access
	set $pmpaddr5 = 0xffffffffff >> 2
	
	set $pmpcfg0 = 0x88989b9b9b8f
	*/
	csr_write(CSR_PMPADDR0, 0xf0000000 >> 2);
	csr_write(CSR_PMPADDR1, 0xf0000000 >> 2 | ((0x8000000 - 1) >> 3));
	csr_write(CSR_PMPADDR2, 0xfff73000 >> 2 | ((0x1000 - 1) >> 3));
	csr_write(CSR_PMPADDR3, 0xfffc0000 >> 2 | ((0x2000 - 1) >> 3));
	csr_write(CSR_PMPADDR4, 0x00000000 >> 2 | ((0x10000000 - 1) >> 3));
	csr_write(CSR_PMPADDR5, 0xffffffffff >> 2);
	csr_write(CSR_PMPCFG0, 0x88989b9b9b8f);
}

void set_cpu_regs(void)
{
	/*
	# Invalidate & Clear IBP BTB BHT ICache & DCache
	set $mcor = 0x70013
	
	# Enable L2 Cache
	set $mccr2 = 0xe0010009
	
	# Enable L1 Cache
	set $mhcr = 0x11ff
	
	# Enable CPU Features
	set $mxstatus = 0x638000
	set $mhint = 0x16e30c
	*/
	csr_write(CSR_MCOR, 0x70013);
	csr_write(CSR_MCCR2, 0xe0010009);
	csr_write(CSR_MHCR, 0x11ff);
	csr_write(CSR_MXSTATUS, 0x638000);
	csr_write(CSR_MHINT, 0x16e30c);
}

void setup_dtb_addr(void)
{
	asm volatile ("li a1, 0x00f00000");
}
